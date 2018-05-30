const fs = require('fs');
const dictCreator = require('../tool/dictcreator');

/**
 * 确保路径对应的目录存在, 若不存在则新建目录, 若存在则不做任何操作(支持多级目录)
 * @param {String} dirpath 目录路径
 */
exports.ensureDirExists =  function(dirpath) {
  let sep = /\//.test(dirpath) ? '/' : (/\\/.test(dirpath) ? '\\' : '');
  let pathArr = dirpath.split(sep);
  dirpath = '';
  pathArr.forEach((s, index) => {
    if (index === 0) {
      if (s === '') {
        dirpath += sep;
      }
      else if (s === '.') {
        dirpath += '.' + sep;
      }
      else {
        dirpath += s + sep;
      }
    }
    else {
      dirpath += s + sep;
    }

    if (fs.existsSync(dirpath)) {
      let stat = fs.lstatSync(dirpath);
      if (!stat.isDirectory()) {
        fs.mkdirSync(dirpath);
      }
    }
    else {
      fs.mkdirSync(dirpath);
    };
  })
};

exports.createDictFile = function() {
  dictCreator.create();
};

exports.getAllProperties = function(obj) {
  let allProps = [], curr = obj;
  do {
    let props = Object.getOwnPropertyNames(curr);
    props.forEach(function(prop) {
      if (allProps.indexOf(prop) === -1) {
        allProps.push(prop);
      }
    });
  } while(curr = Object.getPrototypeOf(curr))
  return allProps;
};

/**
 * 暂停ms后继续执行, 用在async函数中, 封装该接口到原因上ctp要求接口调用的间隔必须大于等于1秒
 * @param {Integer} ms 毫秒
 */
exports.sleep = function(ms) {
  return new Promise((resolve) => {
      setTimeout(resolve, ms);
  });
};