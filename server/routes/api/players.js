
module.exports = function(router, models) {

    router.get('/', function(req, res) {
        res.json({ 
            schlenkster: { online: false }
          , ekoschik   : { online: true  }
        });
    });

    router.get('/:playerId', function(req, res) {
        console.log('returning player for id ' + req.params.playerId);
        res.json({
            online: false
        });
    });

    router.get('/:playerId/location', function(req, res) {
        res.json({
            x: 105
          , y: 308
          , z: 60
          , dimension: 'overworld'
        });
    });
};
