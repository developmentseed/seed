#! /usr/bin/env node

var path = require('path')
var subcommand = require('subcommand')

var config = require('./lib/config')()
var profile = config.get('currentProfile') || {}

var defaults = Object.keys(profile).map(function (key) {
  return { name: key, default: profile[key] }
})

defaults.push({
  name: 'package',
  default: path.parse(process.cwd()).base
})

var match = subcommand({
  root: require('./commands/help'),
  defaults: defaults,
  commands: [
    require('./commands/readme')
  ]
})

match(process.argv.slice(2))