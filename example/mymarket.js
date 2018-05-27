const logger = require('../index').logger.app;
const ee = require('../index').ee;

const Class = require('iguzhi/class');

function Market(ctp) {
	this.$superConstructor(arguments);
}

(function() {

	this.OnRspUserLogin = function(data, rsp, nRequestID, bIsLast) {
		this.$superMethod(arguments);
	 	console.log("SubscribeMarketData:", this.ctp.md.SubscribeMarketData(['CF901', 'AP810']));
	};

	this.OnRspSubMarketData = function(data, rsp, nRequestID, bIsLast) {
    	this.$superMethod(arguments);
	};

	this.OnRspUnSubMarketData = function(data, rsp, nRequestID, bIsLast) {
    	this.$superMethod(arguments);
	};

	this.OnRtnDepthMarketData = function(data) {
		this.$superMethod(arguments);
		ee.emit('OnRtnDepthMarketData', data);
	};
}).call(Market.prototype);

Class.inherit(Market, require('../index').Market);

module.exports = Market;


