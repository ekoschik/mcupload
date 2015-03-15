'use strict';

var Promise   = require('bluebird')
  , fs        = Promise.promisifyAll(require('fs'))
  , _         = require('lodash')
  , path      = require('path')
  , crypto    = require('crypto')
  , md5       = require('MD5')
  , validate  = require('../lib/validate')
  , uploadDir = require('config').get('uploadDir')
  ;

module.exports = function(router, models) {
    router.get('/', function(req, res) {
        models.Image.findAll({
            order: 'createdAt DESC'
        }).then(function(images) {
            res.render('screenshots.jade', { title: 'Screenshots', 'images': images });
        }).error(function(e) {
            res.sendStatus(500);
        });
    });
    
    router.get('/upload', function(req, res) {
        res.render('upload_form.jade', { title: 'Upload File' });
    });

    router.get('/:id', function(req, res) {
        var id = req.params.id;
        res.sendFile(id, { root: uploadDir }, function(e) {
            if (e) {
                var status = e.status;
                var message = {
                    403: 'Forbidden',
                    404: 'File "' + id + '" was not found.'
                }[status];
                res.status(status).json({ status: status, error: message });
            }
        });
    });

    router.post('/', function(req, res) {
        // TODO: make sure a file was uploaded or a url was given, and a username is given.

        var buffer, filename;

        // if this is a json payload, we must decode the base64 file data
        if (req.is('json')) {
            buffer = new Buffer(req.body.filedata, 'base64');
            filename = req.body.filename;
        } else {
            buffer = req.files.file.buffer;
            filename = req.files.file.originalname;
        }

        validate.isImage(buffer).then(function() {

            var ext  = path.extname(filename)
              , user = req.body.user
              , url  = createFileName() + ext
              , hash = md5(buffer)
              ;

            console.log('File ' + filename + ' hashed to ' + hash);

            var imageRec = {
                url : url
              , name: filename
              , user: user
              , hash: hash
            };

            console.log('Adding new image to database:', imageRec);

            return models.Image.create(imageRec).then(fs.writeFileAsync.bind(null, path.join(uploadDir, url), buffer))
                                                .then(res.sendStatus.bind(res, 204));
        }).catch(validate.ValidationError, function(e) {
            res.status(403).json({ error: 'The uploaded file is not an image.' });
        }).catch(models.sequelize.UniqueConstraintError, function(e) {
            res.status(409).json({ error: 'This file has already been uploaded.'});
        }).catch(function(e) {
            res.status(500).json({ error: e.message });
        });
    });

};

// TODO: don't make this random, base it on the username
function createFileName() {
    return crypto.randomBytes(20).toString('hex');
}

