#!/usr/bin/env node

var express = require('express')
  , bodyParser = require('body-parser')
  , multer = require('multer')
  , path = require('path')
  , models = require('./models')
  , app = express()
  ;


var PORT = process.env.PORT || 3000;

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(multer({ dest: './uploads' }));
app.use(express.static(path.join(__dirname, 'public')));

app.post('/upload', function(req, res) {
    console.log(req.body);
    console.log(req.files);
    console.log('file name:', req.files.file.name);
    console.log('file path:', req.files.file.path);

    var imageRec = {
        user: req.body.user,
        url: req.files.file.path
    };

    console.log('Adding new image to database:', imageRec);
    models.Image.create(imageRec).then(function() {
        res.sendStatus(204);
    });
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
