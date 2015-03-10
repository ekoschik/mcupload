'use strict';

var errorUtils = {
    createError: function(name) {
        var error = function(message) {
            this.message = message;
            this.name = name;
            Error.captureStackTrace(this, error);
        }
        error.prototype = Object.create(Error.prototype);
        error.prototype.constructor = error;
        return error;
    }
}

module.exports = errorUtils;
