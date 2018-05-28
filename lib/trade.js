const logger = require('./logger').getLogger('trade');

function Trade(ctp, { privateTopicMode, publicTopicModel }) {
	this.ctp = ctp;
	this._init({ privateTopicMode, publicTopicModel });
}

(function() {
	this._init = function({ privateTopicMode, publicTopicModel }) {
		let ctp = this.ctp;
		
		ctp.createTdApi();
		let td = ctp.td;
		this._regsiterHandlers();
		//THOST_TERT_RESTART:从本交易日开始重传 0
		//THOST_TERT_RESUME:从上次收到的续传    1
		//THOST_TERT_QUICK:只传送登录后私有流的内容 2
		td.SubscribePrivateTopic(typeof privateTopicMode === 'undefined' ? 2 : privateTopicMode);
		td.SubscribePublicTopic(typeof publicTopicModel === 'undefined' ? 2 : publicTopicModel);
		ctp.registerTdFront();
	};

	this._regsiterHandlers = function() {
		let td = this.ctp.td;
		for (let property in this) {
			if (/^On[a-zA-Z]+$/.test(property)) {
				td.On(property, this[property].bind(this));
			}
		}
	};

	this.OnFrontConnected = function() {
		let { investor, td, setting } = this.ctp;
		// 登录失败达到10次, 则释放该账户的ctp对象
		if (!investor.tryLoginTimes || investor.tryLoginTimes < setting.maxTryLoginTimes) {
			investor.tryLoginTimes = investor.tryLoginTimes || 0;
			investor.tryLoginTimes++;
			
			logger.info('ReqUserLogin : %s', td.ReqUserLogin(investor, this.ctp.nRequestID()));
	  		logger.info('OnFrontConnected');
		}
		else {
			td.Release();
			logger.info('Try ReqUserLogin %s times by Investor: %j, but failed!', setting.maxTryLoginTimes, investor);
		}
	};

	this.OnFrontDisconnected = function(nReason) {
	  logger.info('OnFrontDisconnected nReason: %s', nReason);
	};

	this.OnRspAuthenticate = function(data, rsp, nRequestID, bIsLast) {
	  logger.info('OnRspAuthenticate : %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};

	this.OnRspUserLogin = function(data, rsp, nRequestID, bIsLast) {
	  logger.info('OnRspUserLogin : %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	  // 投资者结算结果确认, 做完这一步才可以进行正常的交易
	  // this.ctp.td.ReqSettlementInfoConfirm({
	  // 	BrokerID: data.BrokerID,
	  // 	InvestorID: data.UserID,
	  // 	ConfirmDate: data.TradingDay,
	  // 	ConfirmTime: data.SHFETime
	  // }, this.ctp.nRequestID());
	};

	this.OnRspUserLogout = function(data, rsp, nRequestID, bIsLast) {
	  logger.info('OnRspUserLogout : %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 报单通知
	this.OnRtnOrder = function(data) {
	  logger.info('OnRtnOrder: %j',  data)
	};
	// 成交通知
	this.OnRtnTrade = function(data) {
	  logger.info('OnRtnTrade:',  data);
	};
	// 报单录入请求响应
	this.OnRspOrderInsert = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspOrderInsert: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 报单操作请求响应
	this.OnRspOrderAction = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspOrderAction: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 预埋单录入请求响应
	this.OnRspParkedOrderInsert = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspParkedOrderInsert: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 预埋撤单录入请求响应
	this.OnRspParkedOrderAction = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspParkedOrderAction: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 删除预埋单响应
	this.OnRspRemoveParkedOrder = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspParkedOrderAction: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 删除预埋撤单响应
	this.OnRspRemoveParkedOrderAction = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspParkedOrderAction: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 执行宣告录入请求响应
	this.OnRspExecOrderInsert = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspExecOrderInsert: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 执行宣告操作请求响应
	this.OnRspExecOrderAction = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspExecOrderAction: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 询价录入请求响应
	this.OnRspForQuoteInsert = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspForQuoteInsert: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 报价录入请求响应
	this.OnRspQuoteInsert = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQuoteInsert: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 报价操作请求响应
	this.OnRspQuoteAction = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQuoteAction: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 批量报单操作请求响应
	this.OnRspBatchOrderAction = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspBatchOrderAction: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 申请组合录入请求响应
	this.OnRspCombActionInsert = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspCombActionInsert: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询报单响应
	this.OnRspQryOrder = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryOrder: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询成交响应
	this.OnRspQryTrade = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryTrade: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 查询最大报单数量响应
	this.OnRspQueryMaxOrderVolume = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQueryMaxOrderVolume: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 投资者结算结果确认响应
	this.OnRspSettlementInfoConfirm = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspSettlementInfoConfirm: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询投资者持仓响应
	this.OnRspQryInvestorPosition = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryInvestorPosition: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询签约银行响应
	this.OnRspQryContractBank = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryContractBank: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询资金账户响应
	this.OnRspQryTradingAccount = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryTradingAccount: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 期货发起期货资金转银行应答
	this.OnRspFromFutureToBankByFuture = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspFromFutureToBankByFuture: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 期货发起银行资金转期货应答
	this.OnRspFromBankToFutureByFuture = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspFromBankToFutureByFuture: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
		// ctp.td.ReqTradingAccountPasswordUpdate({
		// 	BrokerID: '4040',
		// 	AccountID: '',
		// 	OldPassword: '',
		// 	NewPassword: '',
		// 	CurrencyID: ''
		// }, ctp.nRequestID());
	};
	// 用户口令更新请求响应
	this.OnRspUserPasswordUpdate = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspUserPasswordUpdate: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 资金账户口令更新请求响应
	this.OnRspTradingAccountPasswordUpdate = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspTradingAccountPasswordUpdate: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询投资者响应
	this.OnRspQryInvestor = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryInvestor: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询交易编码响应
	this.OnRspQryTradingCode = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryTradingCode: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询合约保证金率响应
	this.OnRspQryInstrumentMarginRate = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryInstrumentMarginRate: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询合约手续费率响应 
	this.OnRspQryInstrumentCommissionRate = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryInstrumentCommissionRate: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询交易所响应
	this.OnRspQryExchange = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryExchange: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询产品响应
	this.OnRspQryProduct = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryProduct: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询合约响应
	this.OnRspQryInstrument = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryInstrument: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询行情响应
	this.OnRspQryDepthMarketData = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryDepthMarketData: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询投资者结算结果响应
	this.OnRspQrySettlementInfo = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQrySettlementInfo: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询转帐银行响应
	this.OnRspQryTransferBank = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryTransferBank: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询投资者持仓明细响应
	this.OnRspQryInvestorPositionDetail = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryInvestorPositionDetail: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询客户通知响应
	this.OnRspQryNotice = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryNotice: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询结算信息确认响应
	this.OnRspQrySettlementInfoConfirm = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQrySettlementInfoConfirm: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询投资者持仓组合明细响应
	this.OnRspQryInvestorPositionCombineDetail = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryInvestorPositionCombineDetail: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 查询保证金监管系统经纪公司资金账户密钥响应
	this.OnRspQryCFMMCTradingAccountKey = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryCFMMCTradingAccountKey: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询仓单折抵信息响应
	this.OnRspQryEWarrantOffset = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryEWarrantOffset: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询投资者品种/跨品种保证金响应
	this.OnRspQryInvestorProductGroupMargin = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryInvestorProductGroupMargin: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询交易所保证金率响应
	this.OnRspQryExchangeMarginRate = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryExchangeMarginRate: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询交易所调整保证金率响应
	this.OnRspQryExchangeMarginRateAdjust = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryExchangeMarginRateAdjust: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询汇率响应
	this.OnRspQryExchangeRate = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryExchangeRate: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询二级代理操作员银期权限响应
	this.OnRspQrySecAgentACIDMap = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQrySecAgentACIDMap: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询产品报价汇率响应
	this.OnRspQryProductExchRate = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryProductExchRate: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询产品组响应
	this.OnRspQryProductGroup = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryProductGroup: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询做市商合约手续费率响应
	this.OnRspQryMMInstrumentCommissionRate = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryMMInstrumentCommissionRate: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询做市商期权合约手续费响应
	this.OnRspQryMMOptionInstrCommRate = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryMMOptionInstrCommRate: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询报单手续费响应
	this.OnRspQryInstrumentOrderCommRate = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryInstrumentOrderCommRate: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询期权交易成本响应
	this.OnRspQryOptionInstrTradeCost = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryOptionInstrTradeCost: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询期权合约手续费响应
	this.OnRspQryOptionInstrCommRate = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryOptionInstrCommRate: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询执行宣告响应
	this.OnRspQryExecOrder = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryExecOrder: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询询价响应
	this.OnRspQryForQuote = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryForQuote: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询报价响应
	this.OnRspQryQuote = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryQuote: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询组合合约安全系数响应
	this.OnRspQryCombInstrumentGuard = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryCombInstrumentGuard: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询申请组合响应
	this.OnRspQryCombAction = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryCombAction: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询转帐流水响应
	this.OnRspQryTransferSerial = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryTransferSerial: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询银期签约关系响应
	this.OnRspQryAccountregister = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryAccountregister: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 错误应答
	this.OnRspError = function(rsp, nRequestID, bIsLast) {
		logger.info('OnRspError: %j, %s, %s', rsp, nRequestID, bIsLast);
	};
	// 报单录入错误回报
	this.OnErrRtnOrderInsert = function(data, rsp) {
		logger.info('OnErrRtnOrderInsert: %j, %j', data, rsp);
	};
	// 报单操作错误回报
	this.OnErrRtnOrderAction = function(data, rsp) {
		logger.info('OnErrRtnOrderAction: %j, %j', data, rsp);
	};
	// 合约交易状态通知
	this.OnRtnInstrumentStatus = function(data) {
		logger.info('OnRtnInstrumentStatus: %j', data);
	};
	// 交易所公告通知
	this.OnRtnBulletin = function(data) {
		logger.info('OnRtnBulletin: %j', data);
	};
	// 交易通知
	this.OnRtnTradingNotice = function(data) {
		logger.info('OnRtnTradingNotice: %j', data);
	};
	// 提示条件单校验错误
	this.OnRtnErrorConditionalOrder = function(data) {
		logger.info('OnRtnErrorConditionalOrder: %j', data);
	};
	// 执行宣告通知
	this.OnRtnExecOrder = function(data) {
		logger.info('OnRtnExecOrder: %j', data);
	};
	// 执行宣告录入错误回报
	this.OnErrRtnExecOrderInsert = function(data, rsp) {
		logger.info('OnErrRtnExecOrderInsert: %j, %j', data, rsp);
	};
	// 执行宣告操作错误回报
	this.OnErrRtnExecOrderAction = function(data, rsp) {
		logger.info('OnErrRtnExecOrderAction: %j, %j', data, rsp);
	};
	// 询价录入错误回报
	this.OnErrRtnForQuoteInsert = function(data, rsp) {
		logger.info('OnErrRtnForQuoteInsert: %j, %j', data, rsp);
	};
	// 报价通知
	this.OnRtnQuote = function(data) {
		logger.info('OnRtnQuote: %j', data);
	};
	// 报价录入错误回报
	this.OnErrRtnQuoteInsert = function(data, rsp) {
		logger.info('OnErrRtnQuoteInsert: %j, %j', data, rsp);
	};
	// 报价操作错误回报
	this.OnErrRtnQuoteAction = function(data, rsp) {
		logger.info('OnErrRtnQuoteAction: %j, %j', data, rsp);
	};
	// 询价通知
	this.OnRtnForQuoteRsp = function(data) {
		logger.info('OnRtnForQuoteRsp: %j', data);
	};
	// 保证金监控中心用户令牌
	this.OnRtnCFMMCTradingAccountToken = function(data) {
		logger.info('OnRtnCFMMCTradingAccountToken: %j', data);
	};
	// 批量报单操作错误回报
	this.OnErrRtnBatchOrderAction = function(data, rsp) {
		logger.info('OnErrRtnBatchOrderAction: %j, %j', data, rsp);
	};
	// 申请组合通知
	this.OnRtnCombAction = function(data) {
		logger.info('OnRtnCombAction: %j', data);
	};
	// 申请组合录入错误回报
	this.OnErrRtnCombActionInsert = function(data, rsp) {
		logger.info('OnErrRtnCombActionInsert: %j, %j', data, rsp);
	};
	// 请求查询预埋撤单响应
	this.OnRspQryParkedOrder = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryParkedOrder: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询交易通知响应
	this.OnRspQryTradingNotice = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryTradingNotice: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询经纪公司交易参数响应
	this.OnRspQryBrokerTradingParams = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryBrokerTradingParams: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询经纪公司交易算法响应
	this.OnRspQryBrokerTradingAlgos = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQryBrokerTradingAlgos: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 请求查询监控中心用户令牌响应
	this.OnRspQueryCFMMCTradingAccountToken = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQueryCFMMCTradingAccountToken: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 银行发起银行资金转期货通知
	this.OnRtnFromBankToFutureByBank = function(data) {
		logger.info('OnRtnFromBankToFutureByBank: %j', data);
	};
	// 银行发起期货资金转银行通知
	this.OnRtnFromFutureToBankByBank = function(data) {
		logger.info('OnRtnFromFutureToBankByBank: %j', data);
	};
	// 银行发起冲正银行转期货通知
	this.OnRtnRepealFromBankToFutureByBank = function(data) {
		logger.info('OnRtnRepealFromBankToFutureByBank: %j', data);
	};
	// 银行发起冲正期货转银行通知
	this.OnRtnRepealFromFutureToBankByBank = function(data) {
		logger.info('OnRtnRepealFromFutureToBankByBank: %j', data);
	};
	// 期货发起银行资金转期货通知
	this.OnRtnFromBankToFutureByFuture = function(data) {
		logger.info('OnRtnFromBankToFutureByFuture: %j', data);
	};
	// 期货发起期货资金转银行通知
	this.OnRtnFromFutureToBankByFuture = function(data) {
		logger.info('OnRtnFromFutureToBankByFuture: %j', data);
	};
	// 系统运行时期货端手工发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
	this.OnRtnRepealFromBankToFutureByFutureManual = function(data) {
		logger.info('OnRtnRepealFromBankToFutureByFutureManual: %j', data);
	};
	// 系统运行时期货端手工发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
	this.OnRtnRepealFromFutureToBankByFutureManual = function(data) {
		logger.info('OnRtnRepealFromFutureToBankByFutureManual: %j', data);
	};
	// 期货发起查询银行余额通知
	this.OnRtnQueryBankBalanceByFuture = function(data) {
		logger.info('OnRtnQueryBankBalanceByFuture: %j', data);
	};
	// 期货发起银行资金转期货错误回报
	this.OnErrRtnBankToFutureByFuture = function(data, rsp) {
		logger.info('OnErrRtnBankToFutureByFuture: %j, %j', data, rsp);
	};
	// 期货发起期货资金转银行错误回报
	this.OnErrRtnFutureToBankByFuture = function(data, rsp) {
		logger.info('OnErrRtnFutureToBankByFuture: %j, %',data, rsp);
	};
	// 系统运行时期货端手工发起冲正银行转期货错误回报
	this.OnErrRtnRepealBankToFutureByFutureManual = function(data, rsp) {
		logger.info('OnErrRtnRepealBankToFutureByFutureManual: %j, %j', data, rsp);
	};
	// 系统运行时期货端手工发起冲正期货转银行错误回报
	this.OnErrRtnRepealFutureToBankByFutureManual = function(data, rsp) {
		logger.info('OnErrRtnRepealFutureToBankByFutureManual: %j, %j', data, rsp);
	};
	// 期货发起查询银行余额错误回报
	this.OnErrRtnQueryBankBalanceByFuture = function(data, rsp) {
		logger.info('OnErrRtnQueryBankBalanceByFuture: %j, %j', data, rsp);
	};
	// 期货发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
	this.OnRtnRepealFromBankToFutureByFuture = function(data) {
		logger.info('OnRtnRepealFromBankToFutureByFuture: %j', data);
	};
	// 期货发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
	this.OnRtnRepealFromFutureToBankByFuture = function(data) {
		logger.info('OnRtnRepealFromFutureToBankByFuture: %j', data);
	};
	// 期货发起查询银行余额应答 
	this.OnRspQueryBankAccountMoneyByFuture = function(data, rsp, nRequestID, bIsLast) {
		logger.info('OnRspQueryBankAccountMoneyByFuture: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};
	// 银行发起银期开户通知
	this.OnRtnOpenAccountByBank = function(data) {
		logger.info('OnRtnOpenAccountByBank: %j', data);
	};
	// 银行发起银期销户通知
	this.OnRtnCancelAccountByBank = function(data) {
		logger.info('OnRtnCancelAccountByBank: %j', data);
	};
	// 银行发起变更银行账号通知
	this.OnRtnChangeAccountByBank = function(data) {
		logger.info('OnRtnChangeAccountByBank: %j', data);
	};
}).call(Trade.prototype);


module.exports = Trade;