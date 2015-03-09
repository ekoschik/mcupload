#!/usr/bin/env node

var express = require('express')
  , bodyParser = require('body-parser')
  , multer = require('multer')
  , path = require('path')
  , fs = require('fs')
  , md5 = require('md5')
  , models = require('./models')
  , app = express()
  ;


var PORT = process.env.PORT || 3000;

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');

app.use(function(req, res, next) {
    console.log(req.headers);
    console.log(req.method + ' ' + req.url);
    next();
});

app.use(bodyParser.json({ limit: '2mb' }));
app.use(bodyParser.urlencoded({ extended: false }));
app.use(multer({ dest: './uploads', inMemory: true }));
app.use(express.static(path.join(__dirname, 'public')));

function isDirectory(path) {
    return fs.lstatSync(path).isDirectory();
}

app.use(bodyParser.json({ limit: '2mb' }));
app.use(bodyParser.urlencoded({ extended: false }));
app.use(multer({ dest: './uploads', inMemory: true }));
app.use(express.static(path.join(__dirname, 'public')));

function loadRoutes(base) {
    return (function loadRoutesHelper(dir) {
        fs.readdirSync(dir).forEach(function(file) {
            if (file[0] === '.') return; // skip hidden files/directories

            routePath = path.join(dir, file);
            if (isDirectory(routePath)) {
                loadRoutesHelper(routePath);
            } else {
                // remove the extension, remove the base directory name
                var routeName = routePath.substr(0, routePath.indexOf('.')).substr(base.length);

                var router = express.Router();
                require('./' + routePath)(router, models);

                console.log('using route ' + routeName);
                app.use(routeName, router);
            }
        });
    })(base);
}

loadRoutes('routes');

// development error handler
// will print stacktrace
if (app.get('env') === 'development') {
  app.use(function(err, req, res, next) {
    res.status(err.status || 500);
    res.render('error', {
      message: err.message,
      error: err
    });
  });
}

// production error handler
// no stacktraces leaked to user
app.use(function(err, req, res, next) {
  res.status(err.status || 500);
  res.render('error', {
    message: err.message,
    error: {}
  });
});

// sync the database and then start the server
models.sequelize.sync().then(function() {
    app.listen(PORT, function() {
        console.log('Listening on port', PORT);
    });
});
