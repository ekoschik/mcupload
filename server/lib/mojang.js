'use strict';

/**
 * Api for getting a minecraft user name from the uuid or vise versa
 */

var Promise     = require('bluebird')
  , request     = Promise.promisify(require('request'))
  , util        = require('util')
  , MojangError = require('./errorutils').createError('MojangError')
  ;


var UUID_ENDPOINT =  'https://api.mojang.com/users/profiles/minecraft/%s';
var NAME_ENDPOINT = 'https://api.mojang.com/user/profiles/%s/names';

function cleanUUID(uuid) {
    return uuid.replace(/-/g, '');
}

function dashUUID(uuid) {
    if (/-/.test(uuid)) return uuid;
    return uuid.substr(0,  8) + '-' + 
           uuid.substr(8,  4) + '-' + 
           uuid.substr(12, 4) + '-' + 
           uuid.substr(16, 4) + '-' + 
           uuid.substr(20);
}

var mojang = {
    getUsernameFromUUID: function(uuid) {
        return request(util.format(NAME_ENDPOINT, cleanUUID(uuid))).spread(function(res, body) {
            if (res.statusCode === 204 || !body) throw new MojangError('The uuid ' + uuid + ' was not found');
            var data = JSON.parse(body);
            return data[0].name;
        });
    },

    getUUIDFromUsername: function(username) {
        return request(util.format(UUID_ENDPOINT, username)).spread(function(res, body) {
            if (res.statusCode === 204 || !body) throw new MojangError('The user "' + username + '" was not found');
            var data = JSON.parse(body);
            return dashUUID(data.id);
        });
    },

    MojangError: MojangError
}

module.exports = mojang;

if (require.main === module) {
    if (process.argv[2] === undefined) {
        console.log('usage: ' + process.argv[0] + ' ' + process.argv[1] + ' <username>');
        process.exit(1);
    }

    mojang.getUUIDFromUsername(process.argv[2]).then(function(uuid) {
        console.log('got ' + uuid + ' for user ' + process.argv[2]);
    }).catch(function(e) {
        console.log(e);
    });
}
