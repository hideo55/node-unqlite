# unqlite

Node.js binding for [UnQLite](http://unqlite.org/).
This mdoule provides simple Key/Value store functions of UnQLite.

 [![Build Status](https://secure.travis-ci.org/hideo55/node-unqlite.png)](http://travis-ci.org/hideo55/node-unqlite)

## Usage

```JavaScript
var unqlite = require('unqlite');

var db = unqlite.Database('/path/to/db');
db.open(unqlite.OPEN_CREATE, function(err){
  if(err) throw err;
  db.store('key', 'value', function(err, key, value){
    ...
  });
});
```

## API

### open([mode,] callback)

- `mode` is database access mode. See <a href="#constants"><code><b>Constants</b></code></a> section.
- The callback gets sigle argument `error`. 

### close(callback)

- The callback gets sigle argument `error`. 

### store(key, value, callback)

- The callback gets three arguments `(error, key, value)`. 

### append(key, value, callback)

- The callback gets three arguments `(error, key, value)`. 

### delete(key, callback)

- The callback gets two arguments `(error, key)`. 

### fetch(key, callback)

- The callback gets three arguments `(error, key, value)`. 

<a name="constants"></a>
## Constants

See also [http://unqlite.org/c_api/unqlite_open.html](http://unqlite.org/c_api/unqlite_open.html)

### OPEN_CREATE

### OPEN_READWRITE

### OPEN_READONLY

### OPEN_MMAP

### OPEN_EXCLUSIVE

### OPEN_TEMP_DB

### OPEN_IN_MEMORY

### OPEN_OMIT_JOURNALING

### OPEN_NOMUTEX

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

Copyright (c) 2012 Hideaki Ohno &lt;hide.o.j55{at}gmail.com&gt;

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

