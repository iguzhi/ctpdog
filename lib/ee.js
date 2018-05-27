// 用于取代observer
const logger = require('./logger').getLogger('app');
const EventEmitter = require('events');

var ee = new EventEmitter();

ee.on('error', function(err) {
  logger.error('app error: %j', err);
});

module.exports = ee;