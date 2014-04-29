# unqlite

Node.js binding for [UnQLite](http://unqlite.org/).


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

### open([mode], callback)

### store(key, value, callback)

### append(key, value, callback)

### delete(key, callback)

### fetch(key, callback)

## UnQLite version

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

