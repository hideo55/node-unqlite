# unqlite

Node.js binding for [UnQLite](http://unqlite.org/).
This mdoule provides simple Key/Value store functions of UnQLite.

 [![Build Status](https://secure.travis-ci.org/hideo55/node-unqlite.png)](http://travis-ci.org/hideo55/node-unqlite)

## Usage

```JavaScript
var unqlite = require('unqlite');

var db = new unqlite.Database('/path/to/db');
db.open(unqlite.OPEN_CREATE, function(err){
  if(err) throw err;
  db.store('key', 'value', function(err, key, value){
    ...
  });
});
```

## API

### new Database(filename)

Create new instance.

- `filename` is file-path of UnQLite database.

### open([mode,] callback)

Opening a new database handle.

- `mode` is database access mode. See <a href="#constants"><code><b>Constants</b></code></a> section.
- `callback` will called with a sigle argument `error`. 

### close(callback)

Closing a database handle.

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

- OPEN_CREATE

- OPEN_READWRITE

- OPEN_READONLY

- OPEN_MMAP

- OPEN_EXCLUSIVE

- OPEN_TEMP_DB

- OPEN_IN_MEMORY

- OPEN_OMIT_JOURNALING

- OPEN_NOMUTEX

## Unsupported functions

- Document Store (JSON via Jx9)
- Manual Transaction Manager

## Version of UnQLite

1.1.6

## Requirement

Node v0.8 or later

## Author

Hideaki Ohno  &lt;hide.o.j55{at}gmail.com&gt;

## License 

(The MIT License)

Copyright (c) 2013 Hideaki Ohno &lt;hide.o.j55{at}gmail.com&gt;

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

