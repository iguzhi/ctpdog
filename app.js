const setting = require('./config/setting.json');
const logger = require('./lib/logger').ctpapp;
const mailer = require('./lib/mailer');

var Ctp = require('./lib/ctp');
var ctp = new Ctp();

ctp.init(setting);

var td = ctp.td;
// var md = ctp.md;

var account = ctp.getAccountByUserID(369863);
console.log("account:", account);

td.On("OnFrontConnected", function() {
  logger.info("login : %s", td.ReqUserLogin(account, ctp.nRequestID()));
  logger.info("OnFrontConnected")
});

td.On("OnFrontDisconnected", function(nReason) {
  logger.info("OnFrontDisconnected nReason: %s", nReason)
});


td.On("OnRspUserLogin", function(Login, Rsp, nRequestID, bIsLast) {
  logger.info("OnRspUserLogin : %j, %j, %s, %s", Login, Rsp, nRequestID, bIsLast);
  //logger.info("ReqQryTradingAccount=", tduser.ReqQryTradingAccount(q, (new Date()).valueOf()/1000));
  //sleep(2000);
  //logger.info("ReqQryInvestorPosition=", tduser.ReqQryInvestorPosition(q, (new Date()).valueOf()/1000));
  //logger.info("ReqQryTrade=", tduser.ReqQryTrade(q, (new Date()).valueOf()/1000));
  //logger.info("ReqQryOrder=", tduser.ReqQryOrder(q, (new Date()).valueOf()/1000));
  //logger.info("ReqQryProduct=", tduser.ReqQryProduct(q, (new Date()).valueOf()/1000));
  //logger.info("ReqQryInstrument=", tduser.ReqQryInstrument(q, (new Date()).valueOf()/1000));
  //logger.info('-----before ReqQryContractBank-----')
  // td.ReqQryContractBank({
  // 	BankID: '5',
		// BankBranchID: '0000',
		// BrokerID: '4040',
  // }, ctp.nRequestID());
  // logger.info('-----after ReqQryContractBank-----')

  //ReqFromBankToFutureByFuture
	// td.ReqFromFutureToBankByFuture({
	// 	TradeCode: '202002',
	// 	BankID: '5',
	// 	BankBranchID: '0000',
	// 	BrokerID: '4040',
	// 	AccountID: account.UserID,
	// 	Password: account.FundPassword,
	// 	SecuPwdFlag: '1',// 明文核对
	// 	CurrencyID: 'CNY',
	// 	TradeAmount: 2000
	// }, ctp.nRequestID());

	td.ReqFromBankToFutureByFuture({
		TradeCode: '202001',
		BankID: '5',
		BankBranchID: '0000',
		BrokerID: '4040',
		AccountID: account.UserID,
		Password: account.FundPassword,
		SecuPwdFlag: '1',// 明文核对
		CurrencyID: 'CNY',
		TradeAmount: 2000
	}, ctp.nRequestID());
});

td.On("OnRspUserLogout", function(Logout, Rsp, nRequestID,bIsLast) {
  logger.info("OnRspUserLogout : %j, %j, %s, %s", Logout, Rsp, nRequestID, bIsLast);
});
// 报单通知
td.On("OnRtnOrder", function(data) {
  logger.info("OnRtnOrder: %j",  data)
});
// 成交通知
td.On("OnRtnTrade", function(data) {
	// 在这里查资金状况, 根据判断发出通知和出金改密操作
	td.ReqQryTradingAccount(account, ctp.nRequestID());

  logger.info("OnRtnTrade:",  data)
});

td.On('OnRspQryContractBank', function(data, Rsp, nRequestID,bIsLast) {
	logger.info("OnRspQryContractBank: %j",  data)
	logger.info("OnRspQryContractBank: %j, %j, %s, %s", data, Rsp, nRequestID, bIsLast)
});

td.On('OnRspQryTradingAccount', function(data, Rsp, nRequestID,bIsLast) {
	logger.info("OnRspQryTradingAccount: %j, %j, %s, %s",  data, Rsp, nRequestID,bIsLast)
});

td.On('OnRspFromFutureToBankByFuture', function(data, Rsp, nRequestID,bIsLast) {
	logger.info("OnRspFromFutureToBankByFuture: %j, %j, %s, %s",  data, Rsp, nRequestID,bIsLast)
});

td.On('OnRspFromBankToFutureByFuture', function(data, Rsp, nRequestID,bIsLast) {
	// td.ReqTradingAccountPasswordUpdate({
	// 	BrokerID: '4040',
	// 	AccountID: '',
	// 	OldPassword: '',
	// 	NewPassword: '',
	// 	CurrencyID: ''
	// }, ctp.nRequestID());
});

td.On('OnRspTradingAccountPasswordUpdate', function(data, Rsp, nRequestID,bIsLast) {
	logger.info("OnRspTradingAccountPasswordUpdate: %j, %j, %s, %s",  data, Rsp, nRequestID,bIsLast)
});

// ctp.createMdList();
ctp.createTdList();

ctp.registerFront({
	// md: true,
	td: true
});



/*
mailer.sendMailToReceiverList({
	subject: 'Hello',
	text: 'Hello world',
	html: '<h3>Hello World</h3>'
}, function() {
	logger.info(123)
})
*/