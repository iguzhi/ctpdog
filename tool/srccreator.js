'use strict';

const path = require('path');
const fs = require('fs');
const os = require('os');
const fileHelper = require('./helper/filehelper');

const spliter = os === 'win32' ? '\r\n' : '\n';
const readFile = require("util").promisify(fs.readFile);

async function create() {
  let resultsMap = await readMdApi();

  const data = {
    ctpNode_setterMethods_def: await readUserApiStruct(), // ctp_node.h
    mdSpi_rspMethods_call: resultsMap.mdSpi_rspMethods_call.join('\n'),
    mdSpi_rspMethodsImpl_def: resultsMap.mdSpi_rspMethodsImpl_def.join('\n'),
    mdSpi_mainRspMethodsInterface_def: resultsMap.mdSpi_mainRspMethodsInterface_def.join('\n'),
    mdSpi_rspMethodsInterface_def: resultsMap.mdSpi_rspMethodsInterface_def.join('\n'),
    mdSpi_struct: resultsMap.mdSpi_struct.join('\n'),
    wrapMd_setJsMethods: resultsMap.wrapMd_setJsMethods.join('\n'),
    wrapMd_addRspEvent: resultsMap.wrapMd_addRspEvent.join('\n'),
    wrapMd_reqMethodsImpl_def: '#22',
    wrapMd_mainRspMethodsImpl_def: '#22',
    wrapMd_reqMethodsInterface_def: resultsMap.wrapMd_reqMethodsInterface_def.join('\n'),
    wrapMd_rspMethodsInterface_def: resultsMap.wrapMd_rspMethodsInterface_def.join('\n'),
    traderSpi_rspMethods_call: '#11',
    traderSpi_rspMethodsImpl_def: '#11',
    traderSpi_struct: '#11',
    traderSpi_mainRspMethodsInterface_def: '#11',
    traderSpi_rspMethodsInterface_def: '#11',
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
  reqMethodsMap,
  structMap = {},
  resultsMap = {
    mdSpi_rspMethods_call: [],
    mdSpi_rspMethodsImpl_def: [],
    mdSpi_struct: [],
    mdSpi_mainRspMethodsInterface_def: [],
    mdSpi_rspMethodsInterface_def: [],
    wrapMd_rspMethodsInterface_def: [],
    wrapMd_reqMethodsInterface_def: [],
    wrapMd_setJsMethods: [],
    wrapMd_addRspEvent: [],
    wrapMd_reqMethodsImpl_def: [],
    wrapMd_mainRspMethodsImpl_def: [],
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
          resultsMap.wrapMd_rspMethodsInterface_def.push(`        virtual void Main${funcName}(${originArgsText});`);
          resultsMap.wrapMd_addRspEvent.push(`    m_event.insert("${funcName}");`);
        }
        rspMethodsMap = null;
      }
    }

    if (firstWord === 'class' && words[1] === 'MD_API_EXPORT' && words[2] === 'CThostFtdcMdApi') {
      reqMethodsMap = {};
      continue;
    }

    if (reqMethodsMap) {
      if ((firstWord === 'virtual' || firstWord === 'static') && words[1] && words[2]) {
        let rtnObj = {}, argsObj, funcName;
        if (firstWord === 'static') {
          rtnObj.isStatic = true;
        }
        if (words[1] === 'const') {
          rtnObj.isConst = true;

          if (words[2] === 'char') {
            rtnObj.type = 'object';
            rtnObj.originType = `${words[2]}*`;
          }

          funcName = words[3];
          argsObj = getArgs(words.slice(4));
        }
        else if (/^CThostFtdc/.test(words[1])) {
          rtnObj.type = 'object';
          rtnObj.originType = `${words[1]}*`;
          funcName = words[2];
          argsObj = getArgs(words.slice(3));
        }
        else {
          rtnObj.type = words[1];
          funcName = words[2];
          argsObj = getArgs(words.slice(3));
        }
        reqMethodsMap[funcName] = {
          args: argsObj,
          rtnObj
        };
        continue;
      }
    }
  }
  for (let funcName in reqMethodsMap) {
    let { args, rtnObj } = reqMethodsMap[funcName];
    let codeBody = '';
    console.log('==========================================');
    console.log(funcName);
    console.log('args:\n', args);
    console.log('rtnObj:\n', rtnObj);
    
    // TODO: 主动请求函数的处理
    resultsMap.wrapMd_reqMethodsInterface_def.push(`        static void ${funcName}(const v8::FunctionCallbackInfo<v8::Value>& args);`);
    resultsMap.wrapMd_setJsMethods.push(`    NODE_SET_PROTOTYPE_METHOD(tpl, "${funcName}", ${funcName});`);
    resultsMap.wrapMd_reqMethodsImpl_def.push(`void WrapMd::${funcName}(const FunctionCallbackInfo<Value>& args)`);
    resultsMap.wrapMd_reqMethodsImpl_def.push('{');
    resultsMap.wrapMd_reqMethodsImpl_def.push('    WrapMd* obj = node::ObjectWrap::Unwrap<WrapMd>(args.Holder());');
    resultsMap.wrapMd_reqMethodsImpl_def.push('    Isolate* isolate = args.GetIsolate();');
    resultsMap.wrapMd_reqMethodsImpl_def.push(codeBody);
    resultsMap.wrapMd_reqMethodsImpl_def.push('}');
  }

  return resultsMap;
}


function trim(value) {
  return value.replace(/^\s*/, '').replace(/\s*$/, '');
}

function isChineseCharacters(text) {
  return /[\u4e00-\u9fa5]+/.test(text);
}

function removeBlanks(list) {
  for(let i = list.length - 1; i >= 0; i--) {
    if (/^\s*$/.test(list[i])) {
      list.splice(i, 1);
    }
  }
}


function getArgs(words) {
  let args = [], argObj = {};
  console.log(words)
  while(words.length) {
    let s = words.shift();
    if (/^\s|true|false$/) {
      continue;
    }
    if (s === ';') {
      break;
    }
    if (s === 'const') {
      argObj.isConst = true;
      continue;
    }
    if (argObj.isConst) {
      if (s === 'char' || /^CThostFtdc/.test(s)) {
        argObj.type = 'object';
        argObj.originType = s;
      }
      else {
        argObj.type = s;
        argObj.originType = s;
        argObj.name = s.replace(/^p/, '').replace(/\[|\]/g, '');
        argObj.originName = s;
        args.push(argObj);
        argObj = {};
      }
    }
    else {
      if (!argObj.type) {
        if (s === 'char' || /^CThostFtdc/.test(s)) {
          argObj.type = 'object';
          argObj.originType = s;
        }
        else {
          argObj.type = s;
          argObj.originType = s;
        }
      }
      else {
        argObj.name = s.replace(/^p/, '');
        argObj.originName = s;
        args.push(argObj);
        argObj = {};
      }
    }
  }
  return args;
}

// function splitWords(line) {
//   return line.split(/\s+|[(){}*,=]/);
// }
function splitWords(line) {
  let characters = line.split('');

  let beforeArgCharacters = [], argCharacters;
  for (let i = 0; i < characters.length; i++) {
    let c = characters[i];
    if (c === '(') {
      argCharacters = [];
      continue;
    }
    else if (c === ')') {
      break;
    }
    
    if (argCharacters) {
      argCharacters.push(c);
    }
    else {
      beforeArgCharacters.push(c);
    }
  }


  let { words: beforeArgWords } = composeWord(beforeArgCharacters);
  let { wordsList: argWordsList } = composeWord(argCharacters);

  let rtn = parseRtnStatement(beforeArgWords);
  let args = parseArgsStatement(argWordsList);

  console.log(rtn);
  console.log(args);
  return { rtn, args };
}

function composeWord(characters) {
  let word = '', lastCharacter = '', words = [], wordsList = [];
  characters.forEach((c, i) => {
    switch(c) {
      case ';':
      lastCharacter = c;
      break;
      case '*':
      if (lastCharacter === ' ') {
        words[words.length - 1] += '*';
      }
      lastCharacter = c;
      break;
      case ',':
      if (word) {
        words.push(word);
        word = '';
      }

      if (words.length) {
        wordsList.push(words);
        words = [];
      }
      
      lastCharacter = c;
      break;
      case '=':
      if (lastCharacter !== ' ') {
        words.push(word);
      }
      words.push('=');
      word = '';
      lastCharacter = c;
      break;
      case '"':
      word += '"';
      lastCharacter = c;
      break;
      case '\'':
      word += '\'';
      lastCharacter = c;
      break;
      case ' ':
      if (lastCharacter !== ' ' && word) {
        words.push(word);
        word = '';
      }
      lastCharacter = c;
      break;
      default:
      word += c;
      lastCharacter = c;
    }
  });

  word && words.push(word);
  words.length && wordsList.push(words);
  
  return { words, wordsList };
}

function recognizeType(word) {
  if (word === 'static') {
    return { isStatic: true };
  }
  if (word === 'const') {
    return { isConst: true };
  }
  if (/^void|int|bool|double|float|char|[a-zA-Z]+\*$/.test(word)) {
    return { type: word, isPointer: /\*$/.test(word) };
  }
}

function parseValue(word) {
  if (word === 'true') {
    return { defaultValue: true };
  }

  if (word === 'false') {
    return { defaultValue: false };
  }

  if (word === '""' || word === '\'\'') {
    return { defaultValue: '' };
  }

  if (/^[0-9]+(\.[0-9]+)?$/.test(word)) {
    return { defaultValue: Number(word) };
  }
}

function parseRtnStatement(words) {
  let funcName = words.pop();
  let funcLimiter = words.join(' ');
  let result = {};
  words.forEach(word => {
    let data = recognizeType(word);
    result = Object.assign({}, result, data);
  });

  result.funcName = funcName;
  result.funcLimiter = funcLimiter;

  return result;
}

function parseArgsStatement(wordsList) {
  let args = [];
  wordsList.forEach(words => {
    let newWords = [], value, equalIndex;
    words.forEach((word, i) => {
      if (word !== '=' && (!equalIndex || i < equalIndex)) {
        newWords.push(word);
        return;
      }
      if (word === '=') {
        equalIndex = i;
        return;
      }
      if (i === equalIndex + 1) {
        value = word;
      }
    });
    
    let result = {}, varName = newWords.pop(), varLimiter = newWords.join(' ');;
    
    newWords.forEach(word => {
      let data = recognizeType(word);
      result = Object.assign({}, result, data);
    });

    result.varName = varName;
    result.varLimiter = varLimiter;

    if (value) {
      let valueObj = parseValue(value);
      result.defaultValue = valueObj.defaultValue;
      result.originDefaultValue = value;
    }

    args.push(result);
  });

  return args;
}

// (async () => {
//   await create();
// })();
// create();
let s = 'virtual int SubscribeMarketData(char *ppInstrumentID[], int nCount) = 0;';
splitWords(s);