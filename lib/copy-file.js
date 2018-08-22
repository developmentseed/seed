var fs = require('fs')
var maxstache = require('maxstache-stream')

module.exports = function (source, target, data, cb) {
  var stream = fs.createReadStream(source)
  stream.pipe(maxstache(data)).pipe(fs.createWriteStream(target))
  stream.on('end', cb)
}
