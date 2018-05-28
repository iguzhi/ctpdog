const logger = require('../index').logger.getLogger('app');
const { ee, Market } = require('../index');

class MyMarket extends Market {
  
  OnRspUserLogin(data, rsp, nRequestID, bIsLast) {
    super.OnRspUserLogin(...arguments);
    console.log("SubscribeMarketData:", this.ctp.md.SubscribeMarketData(['CF901', 'AP810']));
  }

  OnRspSubMarketData(data, rsp, nRequestID, bIsLast) {
    super.OnRspSubMarketData(...arguments);
  }

  OnRspUnSubMarketData(data, rsp, nRequestID, bIsLast) {
    super.OnRspUnSubMarketData(...arguments);
  }

  OnRtnDepthMarketData(data) {
    super.OnRtnDepthMarketData(...arguments);
    ee.emit('OnRtnDepthMarketData', data);
  }

}

module.exports = MyMarket;


