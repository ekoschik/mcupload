'use strict';

var Promise = require('bluebird')
  , fs = Promise.promisifyAll(require('fs'))
  , join = require('path').join
  , crypto = require('crypto')
  , md5 = require('MD5')
  , log = console.log
  , uploadDir = require('config').get('uploadDir')
  ;

function saveFile(path, buffer) {
    fs.writeFileAsync(path, buffer).then(function() {
        log('Saved buffer to ' + path);
    }).error(function(err) {
        console.err(err);
    });
}

function createFileName() {
    return crypto.randomBytes(20).toString('hex');
}

module.exports = function(router, models) {

    router.post('/', function(req, res) {
        // TODO: make sure a file was uploaded or a url was given, and a username is given.
        log(req.body);
        log(req.files);

        var buffer, filename;

        // if this is a json payload, we must decode the base64 file data
        if (req.headers['content-type'].indexOf('json') !== -1) {
            buffer = new Buffer(req.body.filedata, 'base64');
            filename = req.body.filename;
        } else {
            buffer = req.files.file.buffer;
            filename = req.files.file.originalname;
        }

        var user = req.body.user
          , path = join(uploadDir, createFileName())
          , hash = md5(buffer)
          ;

        console.log('File ' + filename + ' hashed to ' + hash);

        var imageRec = {
            url : path
          , name: filename
          , user: user
          , hash: hash
        };

        log('Adding new image to database:', imageRec);

        models.Image.create(imageRec).then(fs.writeFileAsync.bind(null, path, buffer))
                                     .then(res.sendStatus.bind(res, 204))
                                     .error(res.sendStatus.bind(res, 409));
    });

    router.get('/', function(req, res) {
        res.render('upload_form.jade', { title: 'Upload File' });
    });
};
