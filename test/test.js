var unqlite = require('../lib/unqlite');
var DB = unqlite.Database;

var assert = require('assert');

describe('open', function() {
  it('with callback API', function(done) {
    var uql = new DB('test_open1.db', function(err) {
      assert.equal(err, null);
      done();
    });
  });

  it('with EventEmitter API', function(done) {
    var uql = new DB('test/test_open2.db');
    uql.on('open', function(err) {
      assert.equal(err, null);
      done();
    });
  });

  describe('mode', function() {
    it('with READONLY', function(done) {
      var uql = new DB('test2_open3.db', unqlite.OPEN_READONLY);
      uql.on('open', function() {
        assert.equal(uql.mode, unqlite.OPEN_READONLY);
        done();
      });
    });

    it('with READWRITE', function(done) {
      var uql = new DB('test_open4.db', unqlite.OPEN_READWRITE);
      uql.on('open', function() {
        assert.equal(uql.mode, unqlite.OPEN_READWRITE);
        done();
      });
    });

    it('with MMAP', function(done) {
      var uql = new DB('test_open5.db', unqlite.OPEN_MMAP);
      uql.on('open', function() {
        assert.equal(uql.mode, unqlite.OPEN_MMAP);
        done();
      });
    });

    it('with IN_MEMORY', function(done) {
      var uql = new DB('test_open5.db', unqlite.OPEN_IN_MEMORY);
      uql.on('open', function() {
        assert.equal(uql.mode, unqlite.OPEN_IN_MEMORY);
        done();
      });
    });
  });
});

describe('Key/Value API', function() {
  it('store API', function(done) {
    var uql = new DB('test/test_set1.db', unqlite.OPEN_IN_MEMORY);
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
    var uql = new DB('test/test_set1.db', unqlite.OPEN_IN_MEMORY);
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
    var uql = new DB('test/test_set1.db', unqlite.OPEN_IN_MEMORY);
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
});
