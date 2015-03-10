'use strict';

var Promise    = require('bluebird')
  , fs         = Promise.promisifyAll(require('fs'))
  , path       = require('path')
  , config     = require('config')
  , mojang     = require('../lib/mojang')
  , NBTMonitor = require('../lib/nbtmonitor')
  ;

function removeExtension(path) {
    return path.substr(0, path.lastIndexOf('.'));
}

module.exports = function(models) {
    // if the server hasn't been configured with a minecraft directory,
    // then don't bother to start watching for player updates
    if (!config.has('minecraftDir')) {
        return;
    }

    var minecraftDir = config.get('minecraftDir');
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
        console.log('finished updating all players!');
        results.forEach(function(result) {
            console.log(JSON.stringify(result));
        })
    });
    
    // then start the monitor, saving the player position every time it updates
    
    /*
    var monitor = new NBTMonitor(path.join(config.minecraftDir, 'world', 'playerdata'));
    var previousPositionMap = {};
    monitor.on('data', function(data) {
        if (data.data.Pos === undefined) return;

        var position = data.data.Pos;
        var previousPosition = previousPositionMap[data.file];

        if (previousPosition !== undefined) {
            if (position[0] !== previousPosition[0] ||
                position[1] !== previousPosition[1] ||
                position[2] !== previousPosition[2])
            {
                // save the position!
                modes.PlayerLocation.create({
                    uuid:      removeExtension(data.file),
                    x:         position[0],
                    y:         position[1],
                    z:         position[2],
                    dimension: data.data.dimension
                });
            }
        }
        previousPositionMap[data.file] = position;
    });
    */
}

if (require.main === module) {
    module.exports(require('../models'));
}
