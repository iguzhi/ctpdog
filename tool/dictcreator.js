'use strict';

const path = require('path');
const fs = require('fs');
const os = require('os');

const spliter = os === 'win32' ? '\r\n' : '\n';

function create() {
  let filepath_ThostFtdcUserApiDataType = path.join(__dirname, '../api/tradeapi_linux64/ThostFtdcUserApiDataType.h');

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

    let dictText = "'use strict';\n";
    let enumText = "'use strict';\n\nconst Enum = require('microenum');\n";
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

    fs.writeFile(path.join(__dirname, '../lib/dict.js'), dictText, function(err) {
      if (err) throw err;
      console.log('generate UserApiDataType Dict File success!');
    });

    fs.writeFile(path.join(__dirname, '../lib/enum.js'), enumText, function(err) {
      if (err) throw err;
      console.log('generate UserApiDataType Enum File success!');
    });
  });
}


function trim(value) {
  return value.replace(/^\s*/, '').replace(/\s*$/, '');
}

exports.create = create;
