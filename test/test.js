var unqlite = require('../lib/unqlite');
var DB = unqlite.Database;

var assert = require('assert');

describe('open', function() {
  var temp = require('temp');
  var path = require('path');

  var dbFile;

  beforeEach(function(cb) {
    temp.mkdir(null, function(err, tempPath) {
      if (err) {
        cb(err);
      }
      dbFile = path.join(tempPath, 'test_open.db');
      cb();
    });
  });

  it('with callback API', function(done) {
    var uql = new DB(dbFile, function(err) {
      assert.equal(err, null);
      done();
    });
  });

  it('with EventEmitter API', function(done) {
    var uql = new DB(dbFile);
    uql.on('open', function(err) {
      assert.equal(err, null);
      done();
    });
  });

  describe('mode', function() {
    it('with READONLY', function(done) {
      var uql = new DB(dbFile, unqlite.OPEN_READONLY);
      uql.on('open', function() {
        assert.equal(uql.mode, unqlite.OPEN_READONLY);
        done();
      });
    });

    it('with READWRITE', function(done) {
      var uql = new DB(dbFile, unqlite.OPEN_READWRITE);
      uql.on('open', function() {
        assert.equal(uql.mode, unqlite.OPEN_READWRITE);
        done();
      });
    });

    it('with MMAP', function(done) {
      var uql = new DB(dbFile, unqlite.OPEN_MMAP);
      uql.on('open', function() {
        assert.equal(uql.mode, unqlite.OPEN_MMAP);
        done();
      });
    });

    it('with IN_MEMORY', function(done) {
      var uql = new DB(dbFile, unqlite.OPEN_IN_MEMORY);
      uql.on('open', function() {
        assert.equal(uql.mode, unqlite.OPEN_IN_MEMORY);
        done();
      });
    });
  });
});

describe('Key/Value API', function() {
  it('store API', function(done) {
    var uql = new DB('test/test.db', unqlite.OPEN_IN_MEMORY);
    uql.on('open', function() {
      uql.store('foo', 'bar');
      uql.on('store', function(key, value) {
        assert.equal(key, 'foo');
        assert.equal(value, 'bar');
        done();
      });
    });
  });
  it('fetch API', function(done) {
    var uql = new DB('test/test.db', unqlite.OPEN_IN_MEMORY);
    uql.on('open', function() {
      uql.store('foo', 'bar', function(err, key, value) {
        uql.fetch(key, function(err, key, value) {
          assert.equal(value, 'bar');
          done();
        });
      });
    });
  });
  it('append API', function(done) {
    var uql = new DB('test/test.db', unqlite.OPEN_IN_MEMORY);
    uql.on('open', function() {
      uql.store('foo', 'bar', function() {
        uql.append('foo', 'baz', function(err, key, value) {
          uql.fetch(key, function(err, key, value) {
            assert.equal(value, 'barbaz');
            done();
          });
        });
      });
    });
  });
  it('delete API', function(done) {
    var uql = new DB('test/test.db', unqlite.OPEN_IN_MEMORY);
    uql.on('open', function() {
      uql.store('foo', 'bar', function() {
        uql.
        delete ('foo',
        function(err, key) {
          uql.fetch(key, function(err, key, value) {
            assert(err, null);
            assert.equal(value, '');
            done();
          });
        });
      });
    });
  });
});
