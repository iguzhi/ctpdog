const object = require('iguzhi/object');
const helper = require('./helper');
// maxLogSize是以byte为单位计算的
const log4js = require('log4js');
const loggerCfg = require('../config/logger');

helper.ensureDirExists('./logs');

var configure = {
  appenders: [
    { type: 'console' }
  ],
  replaceConsole: true
};

object.forEach(loggerCfg.level, function(level, category) {
  configure.appenders.push({ type: 'file', filename: './logs/' + category + '.log', category: category,  backups: 5, maxLogSize: 2048000 });
});

log4js.configure(configure);

var logger = {};

configure.appenders.forEach(function(appender) {
  var category = appender.category;
  logger[category] = log4js.getLogger(category);
  logger[category].setLevel(loggerCfg.level[category] || loggerCfg.defaultLevel);
});

module.exports = logger;
