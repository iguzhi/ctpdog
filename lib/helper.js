const fs = require('fs');
const loggerCfg = require('../config/logger');

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

/**
 * 设置日志打印级别, 若level为undefined, 则针对所有类别的日志设置打印级别
 * @param {String} category 日志类别
 * @param {String} level 打印级别
 */
exports.setLoggerLevel = function(category, level) {
  if (level) {
    loggerCfg.level[category] = level;
  }
  else {
    level = category;
    for (category in loggerCfg.level) {
      logger.hasOwnProperty(category) && (loggerCfg.level[category] = level);
    }
  }
};
