'use strict';

var Promise = require('bluebird')
  , fs = Promise.promisifyAll(require('fs'))
  , md5 = require('MD5')
  , models = require('../models')
  , log = console.log
  ;

function saveFile(path, buffer) {
    fs.writeFileAsync(path, buffer).then(function() {
        log('Saved buffer to ' + path);
    }).error(function(err) {
        console.err(err);
    });
}

module.exports = function(router) {

    router.post('/', function(req, res) {
        // TODO: make sure a file was uploaded or a url was given, and a username is given.
        log(req.body);
        log(req.files);

        var /** {string} */ user         = req.body.user
          , /** {string} */ path         = req.files.file.path
          , /** {string} */ originalname = req.files.file.originalname
          , /** {buffer} */ buffer       = req.files.file.buffer
          , /** {string} */ hash         = md5(buffer)
          ;

        console.log('File ' + path + ' hashed to ' + hash);

        var imageRec = {
            url : path
          , name: originalname
          , user: user
          , hash: hash
        };

        log('Adding new image to database:', imageRec);

        models.Image.create(imageRec).then(fs.writeFileAsync.bind(null, path, buffer))
                                     .then(res.sendStatus.bind(res, 204))
                                     .error(res.sendStatus.bind(res, 409));
    });
};
