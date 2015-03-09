#!/usr/bin/env node

var fs = require('fs')
  , nbt = require('nbt')
  , jdp = require('jsondiffpatch')
  , path = require('path')
  , events = require('events')
  , util = require('util')
  ;

/** @const */
var EXTENSIONS = ['dat', 'mca']

/**
 * Parse the NBT file {@code fname}
 * @param {string} fname The name of the NBT file to parse.
 * @param {function(exception, object)} callback
 */
function parseNBT(fname, callback) {
    fs.readFile(fname, function(err, data) {
        if (err) {
            callback(err);
            return;
        }
        nbt.parse(data, function(err, result) {
            callback(err, result);
        });
    });
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@  NBTFile  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/**
 * NBTFile Class
 * @constructor
 * @param {string} fname The absolute path to the NBT file
 *      this class will represent.
 */
function NBTFile(fname, emitter) {
    var self = this;
    self.fname = fname;
    self.emitter = emitter;
    self.lastUpdated = new Date().getTime();
    self.cleanName = path.basename(self.fname.substr(0, self.fname.length - path.extname(self.fname).length));

    console.log('new nbtfile ' + fname);

    parseNBT(self.fname, function(err, data) {
        if (err) throw err;
        self.data = data;
        console.log(data);
        self.emitter.emit('load', {
            file: self.cleanName,
            data: data
        });
    });
}

/**
 * Update this NBTFile with new data. Emits a diff
 * of the previous and new data. 
 */
NBTFile.prototype.update = function() {
    // get diff of data
    var self = this;
    var now = new Date().getTime();
    console.log(self.fname, "updated after", now - self.lastUpdated, "milliseconds");
    self.lastUpdated = now;

    parseNBT(self.fname, function(err, data) {
        var delta = jdp.diff(self.data, data);
        console.log(delta);
        self.emitter.emit('delta', {
            file: self.cleanName,
            data: delta
        });
        self.emitter.emit('update', {
            file: self.cleanName,
            data: data
        });
        self.data = data;
    });
};

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@  NBTMonitor  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/**
 * NBTMonitor Class
 * starts monitoring the directory {@code dir} for changes
 * @param {String} dir the directory to watch
 */
function NBTMonitor(dir) {
    events.EventEmitter.call(this);
    var self = this;
    self.dir = dir;
    self.files = {};

    self.initializeFiles();

    // set up a watch on the directory
    fs.watch(dir, function (event, fname) {
        console.log('event: ' + event + ', file: ' + fname);
        if (!validFormat(fname)) return;

        if (fname) {
            self.updateFile(path.join(dir, fname));
        } else {
            console.log('filename not provided');
        }
    });
}

util.inherits(NBTMonitor, events.EventEmitter);

/**
 * Start monitoring all the files in the folder this
 * monitor was initialized with.
 */
NBTMonitor.prototype.initializeFiles = function() {
    var self = this;
    fs.readdir(self.dir, function(err, files) {
        if (err) throw err;

        console.log("Found files in" + self.dir + ": " + files);
        files.forEach(function(fname) {
            self.newFile(path.join(self.dir, fname));
        });
    });
}

/**
 * Start monitoring the file given by {@code fname}.
 * @param  {String} fname The absolute path of the file to watch.
 */
NBTMonitor.prototype.newFile = function(fname) {
    var scope = this;
    if (!(fname in scope.files)) {
        scope.files[fname] = new NBTFile(fname, this);
    }
};

/**
 * Tells the file given by {@code fname} to reload its file.
 * If the file does not yet exist in our map, call newFile first.
 * @param  {String} fname The name of the file to update.
 * @return {[type]}
 */
NBTMonitor.prototype.updateFile = function(fname) {
    var scope = this;
    if (fname in scope.files) {
        scope.files[fname].update();
    } else {
        scope.newFile(fname);
    }
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@  Utilities  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/**
 * Checks to see if the path {@code fname} ends with an extension
 * we want to watch.
 * @param  {string} fname The path to check.
 * @return {boolean} Whether we should watch this file or not.
 */
function validFormat(fname) {
    var ext = fname.split('.').slice(-1);
    return EXTENSIONS.some(function(val) {
        return val == ext;
    });
}

/**
 * Starts an NBTMonitor watching the given directory.
 * @param {Array.<string>} argv The args passed into the program.
 */
function main(argv) {
    var dir = argv[2];
    if (!dir) {
        console.log("usage:", process.argv[0], path.basename(process.argv[1]), "<directory>");
        process.exit();
    }
    var monitor = new NBTMonitor(dir);
    monitor.on('delta', function(data) {
        console.log(data);
    });
    monitor.on('load', function(data) {
        console.log(data);
    });
    console.log("monitoring", dir);
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@  Include Guard  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

if (require.main === module) {
    main(process.argv);
}


/** Export our main functionality */
module.exports = NBTMonitor;

