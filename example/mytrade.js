const logger = require('../index').logger.getLogger('trade');
const { Trade } = require('../index');

class MyTrade extends Trade {

	OnFrontConnected() {
		super.OnFrontConnected(...arguments);
		let { investor, td, setting } = this.ctp;
		// 登录失败达到maxTryLoginTimes次, 则释放该账户的ctp对象
		if (!investor.tryLoginTimes || investor.tryLoginTimes < setting.maxTryLoginTimes) {
			investor.tryLoginTimes = investor.tryLoginTimes || 0;
			investor.tryLoginTimes++;
			
			logger.info('ReqUserLogin : %s', td.ReqUserLogin(investor, this.ctp.nRequestID()));
		}
		else {
			td.Release();
			logger.info('Try ReqUserLogin %s times by Investor: %j, but failed!', setting.maxTryLoginTimes, investor);
		}
	}

	OnRspUserLogin(data, rsp, nRequestID, bIsLast) {
	  super.OnRspUserLogin(...arguments);

	  let investor = this.ctp.investor;
	  // 投资者结算结果确认, 做完这一步才可以进行正常的交易
	  this.ctp.td.ReqSettlementInfoConfirm({
	  	BrokerID: investor.BrokerID,
	  	InvestorID: investor.UserID,
	  	ConfirmDate: data.TradingDay,
	  	ConfirmTime: data.SHFETime
	  }, this.ctp.nRequestID());
	  // ctp.td.ReqQryTradingAccount(investor, ctp.nRequestID());
	  //logger.info('ReqQryTradingAccount=', ctp.td.ReqQryTradingAccount(q, (new Date()).valueOf()/1000));
	  //sleep(2000);
	  //logger.info('ReqQryInvestorPosition=', ctp.td.ReqQryInvestorPosition(q, (new Date()).valueOf()/1000));
	  //logger.info('ReqQryTrade=', ctp.td.ReqQryTrade(q, (new Date()).valueOf()/1000));
	  //logger.info('ReqQryOrder=', ctp.td.ReqQryOrder(q, (new Date()).valueOf()/1000));
	  //logger.info('ReqQryProduct=', ctp.td.ReqQryProduct(q, (new Date()).valueOf()/1000));
	  //logger.info('ReqQryInstrument=', ctp.td.ReqQryInstrument(q, (new Date()).valueOf()/1000));
	  //logger.info('-----before ReqQryContractBank-----')
	  // ctp.td.ReqQryContractBank({
	  // 	BankID: '5',
			// BankBranchID: '0000',
			// BrokerID: '4040',
	  // }, ctp.nRequestID());
	  // logger.info('-----after ReqQryContractBank-----')

	  //ReqFromBankToFutureByFuture
		// ctp.td.ReqFromFutureToBankByFuture({
		// 	TradeCode: '202002',
		// 	BankID: '5',
		// 	BankBranchID: '0000',
		// 	BrokerID: '4040',
		// 	BankAccount: '', // 是否必填, 不确定
		// 	BankPassWord: '',
		// 	AccountID: investor.UserID,
		// 	Password: investor.FundPassword,
		// 	SecuPwdFlag: '1',// 明文核对
		// 	CurrencyID: 'CNY',
		// 	TradeAmount: 2000
		// }, ctp.nRequestID());

		// ctp.td.ReqFromBankToFutureByFuture({
		// 	TradeCode: '202001',
		// 	BankID: '5',
		// 	BankBranchID: '0000',
		// 	BrokerID: '4040',
		// 	BankAccount: '', // 是否必填, 不确定
		// 	BankPassWord: '',
		// 	AccountID: investor.UserID,
		// 	Password: investor.FundPassword,
		// 	SecuPwdFlag: '1',// 明文核对
		// 	CurrencyID: 'CNY',
		// 	TradeAmount: 2000
		// }, ctp.nRequestID());
	}

	OnRspUserLogout(data, rsp, nRequestID, bIsLast) {
		super.OnRspUserLogout(...arguments);
	}
	// 报单通知
	OnRtnOrder(data) {
		super.OnRtnOrder(...arguments);
	  // logger.info('OnRtnOrder: %j',  data)
	}
	// 成交通知
	OnRtnTrade(data) {
		super.OnRtnTrade(...arguments);
		// 在这里查资金状况, 根据判断发出通知和出金改密操作
		// 平仓: OffsetFlag==3, 开仓: OffsetFlag==0
		data.OffsetFlag != 0 && this.ctp.td.ReqQryTradingAccount(this.ctp.investor, this.ctp.nRequestID());
	  
	}

	OnRspQryTradingAccount(data, rsp, nRequestID, bIsLast) {
		super.OnRspQryTradingAccount(...arguments);
	}

	OnRspFromFutureToBankByFuture(data, rsp, nRequestID, bIsLast) {
		super.OnRspFromFutureToBankByFuture(...arguments);
	}

	OnRspFromBankToFutureByFuture(data, rsp, nRequestID, bIsLast) {
		super.OnRspFromBankToFutureByFuture(...arguments);
		// ctp.td.ReqTradingAccountPasswordUpdate({
		// 	BrokerID: '4040',
		// 	AccountID: '',
		// 	OldPassword: '',
		// 	NewPassword: '',
		// 	CurrencyID: ''
		// }, ctp.nRequestID());
	}

	OnRspTradingAccountPasswordUpdate(data, rsp, nRequestID, bIsLast) {
		super.OnRspTradingAccountPasswordUpdate(...arguments);
	}

}

module.exports = MyTrade;