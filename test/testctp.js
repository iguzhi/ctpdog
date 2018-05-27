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
        InvestorID: '0060000183',
        Password: 'iguzhi-1288'
    }
);

ctp.createTdApi();
ctp.createMdApi();

let td = ctp.td;
let md = ctp.md;

console.log(td)
for(let i in td) {
    console.log(i)
};