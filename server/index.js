#!/usr/bin/env node

var express    = require('express')
  , bodyParser = require('body-parser')
  , multer     = require('multer')
  , path       = require('path')
  , http       = require('http')
  , fs         = require('fs')
  , config     = require('config')
  , models     = require('./models')
  , app        = express()
  , server     = http.createServer(app)
  , io         = require('socket.io')(server)
  , JSX        = require('node-jsx').install({ extension: '.jsx' })
  ;


app.disable('x-powered-by');
if (app.get('env') === 'development') {
    app.set('json spaces', 4);
}
// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');

app.use(function(req, res, next) {
    console.log(req.headers);
    console.log(req.method + ' ' + req.url);
    next();
});

app.use(bodyParser.json({ limit: config.get('jsonSizeLimit') }));
app.use(bodyParser.urlencoded({ extended: false }));
app.use(multer({ dest: config.get('uploadDir'), inMemory: true }));
app.use(express.static(path.join(__dirname, 'public')));

function isDirectory(path) {
    return fs.lstatSync(path).isDirectory();
}

// TODO: this function is really ugly, make it more elegant
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
				routeName = routeName.replace(/\\/g, '/')
                if (routeName === '/index') {
                    routeName = '/';
                }

                var socket = io.of(routeName);

                var router = express.Router();
                require(path.join(__dirname, routePath))(router, models, socket);

                console.log('using route ' + routeName);
                // replace backslash with forward slash, fix for windows
                app.use(routeName, router);
            }
        });
    })(base);
}

function loadTasks(taskDir) {
    fs.readdirSync(taskDir).forEach(function(task) {
        console.log('loading task ' + task);
        require(path.join(__dirname, taskDir, task))(models);
    });
}

loadRoutes('routes');
loadTasks('tasks');

// development error handler
// will print stacktrace
if (app.get('env') === 'development') {
  app.use(function(err, req, res, next) {
    console.log(err);
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
  console.log(err);
  res.status(err.status || 500);
  res.render('error', {
    message: err.message,
    error: {}
  });
});

// sync the database and then start the server
models.sequelize.sync().then(function() {
    var port = config.get('port');
    var address = config.get('ipAddress');
    server.listen(port, address, function() {
        console.log('Listening on port', port);
    });
});
