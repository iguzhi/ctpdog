const logger = require('./logger').getLogger('market');
const helper = require('./helper');

class Market {
  constructor(ctp) {
    this.ctp = ctp;
    this._init();
  }

  _init() {
    this.createApi();
    this._regsiterHandlers();
    this.registerFront();
  }

  _regsiterHandlers() {
    let md = this.ctp.md;
    let allProps = helper.getAllProperties(this);
    allProps.forEach((prop) => {
      if (/^On[a-zA-Z]+$/.test(prop)) {
        md.On(prop, this[prop].bind(this));
      }
    });
  }

  createApi() {
    this.ctp.createMdApi();
  }

  registerFront() {
    this.ctp.registerMdFront();
  }

  OnFrontConnected() {
    logger.info('OnFrontConnected');
  }

  OnFrontDisconnected(nReason) {
    logger.info('OnFrontDisconnected nReason: %s', nReason);
  }

  OnRspUserLogin(data, rsp, nRequestID, bIsLast) {
    logger.info('OnRspUserLogin: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
  }

  OnRspUserLogout(data, rsp, nRequestID, bIsLast) {
    logger.info('OnRspUserLogout: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
  }

  OnRspError(rsp, nRequestID, bIsLast) {
    logger.info('OnRspError: %j, %s, %s', rsp, nRequestID, bIsLast);
  }

  OnRspSubMarketData(data, rsp, nRequestID, bIsLast) {
    logger.info('OnRspSubMarketData: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
  }

  OnRspUnSubMarketData(data, rsp, nRequestID, bIsLast) {
    logger.info('OnRspSubMarketData: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
  }

  OnRspSubForQuoteRsp(data, rsp, nRequestID, bIsLast) {
    logger.info('OnRspSubForQuoteRsp: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
  }

  OnRspUnSubForQuoteRsp(data, rsp, nRequestID, bIsLast) {
    logger.info('OnRspUnSubForQuoteRsp: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
  }

  OnRtnForQuoteRsp(data) {
    logger.info('OnRtnForQuoteRsp: %j', data);
  }

  OnRtnDepthMarketData(data) {
    logger.info('OnRtnDepthMarketData: %j', data);
  }

  OnRspQryMulticastInstrument(data, rsp, nRequestID, bIsLast) {
    logger.info('OnRspQryMulticastInstrument: %j, %j, %s, %s', data, rsp, nRequestID, bIsLast);
  }
}

module.exports = Market;


