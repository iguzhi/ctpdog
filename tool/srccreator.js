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


function trim(value) {
  return value.replace(/^\s*/, '').replace(/\s*$/, '');
}

exports.create = create;

create()
