const logger = require('./logger').getLogger('market');

function Market(ctp) {
	this.ctp = ctp;
	this._init();
}

(function() {
	this._init = function() {
		let ctp = this.ctp;

		ctp.createMdApi();
		this._regsiterHandlers();
		ctp.registerMdFront();
	};

	this._regsiterHandlers = function() {
		let md = this.ctp.md;
		for (let property in this) {
			if (/^On[a-zA-Z]+$/.test(property)) {
				md.On(property, this[property].bind(this));
			}
		}
	};

	this.OnFrontConnected = function() {
		let { md, investor } = this.ctp;
		logger.info('ReqUserLogin : %s', md.ReqUserLogin(investor, this.ctp.nRequestID()));
		logger.info('OnFrontConnected');
	};

	this.OnFrontDisconnected = function(nReason) {
	  	logger.info('OnFrontDisconnected nReason: %s', nReason);
	};

	this.OnRspUserLogin = function(data, rsp, nRequestID, bIsLast) {
	  	logger.info('OnRspUserLogin: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};

	this.OnRspUserLogout = function(data, rsp, nRequestID, bIsLast) {
	  	logger.info('OnRspUserLogout: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};

	this.OnRspError = function(rsp, nRequestID, bIsLast) {
    	logger.info('OnRspError: %j, %s, %s', rsp, nRequestID, bIsLast);
	};

	this.OnRspSubMarketData = function(data, rsp, nRequestID, bIsLast) {
    	logger.info('OnRspSubMarketData: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};

	this.OnRspUnSubMarketData = function(data, rsp, nRequestID, bIsLast) {
    	logger.info('OnRspSubMarketData: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};

	this.OnRspSubForQuoteRsp = function(data, rsp, nRequestID, bIsLast) {
    	logger.info('OnRspSubForQuoteRsp: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};

	this.OnRspUnSubForQuoteRsp = function(data, rsp, nRequestID, bIsLast) {
    	logger.info('OnRspUnSubForQuoteRsp: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
	};

	this.OnRtnForQuoteRsp = function(data) {
    	logger.info('OnRtnForQuoteRsp: %j', data);
	};

	this.OnRtnDepthMarketData = function(data) {
    	logger.info('OnRtnDepthMarketData: %j', data);
	};
}).call(Market.prototype);


module.exports = Market;


