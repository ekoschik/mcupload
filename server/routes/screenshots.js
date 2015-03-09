'use strict';

var config = require('config')
  , path = require('path')
  , uploadDir = config.get('uploadDir')
  ;

module.exports = function(router, models) {
    router.get('/', function(req, res) {
        // we want to render a page with links to all of the screenshots uploaded

        models.Image.findAll().then(function(images) {
            res.render('screenshots.jade', { title: 'Screenshots', 'images': images });
        }).error(function(e) {
            res.sendStatus(500);
        });
    });

    router.get('/:id', function(req, res) {
        var id = req.params.id;
        res.sendFile(path.join(uploadDir, id));
    })
}