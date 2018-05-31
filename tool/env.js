const path = require('path');
const { getRoot } = require('bindings');

if (process.platform === 'win32') {
  let root = getRoot(__filename);
  let arch = process.arch === 'x64' ? '64' : '';
  let envpath = path.join(root, `20180109_tradeapi${arch}_windows`);
  if (process.env.PATH.indexOf(envpath) === -1) {
    process.env.PATH += `;${envpath}`;
  }
}
