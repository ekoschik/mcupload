'use strict';

var Promise      = require('bluebird')
  , fs           = Promise.promisifyAll(require('fs'))
  , _            = require('lodash')
  , path         = require('path')
  , crypto       = require('crypto')
  , validate     = require('../lib/validate')
  , config       = require('config')
  , uploadDir    = config.get('uploadDir')
  , mapUrlFormat = config.get('mapUrlFormat')
  , moment       = require('moment')
  , format       = require('string-format')
  , React        = require('react')
  , Screenshots  = require('../views/Screenshots.jsx')
  , UserNotFoundError = require('../lib/errorutils').createError('UserNotFoundError')
  ;

module.exports = function(router, models, io) {

    io.on('connection', function(socket) {
        console.log('a client has connected: ' + socket);
    });

    router.get('/', function(req, res, next) {
        var query = {
            order: 'createdAt DESC',
            attributes: ['url', 'name', 'player', 'createdAt', 'hash']
        };
        models.Image.findAll(query).then(function(images) {
            // TODO: factor this type of functionality out into some kind
            // of middleware
            images = images.map(enrichRec);

            if (req.accepts('html')) {
                var screenshotsFactory = React.createFactory(Screenshots);
                var props = { screenshots: images, imageEvent: 'screenshot' };
                var markup = React.renderToString(screenshotsFactory(props));
                res.render('index', { content: markup, props: JSON.stringify(props) });
            } else if (req.accepts('json')) {
                res.json({ screenshots: images });
            } else {
                res.sendStatus(406);
            }
        }).catch(next);
    });

    router.get('/upload', function(req, res) {
        res.render('upload', { title: 'Upload File' });
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
            }).then(enrichRec).then(res.json.bind(res)).catch(next);
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
        }).then(function(player) {

            if (player === null) {
                throw new UserNotFoundError('Player ' + req.body.user + ' was not found.');
            }

            return models.PlayerLocation.find({
                where: {
                    player: player.uuid,
                    createdAt: { $gt: moment().subtract(5, 'minutes').format() }
                },
                limit: 1
            }).then(function(location) {

                location = location || {};

                console.log('got location: ' + location);

                var ext  = path.extname(filename)
                  , hash = md5(buffer)
                  , url  = hash + ext
                  ;

                console.log('File ' + filename + ' hashed to ' + hash);

                var imageRec = {
                    url : url
                  , name: filename
                  , player: player.uuid
                  , hash: hash
                  , x: location.x
                  , y: location.y
                  , z: location.z
                  , dimension: location.dimension
                };

                console.log('Adding new image to database:', imageRec);

                return models.Image.create(imageRec).then(fs.writeFileAsync.bind(null, path.join(uploadDir, url), buffer))
                                                    .then(function() {
                                                        imageRec = enrichRec(imageRec);
                                                        res.json(imageRec);
                                                        io.emit('screenshot', imageRec);
                                                    });
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

/**
 * Get the md5 hash of the given buffer.
 * @param  {buffer} buffer The buffer to hash.
 * @return {string}        The md5 hash of the passed in buffer.
 */
function md5(buffer) {
    var hash = crypto.createHash('md5');
    return hash.update(buffer).digest('hex');
}

/**
 * Return the screenshot endpoint for the given image record.
 * @param  {object} imageRec The image record to get the url for.
 * @return {string}          The url the browser can use to get this image.
 */
function getImageUrl(imageRec) {
    return '/screenshots/' + imageRec.url;
}

/**
 * Return the url to the minecraft map for the given image record's location.
 * @param  {object} imageRec The image record to get the map url for.
 * @return {string}          The map url for the given image record.
 */
function getMapUrl(imageRec) {
    if (imageRec.x == null ||
        imageRec.y == null ||
        imageRec.z == null ||
        imageRec.dimension == null) return null;

    return format(mapUrlFormat, {
        x: imageRec.x,
        y: imageRec.y,
        z: imageRec.z,
        dimension: imageRec.dimension,
        zoom: 8
    });
}

/**
 * Populates the url field and mapUrl field of the given imageRecord.
 * @param  {object} imageRec The image record to enrich.
 * @return {object}          The enriched image record.
 */
function enrichRec(imageRec) {
    return _.extend(imageRec, {
        url: getImageUrl(imageRec),
        mapUrl: getMapUrl(imageRec)
    });
}
