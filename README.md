# unqlite

Node.js binding for [UnQLite](http://unqlite.org/).
This module provides simple Key/Value store functions of UnQLite.

[![Build Status](https://travis-ci.org/hideo55/node-unqlite.svg?branch=master)](http://travis-ci.org/hideo55/node-unqlite)
[![npm](https://img.shields.io/npm/v/unqlite.svg)](https://www.npmjs.com/package/unqlite)

## Installation

    $ npm install unqlite
    
or

    $ git clone https://github.com/hideo55/node-unqlite.git
    $ npm install ./node-unqlite

## Usage

```JavaScript
var unqlite = require('unqlite');

var db = new unqlite.Database('/path/to/db');
db.open(unqlite.OPEN_CREATE, function(err){
  if(err) throw err;
  db.store('example key', 'example value', function(err, key, value){
    db.close(function(err){// commit change to the database
        ...
    });
  });
});
```

## API

### new Database(filename)

Create new instance.

- `filename` is file-path of UnQLite database.

### open([mode,] callback)

Opening a new database handle and start transaction.

- `mode` is database access mode. See <a href="#constants"><code><b>Constants</b></code></a> section.
  - If the `mode` parameter is omitted, the database is opened in `OPEN_CREATE` mode.
- `callback` will called with a sigle argument `error`. 

### close(callback)

Closing a database handle and commit all changes to the database.

- `callback` will called with a sigle argument `error`. 

### store(key, value, callback)

Store records in the database.

- `key` is record key.
- `value` is record data.
- `callback` will called with a three arguments `(error, key, value)`. 

### append(key, value, callback)

Append data to a database record.

- `key` is record key.
- `value` is record data.
- `callback` will called with a three arguments `(error, key, value)`. 

### fetch(key, callback)

Fetch a record from the database.

- `key` is record key.
- `callback` will called with a three arguments `(error, key, value)`. 

### delete(key, callback)

Remove a record from the database.

- `key` is record key.
- `callback` will called with a two arguments `(error, key)`. 

<a name="constants"></a>
## Constants

See also [http://unqlite.org/c_api/unqlite_open.html](http://unqlite.org/c_api/unqlite_open.html)

- **OPEN_CREATE**
  -  If the database does not exists, it is created. Otherwise, it is opened with read+write privileges.
- **OPEN_READWRITE**
  - Open the database with read+write privileges.
- **OPEN_READONLY**
  - Open the database in a read-only mode.
- **OPEN_MMAP**
  - Obtain a read-only memory view of the whole database.
- **OPEN_EXCLUSIVE**
  - Creates a new file, only if it does not already exist. If the file exists, it fails.
- **OPEN_TEMP**
  - A private, temporary on-disk database will be created. This private database will be automatically deleted as soon as the database connection is closed.
- **OPEN_IN_MEMORY**
  - A private, in-memory database will be created. The in-memory database will vanish when the database connection is closed.
- **OPEN_OMIT_JOURNALING**
  - (Not recommended) Disable journaling for this database. In other words, you will not be able to rollback your database after a crash or power failure. This flag is automatically set for temporary database.
- **OPEN_NOMUTEX**
  - (Not recommended) Disable the private recursive mutex associated with each database handle. When set, you should not share this handle between multiple threads. Otherwise, the result is undefined.


## Unsupported features

- Cursor
- Document Store (JSON via Jx9)
- Manual Transaction Manager

## Version of UnQLite

1.1.7

## Requirement

Node v0.12 or later

## Author

Hideaki Ohno  &lt;hide.o.j55{at}gmail.com&gt;

## License 

(The MIT License)

Copyright (c) 2013-2017 Hideaki Ohno &lt;hide.o.j55{at}gmail.com&gt;

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

