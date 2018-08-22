const path = require('path');
const Handlebars = require('handlebars');
const YAML = require('yamljs');
const fs = require('fs');

module.exports = {
  name: 'readme',
  command: function (args) {
    const templatesDir = path.join(__dirname, '..', 'templates')
    const source = fs.readFileSync(path.join(templatesDir, 'README.md'), 'utf-8')
    const target = path.join(process.cwd(), 'README.md')

    const configString = fs.readFileSync('config.yml', 'utf-8');
    const config = YAML.parse(configString);
    const template = Handlebars.compile(source);
    const result = template(config);
    fs.writeFileSync(target, result);
  }
}
