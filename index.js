#! /usr/bin/env node

var path = require('path')
var subcommand = require('subcommand')

var defaults = [];

defaults.push({
  name: 'package',
  default: path.parse(process.cwd()).base
})

var match = subcommand({
  defaults: defaults,
  commands: [
    require('./commands/readme')
  ]
})

match(process.argv.slice(2))