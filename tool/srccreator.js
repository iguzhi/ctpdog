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
  let resultsMap = await readMdApi();
  data.mdSpi_rspMethods_call = resultsMap.mdSpi_rspMethods_call.join('\n');
  data.mdSpi_rspMethodsImpl_def = resultsMap.mdSpi_rspMethodsImpl_def.join('\n');
  data.mdSpi_struct = resultsMap.mdSpi_struct.join('\n');
  data.mdSpi_mainRspMethodsInterface_def = resultsMap.mdSpi_mainRspMethodsInterface_def.join('\n');
  data.mdSpi_rspMethodsInterface_def = resultsMap.mdSpi_rspMethodsInterface_def.join('\n');
  data.wrapMd_rspMethodsInterface_def = data.mdSpi_rspMethodsInterface_def;
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
  removeBlanks(lines);

  let structName, fields = [], structMap = {};

  for (let i = 0; i < lines.length - 1; i++) {
    let line = lines[i];
    line = trim(line);
    let words = splitWords(line);
    removeBlanks(words);
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

async function readMdApi() {
  let filepath = path.join(__dirname, '../api/tradeapi_linux64/ThostFtdcMdApi.h');

  let rsp = await readFile(filepath, 'utf8');
  if (rsp instanceof Error) {
    return;
  }

  rsp = rsp.replace(/\//g, '');
  let lines = rsp.split(spliter);

  // 删除空行
  removeBlanks(lines);

  let rspMethodsMap,
  structMap = {},
  resultsMap = {
    mdSpi_rspMethods_call: [],
    mdSpi_rspMethodsImpl_def: [],
    mdSpi_struct: [],
    mdSpi_mainRspMethodsInterface_def: [],
    mdSpi_rspMethodsInterface_def: []
  };

  for (let i = 0; i < lines.length - 1; i++) {
    let line = lines[i];
    line = trim(line);
    let words = splitWords(line);
    removeBlanks(words);
    let firstWord = words[0];
    
    if (firstWord === 'class' && words[1] === 'CThostFtdcMdSpi') {
      rspMethodsMap = {};
      continue;
    }
    if (rspMethodsMap) {
      if (firstWord === 'virtual' && words[1] === 'void' && /^On/.test(words[2])) {
        let funcName = words[2];
        rspMethodsMap[funcName] = getArgs(words.slice(3));
        continue;
      }

      if (firstWord === 'class') {
        for (let funcName in rspMethodsMap) {
          let args = rspMethodsMap[funcName];
          let argsText = '', originArgsText = '', text = '', structText = '';

          args.forEach((item, i) => {
            if (i) {
              argsText += ', ';
              originArgsText += ', ';
              text += '\n';
            }
            if (item.type === 'object') {
              if (item.name !== 'RspInfo') {
                argsText += `&task->data.${item.name}`;
                text += `  t->data.${item.name} = *${item.originName};`;
                if (!structMap[item.originType]) {
                  structMap[item.originType] = true;
                  structText += `        ${item.originType} ${item.name};`;
                }
              }
              else {
                argsText += `&task->${item.name}`;
                text += `  t->${item.name} = *${item.originName};`;
              }
              originArgsText += `${item.originType} *${item.originName}`;
            }
            else {
              argsText += `task->${item.name}`;
              originArgsText += `${item.originType} ${item.originName}`;
              text += `  t->${item.name} = ${item.name};`
            }
          });
          
          resultsMap.mdSpi_rspMethods_call[funcName === 'OnRtnDepthMarketData' ? 'unshift' : 'push'](`  if(task->api == "${funcName}") { task->pmd->Main${funcName}(${argsText}); continue;}`);
          resultsMap.mdSpi_rspMethodsImpl_def.push(`void CThostFtdcMdSpiI::${funcName}(${originArgsText}) `);
          resultsMap.mdSpi_rspMethodsImpl_def.push('{');
          resultsMap.mdSpi_rspMethodsImpl_def.push('  GET_TASK(_FUNCTION_);');
          resultsMap.mdSpi_rspMethodsImpl_def.push(text);
          resultsMap.mdSpi_rspMethodsImpl_def.push('  uv_async_send_s(&t->handle);');
          resultsMap.mdSpi_rspMethodsImpl_def.push('}');
          structText && resultsMap.mdSpi_struct.push(structText);
          resultsMap.mdSpi_mainRspMethodsInterface_def.push(`        virtual void Main${funcName}(${originArgsText}) = 0;`);
          resultsMap.mdSpi_rspMethodsInterface_def.push(`        virtual void ${funcName}(${originArgsText});`);
        }
        rspMethodsMap = null;
      }
    }
  }

  return resultsMap;
}


function trim(value) {
  return value.replace(/^\s*/, '').replace(/\s*$/, '');
}

function isChineseCharacters(text) {
  return /[\u4e00-\u9fa5]+/.test(text);
}

function splitWords(line) {
  return line.split(/\s+|[;()*,]/);
}

function removeBlanks(list) {
  for(let i = list.length - 1; i >= 0; i--) {
    if (/^\s*$/.test(list[i])) {
      list.splice(i, 1);
    }
  }
}

function getArgType(name) {
  let type = name;
  if (/^CThostFtdc/.test(name)) {
    type = 'object';
  }
  return type;
}

function getArgs(words) {
  let args = [];
  for (let i = 0; i < words.length - 2; i += 2) {
    let originType = words[i];
    let name = words[i + 1];
    let type = getArgType(originType);
    args.push({
      type,
      originType,
      originName: name,
      name: name.replace(/^p/, '')
    });
  }
  return args;
}

exports.create = create;

// (async () => {
//   await create();
// })();
create();