'use strict';

var Promise = require('bluebird')
  , mmm = Promise.promisifyAll(require('mmmagic'))
  , magic = new mmm.Magic(mmm.MAGIC_MIME_TYPE)
  , ValidationError = require('./errorutils').createError('ValidationError')
  ;


function isImageMime(mime) {
    return 'image/' === mime.substr(0, 6);
}

var validate = {
    isImage: function(buffer) {
        return magic.detectAsync(buffer).then(function(mime) {
            if (isImageMime(mime)) return true;
            throw new ValidationError('the given buffer is not an image');
        });
    },

    isImageFile: function(path) {
        return magic.detectFileAsync(path).then(function(mime) {
            if (isImageMime(mime)) return true;
            throw new ValidationError('the file "' + path + '" is not an image');
        });
    },

    ValidationError: ValidationError
};

module.exports = validate;


if (require.main === module) {
    var file = process.argv[2];
    validate.isImageFile(file).then(console.log.bind(console, file, 'is a valid image file'))
        .catch(ValidationError, function(e) {
            console.log(e);
        });
}
