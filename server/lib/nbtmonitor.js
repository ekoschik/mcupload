#!/usr/bin/env node

var Promise = require('bluebird')
  , fs = Promise.promisifyAll(require('fs'))
  , nbt = Promise.promisifyAll(require('nbt'))
  , jdp = require('jsondiffpatch')
  , path = require('path')
  , events = require('events')
  , util = require('util')
  ;

/** @const */
var EXTENSIONS = ['dat', 'mca'];

/**
 * Parse the NBT file {@code fname}
 * @param {string} fname The name of the NBT file to parse.
 * @return {Promise} A promise representing the contents of the NBT file.
 */
function parseNBT(fname) {
    return fs.readFileAsync(fname).then(function(data) {
        return nbt.parseAsync(data);
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

    console.log('new nbtfile ' + fname);

    parseNBT(self.fname).then(function(data) {
        self.data = data;
        self.emitter.emit('load', {
            file: self.fname,
            data: data
        });
    }).catch(function(e) {
        console.log('Failed to parse NBT file "' + self.fname + '": ' + e);
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

    parseNBT(self.fname).then(function(data) {
        var delta = jdp.diff(self.data, data);
        self.emitter.emit('update-delta', {
            file: self.fname,
            data: delta
        });
        self.emitter.emit('update', {
            file: self.fname,
            data: data
        });
        self.data = data;
    }).catch(function(e) {
        console.log('Failed to parse NBT file "' + self.fname + '": ' + e);
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
    fs.watch(dir, function(event, fname) {
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
    fs.readdirAsync(self.dir).then(function(files) {
        console.log("Found files in" + self.dir + ": " + files.join(', '));
        files.forEach(function(fname) {
            self.newFile(path.join(self.dir, fname));
        });
    }).catch(function(e) {
        console.log('Failed to read the specified watch directory "' + self.dir + '": ' + e);
    });
};

/**
 * Start monitoring the file given by {@code fname}.
 * @param  {String} fname The absolute path of the file to watch.
 */
NBTMonitor.prototype.newFile = function(fname) {
    var self = this;
    if (!(fname in self.files)) {
        self.files[fname] = new NBTFile(fname, this);
    }
};

/**
 * Tells the file given by {@code fname} to reload its file.
 * If the file does not yet exist in our map, call newFile first.
 * @param  {String} fname The name of the file to update.
 * @return {[type]}
 */
NBTMonitor.prototype.updateFile = function(fname) {
    var self = this;
    if (fname in self.files) {
        self.files[fname].update();
    } else {
        self.newFile(fname);
    }
};

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@  Utilities  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/**
 * Checks to see if the path {@code fname} ends with an extension
 * we want to watch.
 * TODO: This function is kind of a hack. Figure out a better way
 * @param  {string} fname The path to check.
 * @return {boolean} Whether we should watch this file or not.
 */
function validFormat(fname) {
    var ext = fname.split('.').slice(-1);
    return EXTENSIONS.some(function(val) {
        return val == ext; // triple equals doesn't seem to work here
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

