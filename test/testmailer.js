const { Mailer, logger } = require('../index');

// logger.setLevel('ERROR');

let smtp = {
    host: 'smtp.mxhichina.com',
    port: 465,
    secure: true,
    auth: {
        user: 'admin@iguzhi.cn',
        pass: '123456'
    }
};

let mailer = new Mailer(smtp);

let options = {
    from: smtp.auth.user,
    to: '383523223@qq.com',
    subject: 'nodectp 3',
    html: '<html><body><strong>this is 3 test mail sended from nodectp.</strong></body></html>'
}

mailer.sendMail(options, function() {
    console.log('send mail success!')
})