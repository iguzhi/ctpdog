'use strict';

const path = require('path');
const fs = require('fs');
const os = require('os');
const fileHelper = require('./helper/filehelper');

const spliter = os === 'win32' ? '\r\n' : '\n';

function create() {
  const data = {
    ctpNode_setterMethods_def: '#11',
    mdSpi_rspMethods_call: '#11',
    mdSpi_rspMethodsImpl_def: '#11',
    mdSpi_mainRspMethodsInterface_def: '#11',
    mdSpi_rspMethodsInterface_def: '#11',
    mdSpi_struct: '#11',
    traderSpi_rspMethods_call: '#11',
    traderSpi_rspMethodsImpl_def: '#11',
    traderSpi_struct: '#11',
    traderSpi_mainRspMethodsInterface_def: '#11',
    traderSpi_rspMethodsInterface_def: '#11',
    wrapMd_setJsMethods: '#22',
    wrapMd_addRspEvent: '#22',
    wrapMd_reqMethodsImpl_def: '#22',
    wrapMd_mainRspMethodsImpl_def: '#22',
    wrapMd_reqMethodsInterface_def: '#22',
    wrapMd_rspMethodsInterface_def: '#22',
    wrapTd_setJsMethods: '#22',
    wrapTd_addRspEvent: '#22',
    wrapTd_reqMethodsImpl_def: '#22',
    wrapTd_mainRspMethodsImpl_def: '#22',
    wrapTd_reqMethodsInterface_def: '#22',
    wrapTd_rspMethodsInterface_def: '#22',
  };

  // TODO
  fileHelper.copyDir(path.join(__dirname, '../src_tpl'), path.join(__dirname, '../src'), data, '', true);
}

function readUserApiStruct() {
  let filepath = path.join(__dirname, '../api/tradeapi_linux64/ThostFtdcUserApiStruct.h');

  fs.readFile(filepath, 'utf8', (err, data) => {
    if (err) throw err;
    data = data.replace(/\//g, '');
    let lines = data.split(spliter);

    for(let i = lines.length - 1; i >= 0; i--) {
      if (/^\s*$/.test(lines[i])) {
        lines.splice(i, 1);
      }
      // else {
      //   lines[i] = lines[i].replace(/'/g, '"');
      // }
    }

    let description;

    for (let i = 0; i < lines.length - 1; i++) {
      let line = lines[i];
      let nextLine = lines[i + 1];
      
      if (/^TFtdc/.test(line)) {
        if (/[\u4e00-\u9fa5]+/.test(nextLine)) {
          description = trim(nextLine);
          line = line.replace(/^TFtdc/, '').replace('是一个', ' ');
          let arr = line.split(/\s/);
          dictText += "\n// " + arr[1] + "\n";
          enumText += "\n// " + arr[1] + "\n";
          dictText += "exports." + arr[0] + " = {";
          enumText += "exports." + arr[0] + " = new Enum(";
          // i += 1;
          inDict = true;
        }
        else {
          description = '';
        }
      }
      else if (inDict) {
        if (description && /^#define\s/.test(line)) {
          let words = line.split(/\s+/);
          if (words.length >= 3) {
            let value = words[2];
            value = trim(value);
            let key = words[1];
            let keys = key.split(/_/);
            key = keys[keys.length - 1];
            key = trim(key);
            dictText += "\n  // " + description + "\n";
            dictText += "  " + key + ": " + value + ",";
            enumText += "\n  { alias: '" + key + "', value: " + value + ", text: '" + description + "' },";
            description = '';
          }
        }
        else if (/[\u4e00-\u9fa5]+/.test(line)) {
          description = trim(line);
        }
        else if(/^typedef/.test(line)) {
          dictText += "\n};\n";
          enumText += "\n);\n";
          inDict = false;
        }
      }
    }
}


function trim(value) {
  return value.replace(/^\s*/, '').replace(/\s*$/, '');
}

function isChineseCharacters(text) {
  return /[\u4e00-\u9fa5]+/.test(text);
}

exports.create = create;

create()
