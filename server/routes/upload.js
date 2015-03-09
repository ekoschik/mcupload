'use strict';

var Promise = require('bluebird')
  , fs = Promise.promisifyAll(require('fs'))
  , path = require('path')
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

// TODO: don't make this random, base it on the username
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

        var ext  = path.extname(filename)
          , user = req.body.user
          , url  = createFileName() + ext
          , hash = md5(buffer)
          ;

        console.log('File ' + filename + ' hashed to ' + hash);

        // I think we want to validate that the uploaded file is an image...
        var imageRec = {
            url : url
          , name: filename
          , user: user
          , hash: hash
        };

        log('Adding new image to database:', imageRec);

        models.Image.create(imageRec).then(fs.writeFileAsync.bind(null, path.join(uploadDir, url), buffer))
                                     .then(res.sendStatus.bind(res, 204))
                                     .error(res.sendStatus.bind(res, 409));
    });

    router.get('/', function(req, res) {
        res.render('upload_form.jade', { title: 'Upload File' });
    });
};
