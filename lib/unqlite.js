var unqlite = module.exports = exports = require('./node_unqlite');
var path = require('path');
var util = require('util');
var EventEmitter = require('events').EventEmitter;

var UnQLite = unqlite.UnQLite;

util.inherits(UnQLite, EventEmitter);

