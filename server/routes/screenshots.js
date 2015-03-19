'use strict';

var Promise   = require('bluebird')
  , fs        = Promise.promisifyAll(require('fs'))
  , _         = require('lodash')
  , path      = require('path')
  , crypto    = require('crypto')
  , validate  = require('../lib/validate')
  , uploadDir = require('config').get('uploadDir')
  , React     = require('react')
  , ScreenshotList    = require('../components/ScreenshotList.jsx')
  , UserNotFoundError = require('../lib/errorutils').createError('UserNotFoundError')
  ;

module.exports = function(router, models, io) {

    io.on('connection', function(socket) {
        console.log('a client has connected: ' + socket);
    });

    router.get('/', function(req, res, next) {
        var query = {
            order: 'createdAt DESC',
            attributes: ['url', 'name', 'player', 'createdAt']
        };
        models.Image.findAll(query).then(function(images) {
            // TODO: factor this type of functionality out into some kind
            // of middleware
            if (req.accepts('html')) {
                var markup = getScreenshotListMarkup(images);
                res.render('screenshots.jade', { title: 'Screenshots', markup: markup });
            } else if (req.accepts('json')) {
                res.json({ screenshots: images });
            } else {
                res.sendStatus(406);
            }
        }).catch(next);
    });

    router.get('/upload', function(req, res) {
        res.render('upload_form.jade', { title: 'Upload File' });
    });

    router.get('/:id', function(req, res, next) {
        var id = req.params.id;
        if (req.accepts('html') || req.accepts('image')) {
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
        } else if (req.accepts('json')) {
            models.Image.find({
                where: { url: id }
            }).then(res.json.bind(res)).catch(next);
        }
    });

    router.post('/', function(req, res) {
        // TODO: make sure a file was uploaded or a url was given, and a username is given.
        // TODO: allow a url to be posted and stored

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
            return models.Player.find({
                where: { name: req.body.user },
                attributes: ['uuid']
            });
        }).then(function(user) {

            if (user === null) {
                throw new UserNotFoundError('User ' + req.body.user + ' was not found.');
            }

            var ext  = path.extname(filename)
              , hash = md5(buffer)
              , url  = hash + ext
              ;

            console.log('File ' + filename + ' hashed to ' + hash);

            var imageRec = {
                url : url
              , name: filename
              , player: user.uuid
              , hash: hash
            };

            console.log('Adding new image to database:', imageRec);

            return models.Image.create(imageRec).then(fs.writeFileAsync.bind(null, path.join(uploadDir, url), buffer))
                                                .then(res.sendStatus.bind(res, 204))
                                                .then(function() {
                                                    imageRec.url = getImageUrl(imageRec.url);
                                                    io.emit('screenshot', imageRec);
                                                });
        }).catch(UserNotFoundError, function(e) {
            res.status(401).json({ error: e.message });
        }).catch(validate.ValidationError, function(e) {
            res.status(403).json({ error: 'The uploaded file is not an image.' });
        }).catch(models.sequelize.UniqueConstraintError, function(e) {
            res.status(409).json({ error: 'This file has already been uploaded.'});
        }).catch(function(e) {
            res.status(500).json({ error: e.message });
        });
    });

};

function getScreenshotListMarkup(screenshots) {
    return React.renderComponentToString(
        ScreenshotList({
            screenshots: screenshots
        })
    );
}

function md5(buffer) {
    var hash = crypto.createHash('md5');
    return hash.update(buffer).digest('hex');
}

function getImageUrl(imageId) {
    return 'screenshots/' + imageId;
}
