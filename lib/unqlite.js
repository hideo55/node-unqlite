var unqlite = module.exports = exports = require('./node_unqlite');
var EventEmitter = require('events').EventEmitter;

function inherits(target, source) {
  for (var k in source.prototype)
    target.prototype[k] = source.prototype[k];
}

var Database = unqlite.Database;
inherits(Database, EventEmitter);
