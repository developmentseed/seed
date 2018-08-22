var assert = require('assert')
var path = require('path')
var exit = require('exit')

var copy = require('../lib/copy-file')

module.exports = {
  name: 'readme',
  command: function (args) {
    var templatesDir = path.join(__dirname, '..', 'templates')
    var source = path.join(templatesDir, 'README.md')
    var target = path.join(process.cwd(), 'README.md')

    assert.ok(args.package, 'args.package is required')
    assert.ok(args.github, 'args.github is required')
    assert.ok(args.email, 'args.email is required')

    copy(source, target, args, function () {
      exit(0)
    })
  },
  options: [
    {
      name: 'package',
      boolean: false,
      abbr: 'p'
    },
    {
      name: 'github',
      boolean: false,
      abbr: 'g'
    },
    {
      name: 'email',
      boolean: false,
      abbr: 'e'
    },
    {
      name: 'twitter',
      boolean: false,
      abbr: 't'
    },
    {
      name: 'chat',
      boolean: false,
      abbr: 'c'
    },
    {
      name: 'newsletter',
      boolean: false,
      abbr: 'n'
    }
  ]
}
