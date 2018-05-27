const logger = require('./lib/logger').ctpapp;
const tevent = require('./lib/ntevent');

const Class = require('iguzhi/class');

function Market(ctp, userID) {
	this.$superConstructor(arguments);
}

(function() {

	this.OnRspUserLogin = function(data, rsp, nRequestID, bIsLast) {
		this.$superMethod(arguments);
	 	console.log("SubscribeMarketData:", this.ctp.md.SubscribeMarketData(['zn1707', 'ru1709', 'rb1710']));
	};

	this.OnRspSubMarketData = function(data, rsp, nRequestID, bIsLast) {
    	this.$superMethod(arguments);
	};

	this.OnRspUnSubMarketData = function(data, rsp, nRequestID, bIsLast) {
    	this.$superMethod(arguments);
	};

	this.OnRtnDepthMarketData = function(data) {
    	this.$superMethod(arguments);
	};
}).call(Market.prototype);

Class.inherit(Market, require('./lib/market'));

module.exports = Market;


