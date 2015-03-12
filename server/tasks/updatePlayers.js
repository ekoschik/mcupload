'use strict';

var Promise    = require('bluebird')
  , _          = require('lodash')
  , fs         = Promise.promisifyAll(require('fs'))
  , path       = require('path')
  , config     = require('config')
  , mojang     = require('../lib/mojang')
  , NBTMonitor = require('../lib/nbtmonitor')
  ;

function removeExtension(path) {
    var dotIndex = path.lastIndexOf('.');
    if (dotIndex === -1) return path;
    return path.substr(0, dotIndex);
}

function getPlayerFromFilename(fname) {
    return removeExtension(path.basename(fname));
}

// TODO: this needs to be set up to update new players when they are added,
// otherwise trying to insert the locations will fail
module.exports = function(models) {
    // if the server hasn't been configured with a minecraft directory,
    // then don't bother to start watching for player updates
    var minecraftDir = config.get('minecraftDir');
    if (!minecraftDir) {
        return;
    }

    var playerDir = path.join(minecraftDir, 'world', 'playerdata');

    // start by checking each playerdata file agains the database
    var playerPromises = fs.readdirAsync(playerDir).map(removeExtension).map(function(uuid) {

        console.log('checking if player ' + uuid + ' already exists in the database');

        return models.Player.find({ where: { uuid: uuid }, attributes: ['uuid'] }).then(function(player) {
            console.log('got player ' + player);
            if (player !== null) return; // we already have the player stored
            return mojang.getUsernameFromUUID(uuid);
        }).then(function(username) {
            if (username === undefined) return;
            console.log('saving player uuid=' + uuid + ', name=' + username);
            return models.Player.create({
                uuid: uuid,
                name: username
            });
        }).catch(function(e) {
            console.log(e);
        });
    });
    Promise.settle(playerPromises).then(function(results) {
        console.log('finished adding players to the database!');
    
        // start the monitor, saving the player position every time it updates
    
        var monitor = new NBTMonitor(path.join(config.minecraftDir, 'world', 'playerdata'));
        var previousPositionMap = {};
        monitor.on('update', function(data) {
            if (data.data.Pos === undefined) return;

            //console.log(data);

            var position = data.data.Pos;
            var previousPosition = previousPositionMap[data.file];

            // return if the player hasn't moved!
            if (_.isEqual(position, previousPosition)) return;

            // save the position!
            models.PlayerLocation.create({
                player:    getPlayerFromFilename(data.file),
                x:         position[0],
                y:         position[1],
                z:         position[2],
                dimension: data.data.Dimension
            });

            previousPositionMap[data.file] = position;
        });
    }).catch(function(e) {
        console.log(e);
    });
};

if (require.main === module) {
    module.exports(require('../models'));
}
