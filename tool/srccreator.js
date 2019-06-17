'use strict';

const path = require('path');
const fs = require('fs');
const os = require('os');
const fileHelper = require('./helper/filehelper');

const spliter = os === 'win32' ? '\r\n' : '\n';
const readFile = require("util").promisify(fs.readFile);

async function create() {
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

  data.ctpNode_setterMethods_def = await readUserApiStruct(); // ctp_node.h
  // TODO
  fileHelper.copyDir(path.join(__dirname, '../src_tpl'), path.join(__dirname, '../src'), data, '', true);
}

async function readUserApiStruct() {
  let filepath = path.join(__dirname, '../api/tradeapi_linux64/ThostFtdcUserApiStruct.h');

  let rsp = await readFile(filepath, 'utf8');
  if (rsp instanceof Error) {
    return;
  }

  rsp = rsp.replace(/\//g, '');
  let lines = rsp.split(spliter);

  // 删除空行
  for(let i = lines.length - 1; i >= 0; i--) {
    if (/^\s*$/.test(lines[i])) {
      lines.splice(i, 1);
    }
  }

  let structName, fields = [], structMap = {};

  for (let i = 0; i < lines.length - 1; i++) {
    let line = lines[i];
    line = trim(line);
    let words = splitWords(line);
    let firstWord = words[0];
    
    if (firstWord === 'struct') {
      structName = words[1];
      continue;
    }
    if (structName) {
      if (firstWord === '}') {
        structMap[structName] = fields;
        structName = null;
        fields = [];
        continue;
      }
      if (/^TThostFtdc/.test(firstWord)) {
        fields.push(words[1]);
      }
    }
  }

  let results = [];

  for(let structName in structMap) {
    let fields = structMap[structName];

    results.push(`static void set_obj(Local<Object>& obj, ${structName} *p)`);
    results.push('{');
    fields.forEach(function(fieldName) {
      results.push(`  set_obj(obj, "${fieldName}", &p->${fieldName});`);
    });
    results.push('}');
  }

  return results.join('\n');
}


function trim(value) {
  return value.replace(/^\s*/, '').replace(/\s*$/, '');
}

function isChineseCharacters(text) {
  return /[\u4e00-\u9fa5]+/.test(text);
}

function splitWords(line) {
  return line.split(/\s+|;/);
}

exports.create = create;

// (async () => {
//   await create();
// })();
create();