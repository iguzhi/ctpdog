
const fd = require('./fd');
// maxLogSize是以byte为单位计算的
const log4js = require('log4js');

fd.ensureDirectory('./logs');

log4js.configure({
  appenders: [
    { type: 'console' },
    { type: 'file', filename: './logs/ctpapp.log', category: 'ctpapp',  backups: 5, maxLogSize: 2048000 },
    { type: 'file', filename: './logs/schedule.log', category: 'schedule',  backups: 5, maxLogSize: 2048000 },
    // { type: 'file', filename: './logs/notify.log', category: 'notify',  backups: 5, maxLogSize: 2048000 },
    { type: 'file', filename: './logs/email.log', category: 'email',  backups: 5, maxLogSize: 2048000 }
  ],
  replaceConsole: true
});

var logger = {
  ctpapp: log4js.getLogger('ctpapp'),
  email: log4js.getLogger('email'),
  // notify: log4js.getLogger('notify'),
  schedule: log4js.getLogger('schedule')
};

// logger.ctpapp.setLevel('ERROR');
// logger.schedule.setLevel('ERROR');

module.exports = logger;