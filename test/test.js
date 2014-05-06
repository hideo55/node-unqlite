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

  it('default', function(done) {
    var uql = new DB(dbFile);
    uql.open(function(err) {
      assert.equal(err, null);
      done();
    });
  });

  describe('mode', function() {
    it('with READONLY', function(done) {
      var uql = new DB(dbFile);
      uql.open(unqlite.OPEN_READONLY, function() {
        assert.equal(uql.mode, unqlite.OPEN_READONLY);
        done();
      });
    });

    it('with READWRITE', function(done) {
      var uql = new DB(dbFile);
      uql.open(unqlite.OPEN_READWRITE, function() {
        assert.equal(uql.mode, unqlite.OPEN_READWRITE);
        done();
      });
    });

    it('with MMAP', function(done) {
      var uql = new DB(dbFile);
      uql.open(unqlite.OPEN_MMAP, function() {
        assert.equal(uql.mode, unqlite.OPEN_MMAP);
        done();
      });
    });

    it('with IN_MEMORY', function(done) {
      var uql = new DB(dbFile);
      uql.open(unqlite.OPEN_IN_MEMORY, function() {
        assert.equal(uql.mode, unqlite.OPEN_IN_MEMORY);
        done();
      });
    });
  });

  it('close', function(done) {
    var uql = new DB(dbFile);
    uql.open(function() {
      uql.close(function(err) {
        assert.equal(err, null);
        done();
      });
    });
  });
});

describe('Key/Value API', function() {
  it('store API', function(done) {
    var uql = new DB('test/test.db');
    uql.open(unqlite.OPEN_IN_MEMORY, function() {
      uql.store('foo', 'bar', function(err, key, value) {
        assert.equal(key, 'foo');
        assert.equal(value, 'bar');
        done();
      });
    });
  });

  it('fetch API', function(done) {
    var uql = new DB('test/test.db');
    uql.open(unqlite.OPEN_IN_MEMORY, function() {
      uql.store('foo', 'bar', function(err, key, value) {
        uql.fetch(key, function(err, key, value) {
          assert.equal(value, 'bar');
          done();
        });
      });
    });
  });

  it('append API', function(done) {
    var uql = new DB('test/test.db');
    uql.open(unqlite.OPEN_IN_MEMORY, function() {
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
    var uql = new DB('test/test.db');
    uql.open(unqlite.OPEN_IN_MEMORY, function() {
      uql.store('foo', 'bar', function(err, key, value) {
        uql.
        delete ('foo',
        function(err, key) {
          uql.fetch(key, function(err, key, value) {
            assert.notEqual(err, null);
            assert.ok(err.message.match(/^Failed to fetch/));
            done();
          });
        });
      });
    });
  });
});

describe('exceptions', function() {
  describe('new', function() {
    it('argument missing', function(done) {
      try {
        var uql = new DB();
        assert.fail();
      } catch(e) {
        assert.ok( e instanceof RangeError);
        assert.ok(e.message.match(/A least 1 arguments are required/));
        done();
      }
    });
    it('argument is not string', function(done) {
      try {
        var uql = new DB(1);
        assert.fail();
      } catch(e) {
        assert.ok( e instanceof TypeError);
        assert.ok(e.message.match(/Argument 1 must be a String/));
        done();
      }
    });
  });
  describe('open', function() {
    it('callback missing', function(done) {
      try {
        var uql = new DB('');
        uql.open();
        assert.fail('Must be fail');
      } catch(e) {
        assert.ok( e instanceof RangeError);
        assert.ok(e.message.match(/A least 1 arguments are required/));
        done();
      };
    });
    
    it('callback is not function', function(done) {
      try {
        var uql = new DB('');
        uql.open(0, 0);
        assert.fail('Must be fail');
      } catch(e) {
        assert.ok( e instanceof TypeError);
        assert.ok(e.message.match(/Argument 2 must be a Function/));
        done();
      };
    });
    
  });
});
