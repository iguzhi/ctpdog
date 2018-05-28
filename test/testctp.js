const CtpClass = require('../lib/ctp');

let ctp = new CtpClass(
    {
        frontend: {
           	BrokerID: '4040',// 银河期货
           	MdURL: 'tcp://180.166.103.21:51213',
           	TdURL: 'tcp://180.166.103.21:51205'
        }
    },
    {
        InvestorID: '369888',
        Password: '123456'
    }
);

// ctp.createTdApi();
ctp.createMdApi();

// let td = ctp.td;
// let md = ctp.md;

// console.log(td)
// for(let i in td) {
//     console.log(i)
// };

ctp.md.On('OnFrontConnected', function() {

    let { md, investor } = ctp;
    console.log('ReqUserLogin : %s', md.ReqUserLogin(investor, ctp.nRequestID()));
    console.log('OnFrontConnected');
})
ctp.registerMdFront();