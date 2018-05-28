const logger = require('./logger').getLogger('market');

class Market {
  constructor(ctp) {
    this.ctp = ctp;
    this._init();
  }

  _init() {
    let ctp = this.ctp;

    ctp.createMdApi();
    this._regsiterHandlers();
    ctp.registerMdFront();
  }

  _regsiterHandlers() {
    let md = this.ctp.md;
    for (let property in this) {
      if (/^On[a-zA-Z]+$/.test(property)) {
        md.On(property, this[property].bind(this));
      }
    }
  }

  OnFrontConnected() {
    let { md, investor } = this.ctp;
    logger.info('ReqUserLogin : %s', md.ReqUserLogin(investor, this.ctp.nRequestID()));
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
}

module.exports = Market;


