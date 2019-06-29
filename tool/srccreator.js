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

  rsp = rsp.replace(/[/]{2,}[^\r\n]*/g, '');
  let lines = rsp.split(spliter);

  // 删除空行
  removeBlanks(lines);

  let structName, fields = [], structMap = {};

  for (let i = 0; i < lines.length - 1; i++) {
    let line = lines[i];
    line = trim(line);
    let words = line.split(/\s+|;/);
    removeBlanks(words);
    let firstWord = words[0];
    
    if (firstWord === 'struct') {
      structName = words[1];
      continue;
    }
    if (structName) {
      if (/^\s*\}\s*;?/.test(firstWord)) {
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

  rsp = rsp.replace(/[/]{2,}[^\r\n]*/g, '');
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
    
    if (startWith(line, 'class CThostFtdcMdSpi')) {
      rspMethodsMap = {};
      continue;
    }
    if (rspMethodsMap) {
      if (isFuncDefine(line)) {
        let { rtn, args } = splitWords(line);
        rspMethodsMap[rtn.funcName] = { rtn, args };
        continue;
      }

      if (isClassDefine(line)) {
        for (let funcName in rspMethodsMap) {
          let { args } = rspMethodsMap[funcName];
          let argsText = '', originArgsText = '', text = '', structText = '';

          args.forEach((item, i) => {
            if (i) {
              argsText += ', ';
              originArgsText += ', ';
              text += '\n';
            }
            if (item.isPointer) {
              if (item.fixedVarName !== 'RspInfo') {
                argsText += `&task->data.${item.fixedVarName}`;
                text += `  t->data.${item.fixedVarName} = *${item.originVarName};`;
                let fixedVarLimiter = item.varLimiter.replace(/\*$/, '');
                if (!structMap[fixedVarLimiter]) {
                  structMap[fixedVarLimiter] = true;
                  structText += `        ${fixedVarLimiter} ${item.fixedVarName};`;
                }
              }
              else {
                argsText += `&task->${item.fixedVarName}`;
                text += `  t->${item.fixedVarName} = *${item.originVarName};`;
              }
              originArgsText += `${item.varLimiter} ${item.originVarName}`;
            }
            else {
              argsText += `task->${item.fixedVarName}`;
              originArgsText += `${item.varLimiter} ${item.originVarName}`;
              text += `  t->${item.fixedVarName} = ${item.fixedVarName};`
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

    if (startWith(line, 'class MD_API_EXPORT CThostFtdcMdApi')) {
      reqMethodsMap = {};
      continue;
    }

    if (reqMethodsMap) {
      console.log(line)
      if (isFuncDefine(line)) {
        let { rtn, args } = splitWords(line);
        reqMethodsMap[rtn.funcName] = { rtn, args };
        continue;
      }
    }
  }
  for (let funcName in reqMethodsMap) {
    // console.log(funcName)
    let { args, rtn } = reqMethodsMap[funcName];
    let codeBody = '';
    // console.log('==========================================');
    // console.log(funcName);
    // console.log('args:\n', args);
    // console.log('rtn:\n', rtn);
    
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

function startWith(line, s) {
  return line.indexOf(s) !== -1;
}

function isStructDefine(line) {
  return /^\s*struct\s+/.test(line);
}

function isClassDefine(line) {
  return /^\s*class\s+/.test(line);
}

function isFuncDefine(line) {
  return /^\s*[a-zA-Z][a-zA-Z0-9]*\*?\s+\*?([a-zA-Z][a-zA-Z0-9*]*\*?\s*\*?)+\([^.]*\)/.test(line);
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


// function splitWords(line) {
//   return line.split(/\s+|[(){}*,=]/);
// }
function splitWords(line) {
  line = trim(line);
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

  console.log(beforeArgCharacters.join(''))

  let { words: beforeArgWords } = composeWord(beforeArgCharacters);
  let { wordsList: argWordsList } = composeWord(argCharacters);

  let rtn = parseRtnStatement(beforeArgWords);
  let args = parseArgsStatement(argWordsList);

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
  result.fixedFuncLimiter = fixFuncLimiter(funcLimiter);

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
    let { fixedVarName, originVarName } = fixVarName(varName);
    result.fixedVarName = fixedVarName;
    result.originVarName = originVarName;
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

function fixFuncLimiter(funcLimiter) {
  return funcLimiter.replace(/^virtual\s+/, '');
}

function fixVarName(varName) {
  let characters = varName.split('');
  let originVarName = varName, fixedVarName = varName;
  
  if (characters[characters.length - 1] === ']' && characters[characters.length - 2] === '[') {
    characters.splice(characters.length - 2, 2);

    originVarName = characters.join('');

    if (characters[0] === 'p' && characters[1] === 'p' && characters[2].toUpperCase() === characters[2]) {
      characters.splice(0, 1);
      // fixedVarName = characters.join('');
    }
  }

  if (characters[0] === 'p' && characters[1].toUpperCase() === characters[1]) {
    characters.splice(0, 1);
  }

  fixedVarName = characters.join('');
  
  return { fixedVarName, originVarName };
}

// (async () => {
//   await create();
// })();
create();
// let s = 'virtual test()';
// splitWords(s);  set_obj(obj, "GuarantRatio", &p->GuarantRatio);
// let s = 'static CThostFtdcMdApi *CreateFtdcMdApi(const char *pszFlowPath = "", const bool bIsUsingUdp=false, const bool bIsMulticast=false);'

// console.log(isFuncDefine(s))