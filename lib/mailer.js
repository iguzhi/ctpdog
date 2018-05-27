const logger = require('./logger').getLogger('email');
const nodemailer = require('nodemailer');

function Mailer(smtp) {
  this.smtp = smtp;
	this.createTransport();
}

(function() {

  /**
   * create reusable transporter object using the SMTP transport
   * @param {Object} smtp 
   * @param {String} smtp.host
   * @param {Integer} stmp.port
   * @param {Boolean} stmp.secure
   * @param {Object} stmp.auth
   * @param {String} stmp.auth.user
   * @param {String} stmp.auth.pass
   * 
   * 例如:
   * {
   *   host: 'smtp.mxhichina.com',
   *   port: 465,
   *   secure: true,
   *   auth: {
   *     user: '****@iguzhi.cn',
   *     pass: '****'
   *   }
   * }
   */
  this.createTransport = function() {
    this.transporter = nodemailer.createTransport(this.smtp);
  };

  /**
   * 发送邮件, text和html只能选其一
   * @param {Object} options
   * @param {String} options.from 发送者(例如: '"Fred Foo 👻" <foo@example.com>')
   * @param {String} options.to 接收者(例如: 'bar@example.com, baz@example.com')
   * @param {String} options.subject 邮件标题(例如: 'Hello ✔')
   * @param {String} options.text 邮件内容(plain text body)
   * @param {String} options.html 邮件内容(html body)
   * @param {Function} callback 
   */
  this.sendMail = function(options={}, callback) {
    if (!options.from || !options.to) {
      return;
    }

    this.transporter.sendMail(options, function(error, info) {
      if (error) {
        logger.error(error);
        return;
      }
      callback && callback();
      logger.info('Message %s sent: %s', info.messageId, info.response);
    });
  };

}).call(Mailer.prototype);

module.exports = Mailer;

