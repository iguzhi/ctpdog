'use strict';

const path = require('path');
const fs = require('fs');
const os = require('os');

const spliter = os === 'win32' ? '\r\n' : '\n';

function create() {
  let filepath_ThostFtdcUserApiDataType = path.join(__dirname, '../linux_api_trade_6.3.6/ThostFtdcUserApiDataType.h');

  fs.readFile(filepath_ThostFtdcUserApiDataType, 'utf8', (err, data) => {
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

    let text = "'use strict';\n";
    let description;
    let inDict = false;

    for (let i = 0; i < lines.length - 1; i++) {
      let line = lines[i];
      let nextLine = lines[i + 1];
      
      if (/^TFtdc/.test(line)) {
        if (/[\u4e00-\u9fa5]+/.test(nextLine)) {
          description = trim(nextLine);
          line = line.replace(/^TFtdc/, '').replace('是一个', ' ');
          let arr = line.split(/\s/);
          text += "\n// " + arr[1] + "\n";
          text += "exports." + arr[0] + " = {";
          // i += 1;
          inDict = true;
        }
        else {
          description = '';
        }
      }
      else if (inDict) {
        if (description && /^#define\s/.test(line)) {
          let words = line.split(/\s/);
          if (words.length === 3) {
            let value = words[words.length - 1];
            value = trim(value);
            let key = words[words.length - 2];
            let keys = key.split(/_/);
            key = keys[keys.length - 1];
            key = trim(key);
            text += "\n  // " + description + "\n";
            text += "  " + key + ": " + value + ",";
            description = '';
          }
        }
        else if (/[\u4e00-\u9fa5]+/.test(line)) {
          description = trim(line);
        }
        else if(/^typedef/.test(line)) {
          text += "\n};\n";
          inDict = false;
        }
      }
    }

    fs.writeFile(path.join(__dirname, '../lib/dict.js'), text, function(err) {
      if (err) throw err;
      console.log('generate UserApiDataType Dict File success!');
    });
  });
}


function trim(value) {
  return value.replace(/^\s*/, '').replace(/\s*$/, '');
}

exports.create = create;
