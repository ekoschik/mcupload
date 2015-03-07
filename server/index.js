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

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(multer({ dest: './uploads', inMemory: true }));
app.use(express.static(path.join(__dirname, 'public')));

var routes = fs.readdirSync('./routes')
console.log(routes);
routes = routes.filter(function(s) { return s[0] !== '.'; });
console.log(routes);
routes.forEach(function(route) {
    var routeName = '/' + route.substr(0, route.indexOf('.'));
    console.log('using route ' + routeName);
    var router = express.Router();
    require(path.resolve('routes', route))(router);
    app.use(routeName, router);
});

app.get('/', function(req, res) {
    res.render('upload_form.jade', { title: 'Upload File' });
});

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
