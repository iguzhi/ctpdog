/////////////////////////////////////////////////////////////////////////
///@system ctp 行情nodejs addon
///@company 慧网基金
///@file WrapMd.h
///@brief js回调接口
///@history
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#ifndef __WRAPMD_H__
#define __WRAPMD_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <set>
#include <map>
#include <string>
// #include <node.h>
// #include <node_object_wrap.h>
#include <napi.h>

#include "comm.h"

#include "ThostFtdcMdSpiI.h"
#include "ctp_node.h"

using namespace std;

namespace md
{
class WrapMd : public CThostFtdcMdSpiI, public node::ObjectWrap
{
    public:
        WrapMd();
        ~WrapMd();

        //wrap主动请求函数
        static void On(const Napi::CallbackInfo& args);
        static void NewInstance(const Napi::CallbackInfo& args);
        static void Init(Napi::Env env);

        static void Init(const Napi::CallbackInfo& args);
        static void Release(const Napi::CallbackInfo& args);
        static void Dispose(const Napi::CallbackInfo& args);
        static void CreateFtdcMdApi(const Napi::CallbackInfo& args);
        static void GetApiVersion(const Napi::CallbackInfo& args);
        static void GetTradingDay(const Napi::CallbackInfo& args);
        static void RegisterFront(const Napi::CallbackInfo& args);
        static void RegisterNameServer(const Napi::CallbackInfo& args);
        static void RegisterFensUserInfo(const Napi::CallbackInfo& args);
        static void ReqUserLogin(const Napi::CallbackInfo& args);
        static void ReqUserLogout(const Napi::CallbackInfo& args);
        static void SubscribeMarketData(const Napi::CallbackInfo& args);
        static void UnSubscribeMarketData(const Napi::CallbackInfo& args);
        static void SubscribeForQuoteRsp(const Napi::CallbackInfo& args);
        static void UnSubscribeForQuoteRsp(const Napi::CallbackInfo& args);
        static void ReqQryMulticastInstrument(const Napi::CallbackInfo& args);

        //回调js部分
        virtual void MainOnFrontConnected();
        virtual void MainOnFrontDisconnected(int nReason);
        virtual void MainOnHeartBeatWarning(int nTimeLapse);
        virtual void MainOnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
        virtual void MainOnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
        virtual void MainOnRspQryMulticastInstrument(CThostFtdcMulticastInstrumentField *pMulticastInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
        virtual void MainOnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
        virtual void MainOnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
        virtual void MainOnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
        virtual void MainOnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
        virtual void MainOnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
        virtual void MainOnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) ;
        virtual void MainOnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) ;

        bool CanCallback(string event)
        {
            if(callback_map.find(event) != callback_map.end()) return true;
            return false;
        }
        void SetCallback(string event, v8::Local<v8::Function>& cb, Napi::Env env)
        {
            callback_map[event].Reset(env, cb);
        }
    private:
        typedef std::map<string, Napi::FunctionReference > __callback_type;
        typedef std::map<string, Napi::FunctionReference >::iterator  __callback_iter_type;
        __callback_type callback_map; //回调js name映射回调处理函数

        static set<string>         m_event;                //可以注册的回调事件
        // static Napi::FunctionReference constructor;           //主动请求函数映射js name

        static void New(const Napi::CallbackInfo& args);
        Napi::Value PkgRspInfo(Napi::Env env, CThostFtdcRspInfoField *pRspInfo)
        {
            if (pRspInfo != NULL)
            {
                Napi::Object jsonInfo = Napi::Object::New(env);
                jsonInfo.Set(Napi::String::New(env, "ErrorID"), Napi::Int32::New(env, pRspInfo->ErrorID));
                jsonInfo.Set(Napi::String::New(env, "ErrorMsg"), Napi::String::New(env, CSFunction::GBK2UTF8(pRspInfo->ErrorMsg).c_str()));
                return jsonInfo;
            }
            else
            {
                return  Napi::Value::New(env, Napi::Undefined(env));
            }
        }
};
}

#endif






