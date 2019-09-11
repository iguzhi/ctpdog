const logger = require('../index').logger.getLogger('market');
const { Market } = require('../index');

class MyMarket extends Market {

  OnFrontConnected() {
    super.OnFrontConnected(...arguments);
    let { md, investor } = this.ctp;
    logger.info('ReqUserLogin : %s', md.ReqUserLogin(investor, this.ctp.nRequestID()));
  }
  
  OnRspUserLogin(data, rsp, nRequestID, bIsLast) {
    super.OnRspUserLogin(...arguments);
    console.log("SubscribeMarketData:", this.ctp.md.SubscribeMarketData(['sc1911']));
  }

  OnRspSubMarketData(data, rsp, nRequestID, bIsLast) {
    super.OnRspSubMarketData(...arguments);
  }

  OnRspUnSubMarketData(data, rsp, nRequestID, bIsLast) {
    super.OnRspUnSubMarketData(...arguments);
  }

  OnRtnDepthMarketData(data) {
    super.OnRtnDepthMarketData(...arguments);
  }

}

module.exports = MyMarket;


