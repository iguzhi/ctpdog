//*******************************************************************************
//  *Copyright(C),2018, invesmart.cn                                             
//  *FileName:  WrapTd.h                                                       
//  *Author: 	dreamyzhang                                                      
//  *Version:   v2.1                                                             
//  *Date:  	2018-05-30                                                       
//  *Description:  invoked by js                                                 
//*******************************************************************************

#ifndef __WRAPTD_H__
#define __WRAPTD_H__

//#include <sys/socket.h>
//#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
//#include <pthread.h>
#include <map>
#include <node.h>
#include <node_object_wrap.h>

#include <set>

#include "comm.h"

#include "ThostFtdcTraderSpiI.h"
#include "ctp_node.h"


using namespace std;

namespace td
{
    class WrapTd : public CThostFtdcTraderSpiI, public node::ObjectWrap
    {
        public:
            WrapTd();
            virtual ~WrapTd();

            static void On(const v8::FunctionCallbackInfo<v8::Value>& args);
            static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args);
            static void Init(v8::Isolate* isolate);

            ///////////////////////////////主动请求函数start//////////////////////////////////////////////////
            //参数传递以对象的形式
            // AUTOCODE
            #22
            /////////////////////////////主动请求函数end/////////////////////////////////////////////////////

            /////////////////////////ctp线程回调过来的///////////////////////////////////////////////////
            // AUTOCODE
            #22

            bool CanCallback(string event)
            {
                if(callback_map.find(event) != callback_map.end()) return true;
                return false;
            }
            void SetCallback(string event, v8::Local<v8::Function>& cb, v8::Isolate* isolate)
            {
                callback_map[event].Reset(isolate, cb);
            }

        private:
            typedef std::map<string, v8::Persistent<v8::Function> > __callback_type;
            typedef std::map<string, v8::Persistent<v8::Function> >::iterator  __callback_iter_type;
        
            __callback_type callback_map; //回调js name映射回调处理函数

            static set<string>         m_event;                //可以注册的回调事件
            static v8::Persistent<v8::Function> constructor;           //主动请求函数映射js name

            static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
            v8::Local<v8::Value> PkgRspInfo(CThostFtdcRspInfoField *pRspInfo) 
            {
                v8::Isolate* isolate = v8::Isolate::GetCurrent();
                if (pRspInfo != NULL) 
                { 
                    //printf("-----%d %s--------\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
                    v8::Local<v8::Object> jsonInfo = v8::Object::New(isolate);                                       
                    jsonInfo->Set(v8::String::NewFromUtf8(isolate, "ErrorID"), v8::Int32::New(isolate, pRspInfo->ErrorID));   
                    jsonInfo->Set(v8::String::NewFromUtf8(isolate, "ErrorMsg"), v8::String::NewFromUtf8(isolate, CSFunction::GBK2UTF8(pRspInfo->ErrorMsg).c_str()));
                    return jsonInfo; 
                } 
                else
                {
                    return  v8::Local<v8::Value>::New(isolate, v8::Undefined(isolate));                                       
                }     
            }

    };
}


#endif



