#ifndef __NODE_CTP__
#define __NODE_CTP__

#include <v8.h>
#include <string>
#include "ThostFtdcUserApiStruct.h"

using  v8::Local;
using  v8::Isolate;
using  v8::Object;
using  v8::String;
using  v8::Exception;
using  v8::Value;

class CSFunction
{

public:

#ifdef WIN32
#include <windows.h>
static string GBK2UTF8(string strGBK)  
{  
    string strOutUTF8 = "";  
    WCHAR * str1;  
    int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);  
    str1 = new WCHAR[n];  
    MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);  
    n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);  
    char * str2 = new char[n];  
    WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);  
    strOutUTF8 = str2;  
    delete[]str1;  
    str1 = NULL;  
    delete[]str2;  
    str2 = NULL;  
    return strOutUTF8;  
}  
#else
static string GBK2UTF8(string src)
{
    if(src.length() < 1) return "";
    size_t outlen = 4*src.length()+1;
    char* pout = new char[outlen];
    memset(pout, 0,  outlen);
    char* dest = pout;

    char * pin = (char*)src.c_str();
    size_t inlen = src.length();

    iconv_t conv = iconv_open("UTF-8", "GB2312");
    if(conv == 0) return "";
    iconv(conv, &pin, &inlen, &pout, &outlen);
    iconv_close(conv);
    
    string tmp = dest;
    delete dest;
    return tmp;
}
#endif

static void set_struct(Local<Object>& obj, const char* key, void* dest, int len)
{
    v8::Isolate* isolate  = v8::Isolate::GetCurrent();
    Local<Value> v = obj->Get(v8::String::NewFromUtf8(isolate, key));
    if (v->IsUndefined())
    {
        memset(dest, 0, len);
        //isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, string("Wrong arguments->") + key)));
        return ;
    }
    String::Utf8Value t(v);
    strncpy((char*)dest, (char*)*t, len);
    if(len>1) ((char*)dest)[len-1] = '0';
}

static void set_struct(Local<Object>& obj, const char* key, char* dest, int len)
{
    v8::Isolate* isolate  = v8::Isolate::GetCurrent();
    Local<Value> v = obj->Get(v8::String::NewFromUtf8(isolate, key));
    if (v->IsUndefined())
    {
        memset(dest, 0, len);
        //isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, string("Wrong arguments->") + key)));
        return ;
    }
    String::Utf8Value t(v);
    strncpy(dest, (char*)*t, len);
    if(len>1) dest[len-1] = '0';
}

static void set_struct(Local<Object>& obj, const char* key, int* dest, int len=0)
{
    v8::Isolate* isolate  = v8::Isolate::GetCurrent();
    Local<Value> v = obj->Get(v8::String::NewFromUtf8(isolate, key));
    if (v->IsUndefined())
    {
        dest = 0;
        //isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, string("Wrong arguments->") + key)));
        return ;
    }
    *dest = v->Int32Value();
}

static void set_struct(Local<Object>& obj, const char* key, double* dest, int len=0)
{
    v8::Isolate* isolate  = v8::Isolate::GetCurrent();
    Local<Value> v = obj->Get(v8::String::NewFromUtf8(isolate, key));
    if (v->IsUndefined() || !v->IsNumber())
    {
        dest = 0;
        //isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, string("Wrong arguments->") + key)));
        return ;
    }
    *dest = v->NumberValue();
}

static void set_struct(Local<Object>& obj, const char* key, * dest, int len=0)
{
    v8::Isolate* isolate  = v8::Isolate::GetCurrent();
    Local<Value> v = obj->Get(v8::String::NewFromUtf8(isolate, key));
    if (v->IsUndefined() || !v->IsNumber())
    {
        dest = 0;
        //isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, string("Wrong arguments->") + key)));
        return ;
    }
    *dest = (float)v->NumberValue();
}


//处理ctp定义的字符数组指针类型
static void set_obj(v8::Local<v8::Object>& obj, const char* key, void* v)
{
    string u = GBK2UTF8((char*)v);
    obj->Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), key), v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), u.c_str()));
}


static void set_obj(v8::Local<v8::Object>& obj, const char* key, char* v)
{
    char s[2]= {0};
    s[0] = *v;
    obj->Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), key), v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), s));
}

static void set_obj(v8::Local<v8::Object>& obj, const char* key, int* v)
{
    obj->Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), key), v8::Int32::New(v8::Isolate::GetCurrent(), *v));
}

static void set_obj(v8::Local<v8::Object>& obj, const char* key, short int* v)
{
    obj->Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), key), v8::Int32::New(v8::Isolate::GetCurrent(), (int)*v));
}

static void set_obj(v8::Local<v8::Object>& obj, const char* key, float* v)
{
    obj->Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), key), v8::Number::New(v8::Isolate::GetCurrent(), *v));
}

static void set_obj(v8::Local<v8::Object>& obj, const char* key, double* v)
{
    obj->Set(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), key), v8::Number::New(v8::Isolate::GetCurrent(), *v));
}

// AUTOCODE: set methods

static void set_obj(Local<Object>& obj, CThostFtdcDisseminationField *p)
{
  set_obj(obj, "SequenceSeries", &p->SequenceSeries);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqUserLoginField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "UserProductInfo", &p->UserProductInfo);
  set_obj(obj, "InterfaceProductInfo", &p->InterfaceProductInfo);
  set_obj(obj, "ProtocolInfo", &p->ProtocolInfo);
  set_obj(obj, "MacAddress", &p->MacAddress);
  set_obj(obj, "OneTimePassword", &p->OneTimePassword);
  set_obj(obj, "ClientIPAddress", &p->ClientIPAddress);
  set_obj(obj, "LoginRemark", &p->LoginRemark);
  set_obj(obj, "ClientIPPort", &p->ClientIPPort);
}
static void set_obj(Local<Object>& obj, CThostFtdcRspUserLoginField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "LoginTime", &p->LoginTime);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "SystemName", &p->SystemName);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "MaxOrderRef", &p->MaxOrderRef);
  set_obj(obj, "SHFETime", &p->SHFETime);
  set_obj(obj, "DCETime", &p->DCETime);
  set_obj(obj, "CZCETime", &p->CZCETime);
  set_obj(obj, "FFEXTime", &p->FFEXTime);
  set_obj(obj, "INETime", &p->INETime);
}
static void set_obj(Local<Object>& obj, CThostFtdcUserLogoutField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
}
static void set_obj(Local<Object>& obj, CThostFtdcForceUserLogoutField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqAuthenticateField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "UserProductInfo", &p->UserProductInfo);
  set_obj(obj, "AuthCode", &p->AuthCode);
  set_obj(obj, "AppID", &p->AppID);
}
static void set_obj(Local<Object>& obj, CThostFtdcRspAuthenticateField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "UserProductInfo", &p->UserProductInfo);
  set_obj(obj, "AppID", &p->AppID);
  set_obj(obj, "AppType", &p->AppType);
}
static void set_obj(Local<Object>& obj, CThostFtdcAuthenticationInfoField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "UserProductInfo", &p->UserProductInfo);
  set_obj(obj, "AuthInfo", &p->AuthInfo);
  set_obj(obj, "IsResult", &p->IsResult);
  set_obj(obj, "AppID", &p->AppID);
  set_obj(obj, "AppType", &p->AppType);
}
static void set_obj(Local<Object>& obj, CThostFtdcRspUserLogin2Field *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "LoginTime", &p->LoginTime);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "SystemName", &p->SystemName);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "MaxOrderRef", &p->MaxOrderRef);
  set_obj(obj, "SHFETime", &p->SHFETime);
  set_obj(obj, "DCETime", &p->DCETime);
  set_obj(obj, "CZCETime", &p->CZCETime);
  set_obj(obj, "FFEXTime", &p->FFEXTime);
  set_obj(obj, "INETime", &p->INETime);
  set_obj(obj, "RandomString", &p->RandomString);
}
static void set_obj(Local<Object>& obj, CThostFtdcTransferHeaderField *p)
{
  set_obj(obj, "Version", &p->Version);
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "TradeSerial", &p->TradeSerial);
  set_obj(obj, "FutureID", &p->FutureID);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBrchID", &p->BankBrchID);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "RecordNum", &p->RecordNum);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "RequestID", &p->RequestID);
}
static void set_obj(Local<Object>& obj, CThostFtdcTransferBankToFutureReqField *p)
{
  set_obj(obj, "FutureAccount", &p->FutureAccount);
  set_obj(obj, "FuturePwdFlag", &p->FuturePwdFlag);
  set_obj(obj, "FutureAccPwd", &p->FutureAccPwd);
  set_obj(obj, "TradeAmt", &p->TradeAmt);
  set_obj(obj, "CustFee", &p->CustFee);
  set_obj(obj, "CurrencyCode", &p->CurrencyCode);
}
static void set_obj(Local<Object>& obj, CThostFtdcTransferBankToFutureRspField *p)
{
  set_obj(obj, "RetCode", &p->RetCode);
  set_obj(obj, "RetInfo", &p->RetInfo);
  set_obj(obj, "FutureAccount", &p->FutureAccount);
  set_obj(obj, "TradeAmt", &p->TradeAmt);
  set_obj(obj, "CustFee", &p->CustFee);
  set_obj(obj, "CurrencyCode", &p->CurrencyCode);
}
static void set_obj(Local<Object>& obj, CThostFtdcTransferFutureToBankReqField *p)
{
  set_obj(obj, "FutureAccount", &p->FutureAccount);
  set_obj(obj, "FuturePwdFlag", &p->FuturePwdFlag);
  set_obj(obj, "FutureAccPwd", &p->FutureAccPwd);
  set_obj(obj, "TradeAmt", &p->TradeAmt);
  set_obj(obj, "CustFee", &p->CustFee);
  set_obj(obj, "CurrencyCode", &p->CurrencyCode);
}
static void set_obj(Local<Object>& obj, CThostFtdcTransferFutureToBankRspField *p)
{
  set_obj(obj, "RetCode", &p->RetCode);
  set_obj(obj, "RetInfo", &p->RetInfo);
  set_obj(obj, "FutureAccount", &p->FutureAccount);
  set_obj(obj, "TradeAmt", &p->TradeAmt);
  set_obj(obj, "CustFee", &p->CustFee);
  set_obj(obj, "CurrencyCode", &p->CurrencyCode);
}
static void set_obj(Local<Object>& obj, CThostFtdcTransferQryBankReqField *p)
{
  set_obj(obj, "FutureAccount", &p->FutureAccount);
  set_obj(obj, "FuturePwdFlag", &p->FuturePwdFlag);
  set_obj(obj, "FutureAccPwd", &p->FutureAccPwd);
  set_obj(obj, "CurrencyCode", &p->CurrencyCode);
}
static void set_obj(Local<Object>& obj, CThostFtdcTransferQryBankRspField *p)
{
  set_obj(obj, "RetCode", &p->RetCode);
  set_obj(obj, "RetInfo", &p->RetInfo);
  set_obj(obj, "FutureAccount", &p->FutureAccount);
  set_obj(obj, "TradeAmt", &p->TradeAmt);
  set_obj(obj, "UseAmt", &p->UseAmt);
  set_obj(obj, "FetchAmt", &p->FetchAmt);
  set_obj(obj, "CurrencyCode", &p->CurrencyCode);
}
static void set_obj(Local<Object>& obj, CThostFtdcTransferQryDetailReqField *p)
{
  set_obj(obj, "FutureAccount", &p->FutureAccount);
}
static void set_obj(Local<Object>& obj, CThostFtdcTransferQryDetailRspField *p)
{
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "FutureSerial", &p->FutureSerial);
  set_obj(obj, "FutureID", &p->FutureID);
  set_obj(obj, "FutureAccount", &p->FutureAccount);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBrchID", &p->BankBrchID);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "CertCode", &p->CertCode);
  set_obj(obj, "CurrencyCode", &p->CurrencyCode);
  set_obj(obj, "TxAmount", &p->TxAmount);
  set_obj(obj, "Flag", &p->Flag);
}
static void set_obj(Local<Object>& obj, CThostFtdcRspInfoField *p)
{
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ExchangeName", &p->ExchangeName);
  set_obj(obj, "ExchangeProperty", &p->ExchangeProperty);
}
static void set_obj(Local<Object>& obj, CThostFtdcProductField *p)
{
  set_obj(obj, "ProductID", &p->ProductID);
  set_obj(obj, "ProductName", &p->ProductName);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ProductClass", &p->ProductClass);
  set_obj(obj, "VolumeMultiple", &p->VolumeMultiple);
  set_obj(obj, "PriceTick", &p->PriceTick);
  set_obj(obj, "MaxMarketOrderVolume", &p->MaxMarketOrderVolume);
  set_obj(obj, "MinMarketOrderVolume", &p->MinMarketOrderVolume);
  set_obj(obj, "MaxLimitOrderVolume", &p->MaxLimitOrderVolume);
  set_obj(obj, "MinLimitOrderVolume", &p->MinLimitOrderVolume);
  set_obj(obj, "PositionType", &p->PositionType);
  set_obj(obj, "PositionDateType", &p->PositionDateType);
  set_obj(obj, "CloseDealType", &p->CloseDealType);
  set_obj(obj, "TradeCurrencyID", &p->TradeCurrencyID);
  set_obj(obj, "MortgageFundUseRange", &p->MortgageFundUseRange);
  set_obj(obj, "ExchangeProductID", &p->ExchangeProductID);
  set_obj(obj, "UnderlyingMultiple", &p->UnderlyingMultiple);
}
static void set_obj(Local<Object>& obj, CThostFtdcInstrumentField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InstrumentName", &p->InstrumentName);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "ProductID", &p->ProductID);
  set_obj(obj, "ProductClass", &p->ProductClass);
  set_obj(obj, "DeliveryYear", &p->DeliveryYear);
  set_obj(obj, "DeliveryMonth", &p->DeliveryMonth);
  set_obj(obj, "MaxMarketOrderVolume", &p->MaxMarketOrderVolume);
  set_obj(obj, "MinMarketOrderVolume", &p->MinMarketOrderVolume);
  set_obj(obj, "MaxLimitOrderVolume", &p->MaxLimitOrderVolume);
  set_obj(obj, "MinLimitOrderVolume", &p->MinLimitOrderVolume);
  set_obj(obj, "VolumeMultiple", &p->VolumeMultiple);
  set_obj(obj, "PriceTick", &p->PriceTick);
  set_obj(obj, "CreateDate", &p->CreateDate);
  set_obj(obj, "OpenDate", &p->OpenDate);
  set_obj(obj, "ExpireDate", &p->ExpireDate);
  set_obj(obj, "StartDelivDate", &p->StartDelivDate);
  set_obj(obj, "EndDelivDate", &p->EndDelivDate);
  set_obj(obj, "InstLifePhase", &p->InstLifePhase);
  set_obj(obj, "IsTrading", &p->IsTrading);
  set_obj(obj, "PositionType", &p->PositionType);
  set_obj(obj, "PositionDateType", &p->PositionDateType);
  set_obj(obj, "LongMarginRatio", &p->LongMarginRatio);
  set_obj(obj, "ShortMarginRatio", &p->ShortMarginRatio);
  set_obj(obj, "MaxMarginSideAlgorithm", &p->MaxMarginSideAlgorithm);
  set_obj(obj, "UnderlyingInstrID", &p->UnderlyingInstrID);
  set_obj(obj, "StrikePrice", &p->StrikePrice);
  set_obj(obj, "OptionsType", &p->OptionsType);
  set_obj(obj, "UnderlyingMultiple", &p->UnderlyingMultiple);
  set_obj(obj, "CombinationType", &p->CombinationType);
}
static void set_obj(Local<Object>& obj, CThostFtdcBrokerField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerAbbr", &p->BrokerAbbr);
  set_obj(obj, "BrokerName", &p->BrokerName);
  set_obj(obj, "IsActive", &p->IsActive);
}
static void set_obj(Local<Object>& obj, CThostFtdcTraderField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "InstallCount", &p->InstallCount);
  set_obj(obj, "BrokerID", &p->BrokerID);
}
static void set_obj(Local<Object>& obj, CThostFtdcInvestorField *p)
{
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorGroupID", &p->InvestorGroupID);
  set_obj(obj, "InvestorName", &p->InvestorName);
  set_obj(obj, "IdentifiedCardType", &p->IdentifiedCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "IsActive", &p->IsActive);
  set_obj(obj, "Telephone", &p->Telephone);
  set_obj(obj, "Address", &p->Address);
  set_obj(obj, "OpenDate", &p->OpenDate);
  set_obj(obj, "Mobile", &p->Mobile);
  set_obj(obj, "CommModelID", &p->CommModelID);
  set_obj(obj, "MarginModelID", &p->MarginModelID);
}
static void set_obj(Local<Object>& obj, CThostFtdcTradingCodeField *p)
{
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "IsActive", &p->IsActive);
  set_obj(obj, "ClientIDType", &p->ClientIDType);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "BizType", &p->BizType);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcPartBrokerField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "IsActive", &p->IsActive);
}
static void set_obj(Local<Object>& obj, CThostFtdcSuperUserField *p)
{
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "UserName", &p->UserName);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "IsActive", &p->IsActive);
}
static void set_obj(Local<Object>& obj, CThostFtdcSuperUserFunctionField *p)
{
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "FunctionCode", &p->FunctionCode);
}
static void set_obj(Local<Object>& obj, CThostFtdcInvestorGroupField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorGroupID", &p->InvestorGroupID);
  set_obj(obj, "InvestorGroupName", &p->InvestorGroupName);
}
static void set_obj(Local<Object>& obj, CThostFtdcTradingAccountField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "PreMortgage", &p->PreMortgage);
  set_obj(obj, "PreCredit", &p->PreCredit);
  set_obj(obj, "PreDeposit", &p->PreDeposit);
  set_obj(obj, "PreBalance", &p->PreBalance);
  set_obj(obj, "PreMargin", &p->PreMargin);
  set_obj(obj, "InterestBase", &p->InterestBase);
  set_obj(obj, "Interest", &p->Interest);
  set_obj(obj, "Deposit", &p->Deposit);
  set_obj(obj, "Withdraw", &p->Withdraw);
  set_obj(obj, "FrozenMargin", &p->FrozenMargin);
  set_obj(obj, "FrozenCash", &p->FrozenCash);
  set_obj(obj, "FrozenCommission", &p->FrozenCommission);
  set_obj(obj, "CurrMargin", &p->CurrMargin);
  set_obj(obj, "CashIn", &p->CashIn);
  set_obj(obj, "Commission", &p->Commission);
  set_obj(obj, "CloseProfit", &p->CloseProfit);
  set_obj(obj, "PositionProfit", &p->PositionProfit);
  set_obj(obj, "Balance", &p->Balance);
  set_obj(obj, "Available", &p->Available);
  set_obj(obj, "WithdrawQuota", &p->WithdrawQuota);
  set_obj(obj, "Reserve", &p->Reserve);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "Credit", &p->Credit);
  set_obj(obj, "Mortgage", &p->Mortgage);
  set_obj(obj, "ExchangeMargin", &p->ExchangeMargin);
  set_obj(obj, "DeliveryMargin", &p->DeliveryMargin);
  set_obj(obj, "ExchangeDeliveryMargin", &p->ExchangeDeliveryMargin);
  set_obj(obj, "ReserveBalance", &p->ReserveBalance);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "PreFundMortgageIn", &p->PreFundMortgageIn);
  set_obj(obj, "PreFundMortgageOut", &p->PreFundMortgageOut);
  set_obj(obj, "FundMortgageIn", &p->FundMortgageIn);
  set_obj(obj, "FundMortgageOut", &p->FundMortgageOut);
  set_obj(obj, "FundMortgageAvailable", &p->FundMortgageAvailable);
  set_obj(obj, "MortgageableFund", &p->MortgageableFund);
  set_obj(obj, "SpecProductMargin", &p->SpecProductMargin);
  set_obj(obj, "SpecProductFrozenMargin", &p->SpecProductFrozenMargin);
  set_obj(obj, "SpecProductCommission", &p->SpecProductCommission);
  set_obj(obj, "SpecProductFrozenCommission", &p->SpecProductFrozenCommission);
  set_obj(obj, "SpecProductPositionProfit", &p->SpecProductPositionProfit);
  set_obj(obj, "SpecProductCloseProfit", &p->SpecProductCloseProfit);
  set_obj(obj, "SpecProductPositionProfitByAlg", &p->SpecProductPositionProfitByAlg);
  set_obj(obj, "SpecProductExchangeMargin", &p->SpecProductExchangeMargin);
  set_obj(obj, "BizType", &p->BizType);
  set_obj(obj, "FrozenSwap", &p->FrozenSwap);
  set_obj(obj, "RemainSwap", &p->RemainSwap);
}
static void set_obj(Local<Object>& obj, CThostFtdcInvestorPositionField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "PosiDirection", &p->PosiDirection);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "PositionDate", &p->PositionDate);
  set_obj(obj, "YdPosition", &p->YdPosition);
  set_obj(obj, "Position", &p->Position);
  set_obj(obj, "LongFrozen", &p->LongFrozen);
  set_obj(obj, "ShortFrozen", &p->ShortFrozen);
  set_obj(obj, "LongFrozenAmount", &p->LongFrozenAmount);
  set_obj(obj, "ShortFrozenAmount", &p->ShortFrozenAmount);
  set_obj(obj, "OpenVolume", &p->OpenVolume);
  set_obj(obj, "CloseVolume", &p->CloseVolume);
  set_obj(obj, "OpenAmount", &p->OpenAmount);
  set_obj(obj, "CloseAmount", &p->CloseAmount);
  set_obj(obj, "PositionCost", &p->PositionCost);
  set_obj(obj, "PreMargin", &p->PreMargin);
  set_obj(obj, "UseMargin", &p->UseMargin);
  set_obj(obj, "FrozenMargin", &p->FrozenMargin);
  set_obj(obj, "FrozenCash", &p->FrozenCash);
  set_obj(obj, "FrozenCommission", &p->FrozenCommission);
  set_obj(obj, "CashIn", &p->CashIn);
  set_obj(obj, "Commission", &p->Commission);
  set_obj(obj, "CloseProfit", &p->CloseProfit);
  set_obj(obj, "PositionProfit", &p->PositionProfit);
  set_obj(obj, "PreSettlementPrice", &p->PreSettlementPrice);
  set_obj(obj, "SettlementPrice", &p->SettlementPrice);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "OpenCost", &p->OpenCost);
  set_obj(obj, "ExchangeMargin", &p->ExchangeMargin);
  set_obj(obj, "CombPosition", &p->CombPosition);
  set_obj(obj, "CombLongFrozen", &p->CombLongFrozen);
  set_obj(obj, "CombShortFrozen", &p->CombShortFrozen);
  set_obj(obj, "CloseProfitByDate", &p->CloseProfitByDate);
  set_obj(obj, "CloseProfitByTrade", &p->CloseProfitByTrade);
  set_obj(obj, "TodayPosition", &p->TodayPosition);
  set_obj(obj, "MarginRateByMoney", &p->MarginRateByMoney);
  set_obj(obj, "MarginRateByVolume", &p->MarginRateByVolume);
  set_obj(obj, "StrikeFrozen", &p->StrikeFrozen);
  set_obj(obj, "StrikeFrozenAmount", &p->StrikeFrozenAmount);
  set_obj(obj, "AbandonFrozen", &p->AbandonFrozen);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "YdStrikeFrozen", &p->YdStrikeFrozen);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "PositionCostOffset", &p->PositionCostOffset);
}
static void set_obj(Local<Object>& obj, CThostFtdcInstrumentMarginRateField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "LongMarginRatioByMoney", &p->LongMarginRatioByMoney);
  set_obj(obj, "LongMarginRatioByVolume", &p->LongMarginRatioByVolume);
  set_obj(obj, "ShortMarginRatioByMoney", &p->ShortMarginRatioByMoney);
  set_obj(obj, "ShortMarginRatioByVolume", &p->ShortMarginRatioByVolume);
  set_obj(obj, "IsRelative", &p->IsRelative);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcInstrumentCommissionRateField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "OpenRatioByMoney", &p->OpenRatioByMoney);
  set_obj(obj, "OpenRatioByVolume", &p->OpenRatioByVolume);
  set_obj(obj, "CloseRatioByMoney", &p->CloseRatioByMoney);
  set_obj(obj, "CloseRatioByVolume", &p->CloseRatioByVolume);
  set_obj(obj, "CloseTodayRatioByMoney", &p->CloseTodayRatioByMoney);
  set_obj(obj, "CloseTodayRatioByVolume", &p->CloseTodayRatioByVolume);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "BizType", &p->BizType);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcDepthMarketDataField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "LastPrice", &p->LastPrice);
  set_obj(obj, "PreSettlementPrice", &p->PreSettlementPrice);
  set_obj(obj, "PreClosePrice", &p->PreClosePrice);
  set_obj(obj, "PreOpenInterest", &p->PreOpenInterest);
  set_obj(obj, "OpenPrice", &p->OpenPrice);
  set_obj(obj, "HighestPrice", &p->HighestPrice);
  set_obj(obj, "LowestPrice", &p->LowestPrice);
  set_obj(obj, "Volume", &p->Volume);
  set_obj(obj, "Turnover", &p->Turnover);
  set_obj(obj, "OpenInterest", &p->OpenInterest);
  set_obj(obj, "ClosePrice", &p->ClosePrice);
  set_obj(obj, "SettlementPrice", &p->SettlementPrice);
  set_obj(obj, "UpperLimitPrice", &p->UpperLimitPrice);
  set_obj(obj, "LowerLimitPrice", &p->LowerLimitPrice);
  set_obj(obj, "PreDelta", &p->PreDelta);
  set_obj(obj, "CurrDelta", &p->CurrDelta);
  set_obj(obj, "UpdateTime", &p->UpdateTime);
  set_obj(obj, "UpdateMillisec", &p->UpdateMillisec);
  set_obj(obj, "BidPrice1", &p->BidPrice1);
  set_obj(obj, "BidVolume1", &p->BidVolume1);
  set_obj(obj, "AskPrice1", &p->AskPrice1);
  set_obj(obj, "AskVolume1", &p->AskVolume1);
  set_obj(obj, "BidPrice2", &p->BidPrice2);
  set_obj(obj, "BidVolume2", &p->BidVolume2);
  set_obj(obj, "AskPrice2", &p->AskPrice2);
  set_obj(obj, "AskVolume2", &p->AskVolume2);
  set_obj(obj, "BidPrice3", &p->BidPrice3);
  set_obj(obj, "BidVolume3", &p->BidVolume3);
  set_obj(obj, "AskPrice3", &p->AskPrice3);
  set_obj(obj, "AskVolume3", &p->AskVolume3);
  set_obj(obj, "BidPrice4", &p->BidPrice4);
  set_obj(obj, "BidVolume4", &p->BidVolume4);
  set_obj(obj, "AskPrice4", &p->AskPrice4);
  set_obj(obj, "AskVolume4", &p->AskVolume4);
  set_obj(obj, "BidPrice5", &p->BidPrice5);
  set_obj(obj, "BidVolume5", &p->BidVolume5);
  set_obj(obj, "AskPrice5", &p->AskPrice5);
  set_obj(obj, "AskVolume5", &p->AskVolume5);
  set_obj(obj, "AveragePrice", &p->AveragePrice);
  set_obj(obj, "ActionDay", &p->ActionDay);
}
static void set_obj(Local<Object>& obj, CThostFtdcInstrumentTradingRightField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "TradingRight", &p->TradingRight);
}
static void set_obj(Local<Object>& obj, CThostFtdcBrokerUserField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "UserName", &p->UserName);
  set_obj(obj, "UserType", &p->UserType);
  set_obj(obj, "IsActive", &p->IsActive);
  set_obj(obj, "IsUsingOTP", &p->IsUsingOTP);
  set_obj(obj, "IsAuthForce", &p->IsAuthForce);
}
static void set_obj(Local<Object>& obj, CThostFtdcBrokerUserPasswordField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "LastUpdateTime", &p->LastUpdateTime);
  set_obj(obj, "LastLoginTime", &p->LastLoginTime);
  set_obj(obj, "ExpireDate", &p->ExpireDate);
  set_obj(obj, "WeakExpireDate", &p->WeakExpireDate);
}
static void set_obj(Local<Object>& obj, CThostFtdcBrokerUserFunctionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "BrokerFunctionCode", &p->BrokerFunctionCode);
}
static void set_obj(Local<Object>& obj, CThostFtdcTraderOfferField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "OrderLocalID", &p->OrderLocalID);
  set_obj(obj, "TraderConnectStatus", &p->TraderConnectStatus);
  set_obj(obj, "ConnectRequestDate", &p->ConnectRequestDate);
  set_obj(obj, "ConnectRequestTime", &p->ConnectRequestTime);
  set_obj(obj, "LastReportDate", &p->LastReportDate);
  set_obj(obj, "LastReportTime", &p->LastReportTime);
  set_obj(obj, "ConnectDate", &p->ConnectDate);
  set_obj(obj, "ConnectTime", &p->ConnectTime);
  set_obj(obj, "StartDate", &p->StartDate);
  set_obj(obj, "StartTime", &p->StartTime);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "MaxTradeID", &p->MaxTradeID);
  set_obj(obj, "MaxOrderMessageReference", &p->MaxOrderMessageReference);
}
static void set_obj(Local<Object>& obj, CThostFtdcSettlementInfoField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "Content", &p->Content);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
}
static void set_obj(Local<Object>& obj, CThostFtdcInstrumentMarginRateAdjustField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "LongMarginRatioByMoney", &p->LongMarginRatioByMoney);
  set_obj(obj, "LongMarginRatioByVolume", &p->LongMarginRatioByVolume);
  set_obj(obj, "ShortMarginRatioByMoney", &p->ShortMarginRatioByMoney);
  set_obj(obj, "ShortMarginRatioByVolume", &p->ShortMarginRatioByVolume);
  set_obj(obj, "IsRelative", &p->IsRelative);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeMarginRateField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "LongMarginRatioByMoney", &p->LongMarginRatioByMoney);
  set_obj(obj, "LongMarginRatioByVolume", &p->LongMarginRatioByVolume);
  set_obj(obj, "ShortMarginRatioByMoney", &p->ShortMarginRatioByMoney);
  set_obj(obj, "ShortMarginRatioByVolume", &p->ShortMarginRatioByVolume);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeMarginRateAdjustField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "LongMarginRatioByMoney", &p->LongMarginRatioByMoney);
  set_obj(obj, "LongMarginRatioByVolume", &p->LongMarginRatioByVolume);
  set_obj(obj, "ShortMarginRatioByMoney", &p->ShortMarginRatioByMoney);
  set_obj(obj, "ShortMarginRatioByVolume", &p->ShortMarginRatioByVolume);
  set_obj(obj, "ExchLongMarginRatioByMoney", &p->ExchLongMarginRatioByMoney);
  set_obj(obj, "ExchLongMarginRatioByVolume", &p->ExchLongMarginRatioByVolume);
  set_obj(obj, "ExchShortMarginRatioByMoney", &p->ExchShortMarginRatioByMoney);
  set_obj(obj, "ExchShortMarginRatioByVolume", &p->ExchShortMarginRatioByVolume);
  set_obj(obj, "NoLongMarginRatioByMoney", &p->NoLongMarginRatioByMoney);
  set_obj(obj, "NoLongMarginRatioByVolume", &p->NoLongMarginRatioByVolume);
  set_obj(obj, "NoShortMarginRatioByMoney", &p->NoShortMarginRatioByMoney);
  set_obj(obj, "NoShortMarginRatioByVolume", &p->NoShortMarginRatioByVolume);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeRateField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "FromCurrencyID", &p->FromCurrencyID);
  set_obj(obj, "FromCurrencyUnit", &p->FromCurrencyUnit);
  set_obj(obj, "ToCurrencyID", &p->ToCurrencyID);
  set_obj(obj, "ExchangeRate", &p->ExchangeRate);
}
static void set_obj(Local<Object>& obj, CThostFtdcSettlementRefField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
}
static void set_obj(Local<Object>& obj, CThostFtdcCurrentTimeField *p)
{
  set_obj(obj, "CurrDate", &p->CurrDate);
  set_obj(obj, "CurrTime", &p->CurrTime);
  set_obj(obj, "CurrMillisec", &p->CurrMillisec);
  set_obj(obj, "ActionDay", &p->ActionDay);
}
static void set_obj(Local<Object>& obj, CThostFtdcCommPhaseField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "CommPhaseNo", &p->CommPhaseNo);
  set_obj(obj, "SystemID", &p->SystemID);
}
static void set_obj(Local<Object>& obj, CThostFtdcLoginInfoField *p)
{
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "LoginDate", &p->LoginDate);
  set_obj(obj, "LoginTime", &p->LoginTime);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "UserProductInfo", &p->UserProductInfo);
  set_obj(obj, "InterfaceProductInfo", &p->InterfaceProductInfo);
  set_obj(obj, "ProtocolInfo", &p->ProtocolInfo);
  set_obj(obj, "SystemName", &p->SystemName);
  set_obj(obj, "PasswordDeprecated", &p->PasswordDeprecated);
  set_obj(obj, "MaxOrderRef", &p->MaxOrderRef);
  set_obj(obj, "SHFETime", &p->SHFETime);
  set_obj(obj, "DCETime", &p->DCETime);
  set_obj(obj, "CZCETime", &p->CZCETime);
  set_obj(obj, "FFEXTime", &p->FFEXTime);
  set_obj(obj, "MacAddress", &p->MacAddress);
  set_obj(obj, "OneTimePassword", &p->OneTimePassword);
  set_obj(obj, "INETime", &p->INETime);
  set_obj(obj, "IsQryControl", &p->IsQryControl);
  set_obj(obj, "LoginRemark", &p->LoginRemark);
  set_obj(obj, "Password", &p->Password);
}
static void set_obj(Local<Object>& obj, CThostFtdcLogoutAllField *p)
{
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "SystemName", &p->SystemName);
}
static void set_obj(Local<Object>& obj, CThostFtdcFrontStatusField *p)
{
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "LastReportDate", &p->LastReportDate);
  set_obj(obj, "LastReportTime", &p->LastReportTime);
  set_obj(obj, "IsActive", &p->IsActive);
}
static void set_obj(Local<Object>& obj, CThostFtdcUserPasswordUpdateField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "OldPassword", &p->OldPassword);
  set_obj(obj, "NewPassword", &p->NewPassword);
}
static void set_obj(Local<Object>& obj, CThostFtdcInputOrderField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "OrderRef", &p->OrderRef);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "OrderPriceType", &p->OrderPriceType);
  set_obj(obj, "Direction", &p->Direction);
  set_obj(obj, "CombOffsetFlag", &p->CombOffsetFlag);
  set_obj(obj, "CombHedgeFlag", &p->CombHedgeFlag);
  set_obj(obj, "LimitPrice", &p->LimitPrice);
  set_obj(obj, "VolumeTotalOriginal", &p->VolumeTotalOriginal);
  set_obj(obj, "TimeCondition", &p->TimeCondition);
  set_obj(obj, "GTDDate", &p->GTDDate);
  set_obj(obj, "VolumeCondition", &p->VolumeCondition);
  set_obj(obj, "MinVolume", &p->MinVolume);
  set_obj(obj, "ContingentCondition", &p->ContingentCondition);
  set_obj(obj, "StopPrice", &p->StopPrice);
  set_obj(obj, "ForceCloseReason", &p->ForceCloseReason);
  set_obj(obj, "IsAutoSuspend", &p->IsAutoSuspend);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "UserForceClose", &p->UserForceClose);
  set_obj(obj, "IsSwapOrder", &p->IsSwapOrder);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcOrderField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "OrderRef", &p->OrderRef);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "OrderPriceType", &p->OrderPriceType);
  set_obj(obj, "Direction", &p->Direction);
  set_obj(obj, "CombOffsetFlag", &p->CombOffsetFlag);
  set_obj(obj, "CombHedgeFlag", &p->CombHedgeFlag);
  set_obj(obj, "LimitPrice", &p->LimitPrice);
  set_obj(obj, "VolumeTotalOriginal", &p->VolumeTotalOriginal);
  set_obj(obj, "TimeCondition", &p->TimeCondition);
  set_obj(obj, "GTDDate", &p->GTDDate);
  set_obj(obj, "VolumeCondition", &p->VolumeCondition);
  set_obj(obj, "MinVolume", &p->MinVolume);
  set_obj(obj, "ContingentCondition", &p->ContingentCondition);
  set_obj(obj, "StopPrice", &p->StopPrice);
  set_obj(obj, "ForceCloseReason", &p->ForceCloseReason);
  set_obj(obj, "IsAutoSuspend", &p->IsAutoSuspend);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "OrderLocalID", &p->OrderLocalID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "OrderSubmitStatus", &p->OrderSubmitStatus);
  set_obj(obj, "NotifySequence", &p->NotifySequence);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "OrderSysID", &p->OrderSysID);
  set_obj(obj, "OrderSource", &p->OrderSource);
  set_obj(obj, "OrderStatus", &p->OrderStatus);
  set_obj(obj, "OrderType", &p->OrderType);
  set_obj(obj, "VolumeTraded", &p->VolumeTraded);
  set_obj(obj, "VolumeTotal", &p->VolumeTotal);
  set_obj(obj, "InsertDate", &p->InsertDate);
  set_obj(obj, "InsertTime", &p->InsertTime);
  set_obj(obj, "ActiveTime", &p->ActiveTime);
  set_obj(obj, "SuspendTime", &p->SuspendTime);
  set_obj(obj, "UpdateTime", &p->UpdateTime);
  set_obj(obj, "CancelTime", &p->CancelTime);
  set_obj(obj, "ActiveTraderID", &p->ActiveTraderID);
  set_obj(obj, "ClearingPartID", &p->ClearingPartID);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "UserProductInfo", &p->UserProductInfo);
  set_obj(obj, "StatusMsg", &p->StatusMsg);
  set_obj(obj, "UserForceClose", &p->UserForceClose);
  set_obj(obj, "ActiveUserID", &p->ActiveUserID);
  set_obj(obj, "BrokerOrderSeq", &p->BrokerOrderSeq);
  set_obj(obj, "RelativeOrderSysID", &p->RelativeOrderSysID);
  set_obj(obj, "ZCETotalTradedVolume", &p->ZCETotalTradedVolume);
  set_obj(obj, "IsSwapOrder", &p->IsSwapOrder);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeOrderField *p)
{
  set_obj(obj, "OrderPriceType", &p->OrderPriceType);
  set_obj(obj, "Direction", &p->Direction);
  set_obj(obj, "CombOffsetFlag", &p->CombOffsetFlag);
  set_obj(obj, "CombHedgeFlag", &p->CombHedgeFlag);
  set_obj(obj, "LimitPrice", &p->LimitPrice);
  set_obj(obj, "VolumeTotalOriginal", &p->VolumeTotalOriginal);
  set_obj(obj, "TimeCondition", &p->TimeCondition);
  set_obj(obj, "GTDDate", &p->GTDDate);
  set_obj(obj, "VolumeCondition", &p->VolumeCondition);
  set_obj(obj, "MinVolume", &p->MinVolume);
  set_obj(obj, "ContingentCondition", &p->ContingentCondition);
  set_obj(obj, "StopPrice", &p->StopPrice);
  set_obj(obj, "ForceCloseReason", &p->ForceCloseReason);
  set_obj(obj, "IsAutoSuspend", &p->IsAutoSuspend);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "OrderLocalID", &p->OrderLocalID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "OrderSubmitStatus", &p->OrderSubmitStatus);
  set_obj(obj, "NotifySequence", &p->NotifySequence);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "OrderSysID", &p->OrderSysID);
  set_obj(obj, "OrderSource", &p->OrderSource);
  set_obj(obj, "OrderStatus", &p->OrderStatus);
  set_obj(obj, "OrderType", &p->OrderType);
  set_obj(obj, "VolumeTraded", &p->VolumeTraded);
  set_obj(obj, "VolumeTotal", &p->VolumeTotal);
  set_obj(obj, "InsertDate", &p->InsertDate);
  set_obj(obj, "InsertTime", &p->InsertTime);
  set_obj(obj, "ActiveTime", &p->ActiveTime);
  set_obj(obj, "SuspendTime", &p->SuspendTime);
  set_obj(obj, "UpdateTime", &p->UpdateTime);
  set_obj(obj, "CancelTime", &p->CancelTime);
  set_obj(obj, "ActiveTraderID", &p->ActiveTraderID);
  set_obj(obj, "ClearingPartID", &p->ClearingPartID);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeOrderInsertErrorField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "OrderLocalID", &p->OrderLocalID);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
}
static void set_obj(Local<Object>& obj, CThostFtdcInputOrderActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "OrderActionRef", &p->OrderActionRef);
  set_obj(obj, "OrderRef", &p->OrderRef);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "OrderSysID", &p->OrderSysID);
  set_obj(obj, "ActionFlag", &p->ActionFlag);
  set_obj(obj, "LimitPrice", &p->LimitPrice);
  set_obj(obj, "VolumeChange", &p->VolumeChange);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcOrderActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "OrderActionRef", &p->OrderActionRef);
  set_obj(obj, "OrderRef", &p->OrderRef);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "OrderSysID", &p->OrderSysID);
  set_obj(obj, "ActionFlag", &p->ActionFlag);
  set_obj(obj, "LimitPrice", &p->LimitPrice);
  set_obj(obj, "VolumeChange", &p->VolumeChange);
  set_obj(obj, "ActionDate", &p->ActionDate);
  set_obj(obj, "ActionTime", &p->ActionTime);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "OrderLocalID", &p->OrderLocalID);
  set_obj(obj, "ActionLocalID", &p->ActionLocalID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "OrderActionStatus", &p->OrderActionStatus);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "StatusMsg", &p->StatusMsg);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeOrderActionField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "OrderSysID", &p->OrderSysID);
  set_obj(obj, "ActionFlag", &p->ActionFlag);
  set_obj(obj, "LimitPrice", &p->LimitPrice);
  set_obj(obj, "VolumeChange", &p->VolumeChange);
  set_obj(obj, "ActionDate", &p->ActionDate);
  set_obj(obj, "ActionTime", &p->ActionTime);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "OrderLocalID", &p->OrderLocalID);
  set_obj(obj, "ActionLocalID", &p->ActionLocalID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "OrderActionStatus", &p->OrderActionStatus);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeOrderActionErrorField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "OrderSysID", &p->OrderSysID);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "OrderLocalID", &p->OrderLocalID);
  set_obj(obj, "ActionLocalID", &p->ActionLocalID);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeTradeField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "TradeID", &p->TradeID);
  set_obj(obj, "Direction", &p->Direction);
  set_obj(obj, "OrderSysID", &p->OrderSysID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "TradingRole", &p->TradingRole);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "OffsetFlag", &p->OffsetFlag);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "Price", &p->Price);
  set_obj(obj, "Volume", &p->Volume);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "TradeType", &p->TradeType);
  set_obj(obj, "PriceSource", &p->PriceSource);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "OrderLocalID", &p->OrderLocalID);
  set_obj(obj, "ClearingPartID", &p->ClearingPartID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "TradeSource", &p->TradeSource);
}
static void set_obj(Local<Object>& obj, CThostFtdcTradeField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "OrderRef", &p->OrderRef);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "TradeID", &p->TradeID);
  set_obj(obj, "Direction", &p->Direction);
  set_obj(obj, "OrderSysID", &p->OrderSysID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "TradingRole", &p->TradingRole);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "OffsetFlag", &p->OffsetFlag);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "Price", &p->Price);
  set_obj(obj, "Volume", &p->Volume);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "TradeType", &p->TradeType);
  set_obj(obj, "PriceSource", &p->PriceSource);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "OrderLocalID", &p->OrderLocalID);
  set_obj(obj, "ClearingPartID", &p->ClearingPartID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "BrokerOrderSeq", &p->BrokerOrderSeq);
  set_obj(obj, "TradeSource", &p->TradeSource);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcUserSessionField *p)
{
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "LoginDate", &p->LoginDate);
  set_obj(obj, "LoginTime", &p->LoginTime);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "UserProductInfo", &p->UserProductInfo);
  set_obj(obj, "InterfaceProductInfo", &p->InterfaceProductInfo);
  set_obj(obj, "ProtocolInfo", &p->ProtocolInfo);
  set_obj(obj, "MacAddress", &p->MacAddress);
  set_obj(obj, "LoginRemark", &p->LoginRemark);
}
static void set_obj(Local<Object>& obj, CThostFtdcQueryMaxOrderVolumeField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "Direction", &p->Direction);
  set_obj(obj, "OffsetFlag", &p->OffsetFlag);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "MaxVolume", &p->MaxVolume);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcSettlementInfoConfirmField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "ConfirmDate", &p->ConfirmDate);
  set_obj(obj, "ConfirmTime", &p->ConfirmTime);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
}
static void set_obj(Local<Object>& obj, CThostFtdcSyncDepositField *p)
{
  set_obj(obj, "DepositSeqNo", &p->DepositSeqNo);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "Deposit", &p->Deposit);
  set_obj(obj, "IsForce", &p->IsForce);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
}
static void set_obj(Local<Object>& obj, CThostFtdcSyncFundMortgageField *p)
{
  set_obj(obj, "MortgageSeqNo", &p->MortgageSeqNo);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "FromCurrencyID", &p->FromCurrencyID);
  set_obj(obj, "MortgageAmount", &p->MortgageAmount);
  set_obj(obj, "ToCurrencyID", &p->ToCurrencyID);
}
static void set_obj(Local<Object>& obj, CThostFtdcBrokerSyncField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
}
static void set_obj(Local<Object>& obj, CThostFtdcSyncingInvestorField *p)
{
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorGroupID", &p->InvestorGroupID);
  set_obj(obj, "InvestorName", &p->InvestorName);
  set_obj(obj, "IdentifiedCardType", &p->IdentifiedCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "IsActive", &p->IsActive);
  set_obj(obj, "Telephone", &p->Telephone);
  set_obj(obj, "Address", &p->Address);
  set_obj(obj, "OpenDate", &p->OpenDate);
  set_obj(obj, "Mobile", &p->Mobile);
  set_obj(obj, "CommModelID", &p->CommModelID);
  set_obj(obj, "MarginModelID", &p->MarginModelID);
}
static void set_obj(Local<Object>& obj, CThostFtdcSyncingTradingCodeField *p)
{
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "IsActive", &p->IsActive);
  set_obj(obj, "ClientIDType", &p->ClientIDType);
}
static void set_obj(Local<Object>& obj, CThostFtdcSyncingInvestorGroupField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorGroupID", &p->InvestorGroupID);
  set_obj(obj, "InvestorGroupName", &p->InvestorGroupName);
}
static void set_obj(Local<Object>& obj, CThostFtdcSyncingTradingAccountField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "PreMortgage", &p->PreMortgage);
  set_obj(obj, "PreCredit", &p->PreCredit);
  set_obj(obj, "PreDeposit", &p->PreDeposit);
  set_obj(obj, "PreBalance", &p->PreBalance);
  set_obj(obj, "PreMargin", &p->PreMargin);
  set_obj(obj, "InterestBase", &p->InterestBase);
  set_obj(obj, "Interest", &p->Interest);
  set_obj(obj, "Deposit", &p->Deposit);
  set_obj(obj, "Withdraw", &p->Withdraw);
  set_obj(obj, "FrozenMargin", &p->FrozenMargin);
  set_obj(obj, "FrozenCash", &p->FrozenCash);
  set_obj(obj, "FrozenCommission", &p->FrozenCommission);
  set_obj(obj, "CurrMargin", &p->CurrMargin);
  set_obj(obj, "CashIn", &p->CashIn);
  set_obj(obj, "Commission", &p->Commission);
  set_obj(obj, "CloseProfit", &p->CloseProfit);
  set_obj(obj, "PositionProfit", &p->PositionProfit);
  set_obj(obj, "Balance", &p->Balance);
  set_obj(obj, "Available", &p->Available);
  set_obj(obj, "WithdrawQuota", &p->WithdrawQuota);
  set_obj(obj, "Reserve", &p->Reserve);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "Credit", &p->Credit);
  set_obj(obj, "Mortgage", &p->Mortgage);
  set_obj(obj, "ExchangeMargin", &p->ExchangeMargin);
  set_obj(obj, "DeliveryMargin", &p->DeliveryMargin);
  set_obj(obj, "ExchangeDeliveryMargin", &p->ExchangeDeliveryMargin);
  set_obj(obj, "ReserveBalance", &p->ReserveBalance);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "PreFundMortgageIn", &p->PreFundMortgageIn);
  set_obj(obj, "PreFundMortgageOut", &p->PreFundMortgageOut);
  set_obj(obj, "FundMortgageIn", &p->FundMortgageIn);
  set_obj(obj, "FundMortgageOut", &p->FundMortgageOut);
  set_obj(obj, "FundMortgageAvailable", &p->FundMortgageAvailable);
  set_obj(obj, "MortgageableFund", &p->MortgageableFund);
  set_obj(obj, "SpecProductMargin", &p->SpecProductMargin);
  set_obj(obj, "SpecProductFrozenMargin", &p->SpecProductFrozenMargin);
  set_obj(obj, "SpecProductCommission", &p->SpecProductCommission);
  set_obj(obj, "SpecProductFrozenCommission", &p->SpecProductFrozenCommission);
  set_obj(obj, "SpecProductPositionProfit", &p->SpecProductPositionProfit);
  set_obj(obj, "SpecProductCloseProfit", &p->SpecProductCloseProfit);
  set_obj(obj, "SpecProductPositionProfitByAlg", &p->SpecProductPositionProfitByAlg);
  set_obj(obj, "SpecProductExchangeMargin", &p->SpecProductExchangeMargin);
  set_obj(obj, "FrozenSwap", &p->FrozenSwap);
  set_obj(obj, "RemainSwap", &p->RemainSwap);
}
static void set_obj(Local<Object>& obj, CThostFtdcSyncingInvestorPositionField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "PosiDirection", &p->PosiDirection);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "PositionDate", &p->PositionDate);
  set_obj(obj, "YdPosition", &p->YdPosition);
  set_obj(obj, "Position", &p->Position);
  set_obj(obj, "LongFrozen", &p->LongFrozen);
  set_obj(obj, "ShortFrozen", &p->ShortFrozen);
  set_obj(obj, "LongFrozenAmount", &p->LongFrozenAmount);
  set_obj(obj, "ShortFrozenAmount", &p->ShortFrozenAmount);
  set_obj(obj, "OpenVolume", &p->OpenVolume);
  set_obj(obj, "CloseVolume", &p->CloseVolume);
  set_obj(obj, "OpenAmount", &p->OpenAmount);
  set_obj(obj, "CloseAmount", &p->CloseAmount);
  set_obj(obj, "PositionCost", &p->PositionCost);
  set_obj(obj, "PreMargin", &p->PreMargin);
  set_obj(obj, "UseMargin", &p->UseMargin);
  set_obj(obj, "FrozenMargin", &p->FrozenMargin);
  set_obj(obj, "FrozenCash", &p->FrozenCash);
  set_obj(obj, "FrozenCommission", &p->FrozenCommission);
  set_obj(obj, "CashIn", &p->CashIn);
  set_obj(obj, "Commission", &p->Commission);
  set_obj(obj, "CloseProfit", &p->CloseProfit);
  set_obj(obj, "PositionProfit", &p->PositionProfit);
  set_obj(obj, "PreSettlementPrice", &p->PreSettlementPrice);
  set_obj(obj, "SettlementPrice", &p->SettlementPrice);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "OpenCost", &p->OpenCost);
  set_obj(obj, "ExchangeMargin", &p->ExchangeMargin);
  set_obj(obj, "CombPosition", &p->CombPosition);
  set_obj(obj, "CombLongFrozen", &p->CombLongFrozen);
  set_obj(obj, "CombShortFrozen", &p->CombShortFrozen);
  set_obj(obj, "CloseProfitByDate", &p->CloseProfitByDate);
  set_obj(obj, "CloseProfitByTrade", &p->CloseProfitByTrade);
  set_obj(obj, "TodayPosition", &p->TodayPosition);
  set_obj(obj, "MarginRateByMoney", &p->MarginRateByMoney);
  set_obj(obj, "MarginRateByVolume", &p->MarginRateByVolume);
  set_obj(obj, "StrikeFrozen", &p->StrikeFrozen);
  set_obj(obj, "StrikeFrozenAmount", &p->StrikeFrozenAmount);
  set_obj(obj, "AbandonFrozen", &p->AbandonFrozen);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "YdStrikeFrozen", &p->YdStrikeFrozen);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "PositionCostOffset", &p->PositionCostOffset);
}
static void set_obj(Local<Object>& obj, CThostFtdcSyncingInstrumentMarginRateField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "LongMarginRatioByMoney", &p->LongMarginRatioByMoney);
  set_obj(obj, "LongMarginRatioByVolume", &p->LongMarginRatioByVolume);
  set_obj(obj, "ShortMarginRatioByMoney", &p->ShortMarginRatioByMoney);
  set_obj(obj, "ShortMarginRatioByVolume", &p->ShortMarginRatioByVolume);
  set_obj(obj, "IsRelative", &p->IsRelative);
}
static void set_obj(Local<Object>& obj, CThostFtdcSyncingInstrumentCommissionRateField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "OpenRatioByMoney", &p->OpenRatioByMoney);
  set_obj(obj, "OpenRatioByVolume", &p->OpenRatioByVolume);
  set_obj(obj, "CloseRatioByMoney", &p->CloseRatioByMoney);
  set_obj(obj, "CloseRatioByVolume", &p->CloseRatioByVolume);
  set_obj(obj, "CloseTodayRatioByMoney", &p->CloseTodayRatioByMoney);
  set_obj(obj, "CloseTodayRatioByVolume", &p->CloseTodayRatioByVolume);
}
static void set_obj(Local<Object>& obj, CThostFtdcSyncingInstrumentTradingRightField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "TradingRight", &p->TradingRight);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryOrderField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "OrderSysID", &p->OrderSysID);
  set_obj(obj, "InsertTimeStart", &p->InsertTimeStart);
  set_obj(obj, "InsertTimeEnd", &p->InsertTimeEnd);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryTradeField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "TradeID", &p->TradeID);
  set_obj(obj, "TradeTimeStart", &p->TradeTimeStart);
  set_obj(obj, "TradeTimeEnd", &p->TradeTimeEnd);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryInvestorPositionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryTradingAccountField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "BizType", &p->BizType);
  set_obj(obj, "AccountID", &p->AccountID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryInvestorField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryTradingCodeField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ClientIDType", &p->ClientIDType);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryInvestorGroupField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryInstrumentMarginRateField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryInstrumentCommissionRateField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryInstrumentTradingRightField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryBrokerField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryTraderField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "TraderID", &p->TraderID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQrySuperUserFunctionField *p)
{
  set_obj(obj, "UserID", &p->UserID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryUserSessionField *p)
{
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryPartBrokerField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryFrontStatusField *p)
{
  set_obj(obj, "FrontID", &p->FrontID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryExchangeOrderField *p)
{
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "TraderID", &p->TraderID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryOrderActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryExchangeOrderActionField *p)
{
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "TraderID", &p->TraderID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQrySuperUserField *p)
{
  set_obj(obj, "UserID", &p->UserID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryExchangeField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryProductField *p)
{
  set_obj(obj, "ProductID", &p->ProductID);
  set_obj(obj, "ProductClass", &p->ProductClass);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryInstrumentField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "ProductID", &p->ProductID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryDepthMarketDataField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryBrokerUserField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryBrokerUserFunctionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryTraderOfferField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "TraderID", &p->TraderID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQrySyncDepositField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "DepositSeqNo", &p->DepositSeqNo);
}
static void set_obj(Local<Object>& obj, CThostFtdcQrySettlementInfoField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryExchangeMarginRateField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryExchangeMarginRateAdjustField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryExchangeRateField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "FromCurrencyID", &p->FromCurrencyID);
  set_obj(obj, "ToCurrencyID", &p->ToCurrencyID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQrySyncFundMortgageField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "MortgageSeqNo", &p->MortgageSeqNo);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryHisOrderField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "OrderSysID", &p->OrderSysID);
  set_obj(obj, "InsertTimeStart", &p->InsertTimeStart);
  set_obj(obj, "InsertTimeEnd", &p->InsertTimeEnd);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
}
static void set_obj(Local<Object>& obj, CThostFtdcOptionInstrMiniMarginField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "MinMargin", &p->MinMargin);
  set_obj(obj, "ValueMethod", &p->ValueMethod);
  set_obj(obj, "IsRelative", &p->IsRelative);
}
static void set_obj(Local<Object>& obj, CThostFtdcOptionInstrMarginAdjustField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "SShortMarginRatioByMoney", &p->SShortMarginRatioByMoney);
  set_obj(obj, "SShortMarginRatioByVolume", &p->SShortMarginRatioByVolume);
  set_obj(obj, "HShortMarginRatioByMoney", &p->HShortMarginRatioByMoney);
  set_obj(obj, "HShortMarginRatioByVolume", &p->HShortMarginRatioByVolume);
  set_obj(obj, "AShortMarginRatioByMoney", &p->AShortMarginRatioByMoney);
  set_obj(obj, "AShortMarginRatioByVolume", &p->AShortMarginRatioByVolume);
  set_obj(obj, "IsRelative", &p->IsRelative);
  set_obj(obj, "MShortMarginRatioByMoney", &p->MShortMarginRatioByMoney);
  set_obj(obj, "MShortMarginRatioByVolume", &p->MShortMarginRatioByVolume);
}
static void set_obj(Local<Object>& obj, CThostFtdcOptionInstrCommRateField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "OpenRatioByMoney", &p->OpenRatioByMoney);
  set_obj(obj, "OpenRatioByVolume", &p->OpenRatioByVolume);
  set_obj(obj, "CloseRatioByMoney", &p->CloseRatioByMoney);
  set_obj(obj, "CloseRatioByVolume", &p->CloseRatioByVolume);
  set_obj(obj, "CloseTodayRatioByMoney", &p->CloseTodayRatioByMoney);
  set_obj(obj, "CloseTodayRatioByVolume", &p->CloseTodayRatioByVolume);
  set_obj(obj, "StrikeRatioByMoney", &p->StrikeRatioByMoney);
  set_obj(obj, "StrikeRatioByVolume", &p->StrikeRatioByVolume);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcOptionInstrTradeCostField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "FixedMargin", &p->FixedMargin);
  set_obj(obj, "MiniMargin", &p->MiniMargin);
  set_obj(obj, "Royalty", &p->Royalty);
  set_obj(obj, "ExchFixedMargin", &p->ExchFixedMargin);
  set_obj(obj, "ExchMiniMargin", &p->ExchMiniMargin);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryOptionInstrTradeCostField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "InputPrice", &p->InputPrice);
  set_obj(obj, "UnderlyingPrice", &p->UnderlyingPrice);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryOptionInstrCommRateField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcIndexPriceField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ClosePrice", &p->ClosePrice);
}
static void set_obj(Local<Object>& obj, CThostFtdcInputExecOrderField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExecOrderRef", &p->ExecOrderRef);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Volume", &p->Volume);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "OffsetFlag", &p->OffsetFlag);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "ActionType", &p->ActionType);
  set_obj(obj, "PosiDirection", &p->PosiDirection);
  set_obj(obj, "ReservePositionFlag", &p->ReservePositionFlag);
  set_obj(obj, "CloseFlag", &p->CloseFlag);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcInputExecOrderActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "ExecOrderActionRef", &p->ExecOrderActionRef);
  set_obj(obj, "ExecOrderRef", &p->ExecOrderRef);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ExecOrderSysID", &p->ExecOrderSysID);
  set_obj(obj, "ActionFlag", &p->ActionFlag);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcExecOrderField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExecOrderRef", &p->ExecOrderRef);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Volume", &p->Volume);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "OffsetFlag", &p->OffsetFlag);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "ActionType", &p->ActionType);
  set_obj(obj, "PosiDirection", &p->PosiDirection);
  set_obj(obj, "ReservePositionFlag", &p->ReservePositionFlag);
  set_obj(obj, "CloseFlag", &p->CloseFlag);
  set_obj(obj, "ExecOrderLocalID", &p->ExecOrderLocalID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "OrderSubmitStatus", &p->OrderSubmitStatus);
  set_obj(obj, "NotifySequence", &p->NotifySequence);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "ExecOrderSysID", &p->ExecOrderSysID);
  set_obj(obj, "InsertDate", &p->InsertDate);
  set_obj(obj, "InsertTime", &p->InsertTime);
  set_obj(obj, "CancelTime", &p->CancelTime);
  set_obj(obj, "ExecResult", &p->ExecResult);
  set_obj(obj, "ClearingPartID", &p->ClearingPartID);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "UserProductInfo", &p->UserProductInfo);
  set_obj(obj, "StatusMsg", &p->StatusMsg);
  set_obj(obj, "ActiveUserID", &p->ActiveUserID);
  set_obj(obj, "BrokerExecOrderSeq", &p->BrokerExecOrderSeq);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcExecOrderActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "ExecOrderActionRef", &p->ExecOrderActionRef);
  set_obj(obj, "ExecOrderRef", &p->ExecOrderRef);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ExecOrderSysID", &p->ExecOrderSysID);
  set_obj(obj, "ActionFlag", &p->ActionFlag);
  set_obj(obj, "ActionDate", &p->ActionDate);
  set_obj(obj, "ActionTime", &p->ActionTime);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "ExecOrderLocalID", &p->ExecOrderLocalID);
  set_obj(obj, "ActionLocalID", &p->ActionLocalID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "OrderActionStatus", &p->OrderActionStatus);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "ActionType", &p->ActionType);
  set_obj(obj, "StatusMsg", &p->StatusMsg);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryExecOrderField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ExecOrderSysID", &p->ExecOrderSysID);
  set_obj(obj, "InsertTimeStart", &p->InsertTimeStart);
  set_obj(obj, "InsertTimeEnd", &p->InsertTimeEnd);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeExecOrderField *p)
{
  set_obj(obj, "Volume", &p->Volume);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "OffsetFlag", &p->OffsetFlag);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "ActionType", &p->ActionType);
  set_obj(obj, "PosiDirection", &p->PosiDirection);
  set_obj(obj, "ReservePositionFlag", &p->ReservePositionFlag);
  set_obj(obj, "CloseFlag", &p->CloseFlag);
  set_obj(obj, "ExecOrderLocalID", &p->ExecOrderLocalID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "OrderSubmitStatus", &p->OrderSubmitStatus);
  set_obj(obj, "NotifySequence", &p->NotifySequence);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "ExecOrderSysID", &p->ExecOrderSysID);
  set_obj(obj, "InsertDate", &p->InsertDate);
  set_obj(obj, "InsertTime", &p->InsertTime);
  set_obj(obj, "CancelTime", &p->CancelTime);
  set_obj(obj, "ExecResult", &p->ExecResult);
  set_obj(obj, "ClearingPartID", &p->ClearingPartID);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryExchangeExecOrderField *p)
{
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "TraderID", &p->TraderID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryExecOrderActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeExecOrderActionField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ExecOrderSysID", &p->ExecOrderSysID);
  set_obj(obj, "ActionFlag", &p->ActionFlag);
  set_obj(obj, "ActionDate", &p->ActionDate);
  set_obj(obj, "ActionTime", &p->ActionTime);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "ExecOrderLocalID", &p->ExecOrderLocalID);
  set_obj(obj, "ActionLocalID", &p->ActionLocalID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "OrderActionStatus", &p->OrderActionStatus);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "ActionType", &p->ActionType);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "Volume", &p->Volume);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryExchangeExecOrderActionField *p)
{
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "TraderID", &p->TraderID);
}
static void set_obj(Local<Object>& obj, CThostFtdcErrExecOrderField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExecOrderRef", &p->ExecOrderRef);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Volume", &p->Volume);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "OffsetFlag", &p->OffsetFlag);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "ActionType", &p->ActionType);
  set_obj(obj, "PosiDirection", &p->PosiDirection);
  set_obj(obj, "ReservePositionFlag", &p->ReservePositionFlag);
  set_obj(obj, "CloseFlag", &p->CloseFlag);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryErrExecOrderField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
}
static void set_obj(Local<Object>& obj, CThostFtdcErrExecOrderActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "ExecOrderActionRef", &p->ExecOrderActionRef);
  set_obj(obj, "ExecOrderRef", &p->ExecOrderRef);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ExecOrderSysID", &p->ExecOrderSysID);
  set_obj(obj, "ActionFlag", &p->ActionFlag);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryErrExecOrderActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
}
static void set_obj(Local<Object>& obj, CThostFtdcOptionInstrTradingRightField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "Direction", &p->Direction);
  set_obj(obj, "TradingRight", &p->TradingRight);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryOptionInstrTradingRightField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "Direction", &p->Direction);
}
static void set_obj(Local<Object>& obj, CThostFtdcInputForQuoteField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ForQuoteRef", &p->ForQuoteRef);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcForQuoteField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ForQuoteRef", &p->ForQuoteRef);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "ForQuoteLocalID", &p->ForQuoteLocalID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "InsertDate", &p->InsertDate);
  set_obj(obj, "InsertTime", &p->InsertTime);
  set_obj(obj, "ForQuoteStatus", &p->ForQuoteStatus);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "StatusMsg", &p->StatusMsg);
  set_obj(obj, "ActiveUserID", &p->ActiveUserID);
  set_obj(obj, "BrokerForQutoSeq", &p->BrokerForQutoSeq);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryForQuoteField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InsertTimeStart", &p->InsertTimeStart);
  set_obj(obj, "InsertTimeEnd", &p->InsertTimeEnd);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeForQuoteField *p)
{
  set_obj(obj, "ForQuoteLocalID", &p->ForQuoteLocalID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "InsertDate", &p->InsertDate);
  set_obj(obj, "InsertTime", &p->InsertTime);
  set_obj(obj, "ForQuoteStatus", &p->ForQuoteStatus);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryExchangeForQuoteField *p)
{
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "TraderID", &p->TraderID);
}
static void set_obj(Local<Object>& obj, CThostFtdcInputQuoteField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "QuoteRef", &p->QuoteRef);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "AskPrice", &p->AskPrice);
  set_obj(obj, "BidPrice", &p->BidPrice);
  set_obj(obj, "AskVolume", &p->AskVolume);
  set_obj(obj, "BidVolume", &p->BidVolume);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "AskOffsetFlag", &p->AskOffsetFlag);
  set_obj(obj, "BidOffsetFlag", &p->BidOffsetFlag);
  set_obj(obj, "AskHedgeFlag", &p->AskHedgeFlag);
  set_obj(obj, "BidHedgeFlag", &p->BidHedgeFlag);
  set_obj(obj, "AskOrderRef", &p->AskOrderRef);
  set_obj(obj, "BidOrderRef", &p->BidOrderRef);
  set_obj(obj, "ForQuoteSysID", &p->ForQuoteSysID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcInputQuoteActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "QuoteActionRef", &p->QuoteActionRef);
  set_obj(obj, "QuoteRef", &p->QuoteRef);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "QuoteSysID", &p->QuoteSysID);
  set_obj(obj, "ActionFlag", &p->ActionFlag);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcQuoteField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "QuoteRef", &p->QuoteRef);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "AskPrice", &p->AskPrice);
  set_obj(obj, "BidPrice", &p->BidPrice);
  set_obj(obj, "AskVolume", &p->AskVolume);
  set_obj(obj, "BidVolume", &p->BidVolume);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "AskOffsetFlag", &p->AskOffsetFlag);
  set_obj(obj, "BidOffsetFlag", &p->BidOffsetFlag);
  set_obj(obj, "AskHedgeFlag", &p->AskHedgeFlag);
  set_obj(obj, "BidHedgeFlag", &p->BidHedgeFlag);
  set_obj(obj, "QuoteLocalID", &p->QuoteLocalID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "NotifySequence", &p->NotifySequence);
  set_obj(obj, "OrderSubmitStatus", &p->OrderSubmitStatus);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "QuoteSysID", &p->QuoteSysID);
  set_obj(obj, "InsertDate", &p->InsertDate);
  set_obj(obj, "InsertTime", &p->InsertTime);
  set_obj(obj, "CancelTime", &p->CancelTime);
  set_obj(obj, "QuoteStatus", &p->QuoteStatus);
  set_obj(obj, "ClearingPartID", &p->ClearingPartID);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "AskOrderSysID", &p->AskOrderSysID);
  set_obj(obj, "BidOrderSysID", &p->BidOrderSysID);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "UserProductInfo", &p->UserProductInfo);
  set_obj(obj, "StatusMsg", &p->StatusMsg);
  set_obj(obj, "ActiveUserID", &p->ActiveUserID);
  set_obj(obj, "BrokerQuoteSeq", &p->BrokerQuoteSeq);
  set_obj(obj, "AskOrderRef", &p->AskOrderRef);
  set_obj(obj, "BidOrderRef", &p->BidOrderRef);
  set_obj(obj, "ForQuoteSysID", &p->ForQuoteSysID);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcQuoteActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "QuoteActionRef", &p->QuoteActionRef);
  set_obj(obj, "QuoteRef", &p->QuoteRef);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "QuoteSysID", &p->QuoteSysID);
  set_obj(obj, "ActionFlag", &p->ActionFlag);
  set_obj(obj, "ActionDate", &p->ActionDate);
  set_obj(obj, "ActionTime", &p->ActionTime);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "QuoteLocalID", &p->QuoteLocalID);
  set_obj(obj, "ActionLocalID", &p->ActionLocalID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "OrderActionStatus", &p->OrderActionStatus);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "StatusMsg", &p->StatusMsg);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryQuoteField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "QuoteSysID", &p->QuoteSysID);
  set_obj(obj, "InsertTimeStart", &p->InsertTimeStart);
  set_obj(obj, "InsertTimeEnd", &p->InsertTimeEnd);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeQuoteField *p)
{
  set_obj(obj, "AskPrice", &p->AskPrice);
  set_obj(obj, "BidPrice", &p->BidPrice);
  set_obj(obj, "AskVolume", &p->AskVolume);
  set_obj(obj, "BidVolume", &p->BidVolume);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "AskOffsetFlag", &p->AskOffsetFlag);
  set_obj(obj, "BidOffsetFlag", &p->BidOffsetFlag);
  set_obj(obj, "AskHedgeFlag", &p->AskHedgeFlag);
  set_obj(obj, "BidHedgeFlag", &p->BidHedgeFlag);
  set_obj(obj, "QuoteLocalID", &p->QuoteLocalID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "NotifySequence", &p->NotifySequence);
  set_obj(obj, "OrderSubmitStatus", &p->OrderSubmitStatus);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "QuoteSysID", &p->QuoteSysID);
  set_obj(obj, "InsertDate", &p->InsertDate);
  set_obj(obj, "InsertTime", &p->InsertTime);
  set_obj(obj, "CancelTime", &p->CancelTime);
  set_obj(obj, "QuoteStatus", &p->QuoteStatus);
  set_obj(obj, "ClearingPartID", &p->ClearingPartID);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "AskOrderSysID", &p->AskOrderSysID);
  set_obj(obj, "BidOrderSysID", &p->BidOrderSysID);
  set_obj(obj, "ForQuoteSysID", &p->ForQuoteSysID);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryExchangeQuoteField *p)
{
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "TraderID", &p->TraderID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryQuoteActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeQuoteActionField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "QuoteSysID", &p->QuoteSysID);
  set_obj(obj, "ActionFlag", &p->ActionFlag);
  set_obj(obj, "ActionDate", &p->ActionDate);
  set_obj(obj, "ActionTime", &p->ActionTime);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "QuoteLocalID", &p->QuoteLocalID);
  set_obj(obj, "ActionLocalID", &p->ActionLocalID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "OrderActionStatus", &p->OrderActionStatus);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryExchangeQuoteActionField *p)
{
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "TraderID", &p->TraderID);
}
static void set_obj(Local<Object>& obj, CThostFtdcOptionInstrDeltaField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "Delta", &p->Delta);
}
static void set_obj(Local<Object>& obj, CThostFtdcForQuoteRspField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ForQuoteSysID", &p->ForQuoteSysID);
  set_obj(obj, "ForQuoteTime", &p->ForQuoteTime);
  set_obj(obj, "ActionDay", &p->ActionDay);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcStrikeOffsetField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "Offset", &p->Offset);
  set_obj(obj, "OffsetType", &p->OffsetType);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryStrikeOffsetField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
}
static void set_obj(Local<Object>& obj, CThostFtdcInputBatchOrderActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "OrderActionRef", &p->OrderActionRef);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcBatchOrderActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "OrderActionRef", &p->OrderActionRef);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ActionDate", &p->ActionDate);
  set_obj(obj, "ActionTime", &p->ActionTime);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "ActionLocalID", &p->ActionLocalID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "OrderActionStatus", &p->OrderActionStatus);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "StatusMsg", &p->StatusMsg);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeBatchOrderActionField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ActionDate", &p->ActionDate);
  set_obj(obj, "ActionTime", &p->ActionTime);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "ActionLocalID", &p->ActionLocalID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "OrderActionStatus", &p->OrderActionStatus);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryBatchOrderActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcCombInstrumentGuardField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "GuarantRatio", &p->GuarantRatio);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryCombInstrumentGuardField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcInputCombActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "CombActionRef", &p->CombActionRef);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Direction", &p->Direction);
  set_obj(obj, "Volume", &p->Volume);
  set_obj(obj, "CombDirection", &p->CombDirection);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcCombActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "CombActionRef", &p->CombActionRef);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Direction", &p->Direction);
  set_obj(obj, "Volume", &p->Volume);
  set_obj(obj, "CombDirection", &p->CombDirection);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "ActionLocalID", &p->ActionLocalID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "ActionStatus", &p->ActionStatus);
  set_obj(obj, "NotifySequence", &p->NotifySequence);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "UserProductInfo", &p->UserProductInfo);
  set_obj(obj, "StatusMsg", &p->StatusMsg);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
  set_obj(obj, "ComTradeID", &p->ComTradeID);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryCombActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeCombActionField *p)
{
  set_obj(obj, "Direction", &p->Direction);
  set_obj(obj, "Volume", &p->Volume);
  set_obj(obj, "CombDirection", &p->CombDirection);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "ActionLocalID", &p->ActionLocalID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "ActionStatus", &p->ActionStatus);
  set_obj(obj, "NotifySequence", &p->NotifySequence);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
  set_obj(obj, "ComTradeID", &p->ComTradeID);
  set_obj(obj, "BranchID", &p->BranchID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryExchangeCombActionField *p)
{
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "TraderID", &p->TraderID);
}
static void set_obj(Local<Object>& obj, CThostFtdcProductExchRateField *p)
{
  set_obj(obj, "ProductID", &p->ProductID);
  set_obj(obj, "QuoteCurrencyID", &p->QuoteCurrencyID);
  set_obj(obj, "ExchangeRate", &p->ExchangeRate);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryProductExchRateField *p)
{
  set_obj(obj, "ProductID", &p->ProductID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryForQuoteParamField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcForQuoteParamField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "LastPrice", &p->LastPrice);
  set_obj(obj, "PriceInterval", &p->PriceInterval);
}
static void set_obj(Local<Object>& obj, CThostFtdcMMOptionInstrCommRateField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "OpenRatioByMoney", &p->OpenRatioByMoney);
  set_obj(obj, "OpenRatioByVolume", &p->OpenRatioByVolume);
  set_obj(obj, "CloseRatioByMoney", &p->CloseRatioByMoney);
  set_obj(obj, "CloseRatioByVolume", &p->CloseRatioByVolume);
  set_obj(obj, "CloseTodayRatioByMoney", &p->CloseTodayRatioByMoney);
  set_obj(obj, "CloseTodayRatioByVolume", &p->CloseTodayRatioByVolume);
  set_obj(obj, "StrikeRatioByMoney", &p->StrikeRatioByMoney);
  set_obj(obj, "StrikeRatioByVolume", &p->StrikeRatioByVolume);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryMMOptionInstrCommRateField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
}
static void set_obj(Local<Object>& obj, CThostFtdcMMInstrumentCommissionRateField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "OpenRatioByMoney", &p->OpenRatioByMoney);
  set_obj(obj, "OpenRatioByVolume", &p->OpenRatioByVolume);
  set_obj(obj, "CloseRatioByMoney", &p->CloseRatioByMoney);
  set_obj(obj, "CloseRatioByVolume", &p->CloseRatioByVolume);
  set_obj(obj, "CloseTodayRatioByMoney", &p->CloseTodayRatioByMoney);
  set_obj(obj, "CloseTodayRatioByVolume", &p->CloseTodayRatioByVolume);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryMMInstrumentCommissionRateField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
}
static void set_obj(Local<Object>& obj, CThostFtdcInstrumentOrderCommRateField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "OrderCommByVolume", &p->OrderCommByVolume);
  set_obj(obj, "OrderActionCommByVolume", &p->OrderActionCommByVolume);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryInstrumentOrderCommRateField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
}
static void set_obj(Local<Object>& obj, CThostFtdcTradeParamField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "TradeParamID", &p->TradeParamID);
  set_obj(obj, "TradeParamValue", &p->TradeParamValue);
  set_obj(obj, "Memo", &p->Memo);
}
static void set_obj(Local<Object>& obj, CThostFtdcInstrumentMarginRateULField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "LongMarginRatioByMoney", &p->LongMarginRatioByMoney);
  set_obj(obj, "LongMarginRatioByVolume", &p->LongMarginRatioByVolume);
  set_obj(obj, "ShortMarginRatioByMoney", &p->ShortMarginRatioByMoney);
  set_obj(obj, "ShortMarginRatioByVolume", &p->ShortMarginRatioByVolume);
}
static void set_obj(Local<Object>& obj, CThostFtdcFutureLimitPosiParamField *p)
{
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "ProductID", &p->ProductID);
  set_obj(obj, "SpecOpenVolume", &p->SpecOpenVolume);
  set_obj(obj, "ArbiOpenVolume", &p->ArbiOpenVolume);
  set_obj(obj, "OpenVolume", &p->OpenVolume);
}
static void set_obj(Local<Object>& obj, CThostFtdcLoginForbiddenIPField *p)
{
  set_obj(obj, "IPAddress", &p->IPAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcIPListField *p)
{
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "IsWhite", &p->IsWhite);
}
static void set_obj(Local<Object>& obj, CThostFtdcInputOptionSelfCloseField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "OptionSelfCloseRef", &p->OptionSelfCloseRef);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Volume", &p->Volume);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "OptSelfCloseFlag", &p->OptSelfCloseFlag);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcInputOptionSelfCloseActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "OptionSelfCloseActionRef", &p->OptionSelfCloseActionRef);
  set_obj(obj, "OptionSelfCloseRef", &p->OptionSelfCloseRef);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "OptionSelfCloseSysID", &p->OptionSelfCloseSysID);
  set_obj(obj, "ActionFlag", &p->ActionFlag);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcOptionSelfCloseField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "OptionSelfCloseRef", &p->OptionSelfCloseRef);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Volume", &p->Volume);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "OptSelfCloseFlag", &p->OptSelfCloseFlag);
  set_obj(obj, "OptionSelfCloseLocalID", &p->OptionSelfCloseLocalID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "OrderSubmitStatus", &p->OrderSubmitStatus);
  set_obj(obj, "NotifySequence", &p->NotifySequence);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "OptionSelfCloseSysID", &p->OptionSelfCloseSysID);
  set_obj(obj, "InsertDate", &p->InsertDate);
  set_obj(obj, "InsertTime", &p->InsertTime);
  set_obj(obj, "CancelTime", &p->CancelTime);
  set_obj(obj, "ExecResult", &p->ExecResult);
  set_obj(obj, "ClearingPartID", &p->ClearingPartID);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "UserProductInfo", &p->UserProductInfo);
  set_obj(obj, "StatusMsg", &p->StatusMsg);
  set_obj(obj, "ActiveUserID", &p->ActiveUserID);
  set_obj(obj, "BrokerOptionSelfCloseSeq", &p->BrokerOptionSelfCloseSeq);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcOptionSelfCloseActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "OptionSelfCloseActionRef", &p->OptionSelfCloseActionRef);
  set_obj(obj, "OptionSelfCloseRef", &p->OptionSelfCloseRef);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "OptionSelfCloseSysID", &p->OptionSelfCloseSysID);
  set_obj(obj, "ActionFlag", &p->ActionFlag);
  set_obj(obj, "ActionDate", &p->ActionDate);
  set_obj(obj, "ActionTime", &p->ActionTime);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "OptionSelfCloseLocalID", &p->OptionSelfCloseLocalID);
  set_obj(obj, "ActionLocalID", &p->ActionLocalID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "OrderActionStatus", &p->OrderActionStatus);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "StatusMsg", &p->StatusMsg);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryOptionSelfCloseField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "OptionSelfCloseSysID", &p->OptionSelfCloseSysID);
  set_obj(obj, "InsertTimeStart", &p->InsertTimeStart);
  set_obj(obj, "InsertTimeEnd", &p->InsertTimeEnd);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeOptionSelfCloseField *p)
{
  set_obj(obj, "Volume", &p->Volume);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "OptSelfCloseFlag", &p->OptSelfCloseFlag);
  set_obj(obj, "OptionSelfCloseLocalID", &p->OptionSelfCloseLocalID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "OrderSubmitStatus", &p->OrderSubmitStatus);
  set_obj(obj, "NotifySequence", &p->NotifySequence);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "OptionSelfCloseSysID", &p->OptionSelfCloseSysID);
  set_obj(obj, "InsertDate", &p->InsertDate);
  set_obj(obj, "InsertTime", &p->InsertTime);
  set_obj(obj, "CancelTime", &p->CancelTime);
  set_obj(obj, "ExecResult", &p->ExecResult);
  set_obj(obj, "ClearingPartID", &p->ClearingPartID);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryOptionSelfCloseActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeOptionSelfCloseActionField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "OptionSelfCloseSysID", &p->OptionSelfCloseSysID);
  set_obj(obj, "ActionFlag", &p->ActionFlag);
  set_obj(obj, "ActionDate", &p->ActionDate);
  set_obj(obj, "ActionTime", &p->ActionTime);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "OptionSelfCloseLocalID", &p->OptionSelfCloseLocalID);
  set_obj(obj, "ActionLocalID", &p->ActionLocalID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "OrderActionStatus", &p->OrderActionStatus);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "OptSelfCloseFlag", &p->OptSelfCloseFlag);
}
static void set_obj(Local<Object>& obj, CThostFtdcSyncDelaySwapField *p)
{
  set_obj(obj, "DelaySwapSeqNo", &p->DelaySwapSeqNo);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "FromCurrencyID", &p->FromCurrencyID);
  set_obj(obj, "FromAmount", &p->FromAmount);
  set_obj(obj, "FromFrozenSwap", &p->FromFrozenSwap);
  set_obj(obj, "FromRemainSwap", &p->FromRemainSwap);
  set_obj(obj, "ToCurrencyID", &p->ToCurrencyID);
  set_obj(obj, "ToAmount", &p->ToAmount);
}
static void set_obj(Local<Object>& obj, CThostFtdcQrySyncDelaySwapField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "DelaySwapSeqNo", &p->DelaySwapSeqNo);
}
static void set_obj(Local<Object>& obj, CThostFtdcInvestUnitField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "InvestorUnitName", &p->InvestorUnitName);
  set_obj(obj, "InvestorGroupID", &p->InvestorGroupID);
  set_obj(obj, "CommModelID", &p->CommModelID);
  set_obj(obj, "MarginModelID", &p->MarginModelID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryInvestUnitField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcSecAgentCheckModeField *p)
{
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "BrokerSecAgentID", &p->BrokerSecAgentID);
  set_obj(obj, "CheckSelfAccount", &p->CheckSelfAccount);
}
static void set_obj(Local<Object>& obj, CThostFtdcSecAgentTradeInfoField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerSecAgentID", &p->BrokerSecAgentID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "LongCustomerName", &p->LongCustomerName);
}
static void set_obj(Local<Object>& obj, CThostFtdcMarketDataField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "LastPrice", &p->LastPrice);
  set_obj(obj, "PreSettlementPrice", &p->PreSettlementPrice);
  set_obj(obj, "PreClosePrice", &p->PreClosePrice);
  set_obj(obj, "PreOpenInterest", &p->PreOpenInterest);
  set_obj(obj, "OpenPrice", &p->OpenPrice);
  set_obj(obj, "HighestPrice", &p->HighestPrice);
  set_obj(obj, "LowestPrice", &p->LowestPrice);
  set_obj(obj, "Volume", &p->Volume);
  set_obj(obj, "Turnover", &p->Turnover);
  set_obj(obj, "OpenInterest", &p->OpenInterest);
  set_obj(obj, "ClosePrice", &p->ClosePrice);
  set_obj(obj, "SettlementPrice", &p->SettlementPrice);
  set_obj(obj, "UpperLimitPrice", &p->UpperLimitPrice);
  set_obj(obj, "LowerLimitPrice", &p->LowerLimitPrice);
  set_obj(obj, "PreDelta", &p->PreDelta);
  set_obj(obj, "CurrDelta", &p->CurrDelta);
  set_obj(obj, "UpdateTime", &p->UpdateTime);
  set_obj(obj, "UpdateMillisec", &p->UpdateMillisec);
  set_obj(obj, "ActionDay", &p->ActionDay);
}
static void set_obj(Local<Object>& obj, CThostFtdcMarketDataBaseField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PreSettlementPrice", &p->PreSettlementPrice);
  set_obj(obj, "PreClosePrice", &p->PreClosePrice);
  set_obj(obj, "PreOpenInterest", &p->PreOpenInterest);
  set_obj(obj, "PreDelta", &p->PreDelta);
}
static void set_obj(Local<Object>& obj, CThostFtdcMarketDataStaticField *p)
{
  set_obj(obj, "OpenPrice", &p->OpenPrice);
  set_obj(obj, "HighestPrice", &p->HighestPrice);
  set_obj(obj, "LowestPrice", &p->LowestPrice);
  set_obj(obj, "ClosePrice", &p->ClosePrice);
  set_obj(obj, "UpperLimitPrice", &p->UpperLimitPrice);
  set_obj(obj, "LowerLimitPrice", &p->LowerLimitPrice);
  set_obj(obj, "SettlementPrice", &p->SettlementPrice);
  set_obj(obj, "CurrDelta", &p->CurrDelta);
}
static void set_obj(Local<Object>& obj, CThostFtdcMarketDataLastMatchField *p)
{
  set_obj(obj, "LastPrice", &p->LastPrice);
  set_obj(obj, "Volume", &p->Volume);
  set_obj(obj, "Turnover", &p->Turnover);
  set_obj(obj, "OpenInterest", &p->OpenInterest);
}
static void set_obj(Local<Object>& obj, CThostFtdcMarketDataBestPriceField *p)
{
  set_obj(obj, "BidPrice1", &p->BidPrice1);
  set_obj(obj, "BidVolume1", &p->BidVolume1);
  set_obj(obj, "AskPrice1", &p->AskPrice1);
  set_obj(obj, "AskVolume1", &p->AskVolume1);
}
static void set_obj(Local<Object>& obj, CThostFtdcMarketDataBid23Field *p)
{
  set_obj(obj, "BidPrice2", &p->BidPrice2);
  set_obj(obj, "BidVolume2", &p->BidVolume2);
  set_obj(obj, "BidPrice3", &p->BidPrice3);
  set_obj(obj, "BidVolume3", &p->BidVolume3);
}
static void set_obj(Local<Object>& obj, CThostFtdcMarketDataAsk23Field *p)
{
  set_obj(obj, "AskPrice2", &p->AskPrice2);
  set_obj(obj, "AskVolume2", &p->AskVolume2);
  set_obj(obj, "AskPrice3", &p->AskPrice3);
  set_obj(obj, "AskVolume3", &p->AskVolume3);
}
static void set_obj(Local<Object>& obj, CThostFtdcMarketDataBid45Field *p)
{
  set_obj(obj, "BidPrice4", &p->BidPrice4);
  set_obj(obj, "BidVolume4", &p->BidVolume4);
  set_obj(obj, "BidPrice5", &p->BidPrice5);
  set_obj(obj, "BidVolume5", &p->BidVolume5);
}
static void set_obj(Local<Object>& obj, CThostFtdcMarketDataAsk45Field *p)
{
  set_obj(obj, "AskPrice4", &p->AskPrice4);
  set_obj(obj, "AskVolume4", &p->AskVolume4);
  set_obj(obj, "AskPrice5", &p->AskPrice5);
  set_obj(obj, "AskVolume5", &p->AskVolume5);
}
static void set_obj(Local<Object>& obj, CThostFtdcMarketDataUpdateTimeField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "UpdateTime", &p->UpdateTime);
  set_obj(obj, "UpdateMillisec", &p->UpdateMillisec);
  set_obj(obj, "ActionDay", &p->ActionDay);
}
static void set_obj(Local<Object>& obj, CThostFtdcMarketDataExchangeField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcSpecificInstrumentField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
}
static void set_obj(Local<Object>& obj, CThostFtdcInstrumentStatusField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "SettlementGroupID", &p->SettlementGroupID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InstrumentStatus", &p->InstrumentStatus);
  set_obj(obj, "TradingSegmentSN", &p->TradingSegmentSN);
  set_obj(obj, "EnterTime", &p->EnterTime);
  set_obj(obj, "EnterReason", &p->EnterReason);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryInstrumentStatusField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
}
static void set_obj(Local<Object>& obj, CThostFtdcInvestorAccountField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
}
static void set_obj(Local<Object>& obj, CThostFtdcPositionProfitAlgorithmField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Algorithm", &p->Algorithm);
  set_obj(obj, "Memo", &p->Memo);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
}
static void set_obj(Local<Object>& obj, CThostFtdcDiscountField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "Discount", &p->Discount);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryTransferBankField *p)
{
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBrchID", &p->BankBrchID);
}
static void set_obj(Local<Object>& obj, CThostFtdcTransferBankField *p)
{
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBrchID", &p->BankBrchID);
  set_obj(obj, "BankName", &p->BankName);
  set_obj(obj, "IsActive", &p->IsActive);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryInvestorPositionDetailField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcInvestorPositionDetailField *p)
{
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "Direction", &p->Direction);
  set_obj(obj, "OpenDate", &p->OpenDate);
  set_obj(obj, "TradeID", &p->TradeID);
  set_obj(obj, "Volume", &p->Volume);
  set_obj(obj, "OpenPrice", &p->OpenPrice);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "TradeType", &p->TradeType);
  set_obj(obj, "CombInstrumentID", &p->CombInstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "CloseProfitByDate", &p->CloseProfitByDate);
  set_obj(obj, "CloseProfitByTrade", &p->CloseProfitByTrade);
  set_obj(obj, "PositionProfitByDate", &p->PositionProfitByDate);
  set_obj(obj, "PositionProfitByTrade", &p->PositionProfitByTrade);
  set_obj(obj, "Margin", &p->Margin);
  set_obj(obj, "ExchMargin", &p->ExchMargin);
  set_obj(obj, "MarginRateByMoney", &p->MarginRateByMoney);
  set_obj(obj, "MarginRateByVolume", &p->MarginRateByVolume);
  set_obj(obj, "LastSettlementPrice", &p->LastSettlementPrice);
  set_obj(obj, "SettlementPrice", &p->SettlementPrice);
  set_obj(obj, "CloseVolume", &p->CloseVolume);
  set_obj(obj, "CloseAmount", &p->CloseAmount);
  set_obj(obj, "TimeFirstVolume", &p->TimeFirstVolume);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcTradingAccountPasswordField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
}
static void set_obj(Local<Object>& obj, CThostFtdcMDTraderOfferField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "OrderLocalID", &p->OrderLocalID);
  set_obj(obj, "TraderConnectStatus", &p->TraderConnectStatus);
  set_obj(obj, "ConnectRequestDate", &p->ConnectRequestDate);
  set_obj(obj, "ConnectRequestTime", &p->ConnectRequestTime);
  set_obj(obj, "LastReportDate", &p->LastReportDate);
  set_obj(obj, "LastReportTime", &p->LastReportTime);
  set_obj(obj, "ConnectDate", &p->ConnectDate);
  set_obj(obj, "ConnectTime", &p->ConnectTime);
  set_obj(obj, "StartDate", &p->StartDate);
  set_obj(obj, "StartTime", &p->StartTime);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "MaxTradeID", &p->MaxTradeID);
  set_obj(obj, "MaxOrderMessageReference", &p->MaxOrderMessageReference);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryMDTraderOfferField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "TraderID", &p->TraderID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryNoticeField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
}
static void set_obj(Local<Object>& obj, CThostFtdcNoticeField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "Content", &p->Content);
  set_obj(obj, "SequenceLabel", &p->SequenceLabel);
}
static void set_obj(Local<Object>& obj, CThostFtdcUserRightField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "UserRightType", &p->UserRightType);
  set_obj(obj, "IsForbidden", &p->IsForbidden);
}
static void set_obj(Local<Object>& obj, CThostFtdcQrySettlementInfoConfirmField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
}
static void set_obj(Local<Object>& obj, CThostFtdcLoadSettlementInfoField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
}
static void set_obj(Local<Object>& obj, CThostFtdcBrokerWithdrawAlgorithmField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "WithdrawAlgorithm", &p->WithdrawAlgorithm);
  set_obj(obj, "UsingRatio", &p->UsingRatio);
  set_obj(obj, "IncludeCloseProfit", &p->IncludeCloseProfit);
  set_obj(obj, "AllWithoutTrade", &p->AllWithoutTrade);
  set_obj(obj, "AvailIncludeCloseProfit", &p->AvailIncludeCloseProfit);
  set_obj(obj, "IsBrokerUserEvent", &p->IsBrokerUserEvent);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "FundMortgageRatio", &p->FundMortgageRatio);
  set_obj(obj, "BalanceAlgorithm", &p->BalanceAlgorithm);
}
static void set_obj(Local<Object>& obj, CThostFtdcTradingAccountPasswordUpdateV1Field *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "OldPassword", &p->OldPassword);
  set_obj(obj, "NewPassword", &p->NewPassword);
}
static void set_obj(Local<Object>& obj, CThostFtdcTradingAccountPasswordUpdateField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "OldPassword", &p->OldPassword);
  set_obj(obj, "NewPassword", &p->NewPassword);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryCombinationLegField *p)
{
  set_obj(obj, "CombInstrumentID", &p->CombInstrumentID);
  set_obj(obj, "LegID", &p->LegID);
  set_obj(obj, "LegInstrumentID", &p->LegInstrumentID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQrySyncStatusField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
}
static void set_obj(Local<Object>& obj, CThostFtdcCombinationLegField *p)
{
  set_obj(obj, "CombInstrumentID", &p->CombInstrumentID);
  set_obj(obj, "LegID", &p->LegID);
  set_obj(obj, "LegInstrumentID", &p->LegInstrumentID);
  set_obj(obj, "Direction", &p->Direction);
  set_obj(obj, "LegMultiple", &p->LegMultiple);
  set_obj(obj, "ImplyLevel", &p->ImplyLevel);
}
static void set_obj(Local<Object>& obj, CThostFtdcSyncStatusField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "DataSyncStatus", &p->DataSyncStatus);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryLinkManField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
}
static void set_obj(Local<Object>& obj, CThostFtdcLinkManField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "PersonType", &p->PersonType);
  set_obj(obj, "IdentifiedCardType", &p->IdentifiedCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "PersonName", &p->PersonName);
  set_obj(obj, "Telephone", &p->Telephone);
  set_obj(obj, "Address", &p->Address);
  set_obj(obj, "ZipCode", &p->ZipCode);
  set_obj(obj, "Priority", &p->Priority);
  set_obj(obj, "UOAZipCode", &p->UOAZipCode);
  set_obj(obj, "PersonFullName", &p->PersonFullName);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryBrokerUserEventField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "UserEventType", &p->UserEventType);
}
static void set_obj(Local<Object>& obj, CThostFtdcBrokerUserEventField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "UserEventType", &p->UserEventType);
  set_obj(obj, "EventSequenceNo", &p->EventSequenceNo);
  set_obj(obj, "EventDate", &p->EventDate);
  set_obj(obj, "EventTime", &p->EventTime);
  set_obj(obj, "UserEventInfo", &p->UserEventInfo);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryContractBankField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBrchID", &p->BankBrchID);
}
static void set_obj(Local<Object>& obj, CThostFtdcContractBankField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBrchID", &p->BankBrchID);
  set_obj(obj, "BankName", &p->BankName);
}
static void set_obj(Local<Object>& obj, CThostFtdcInvestorPositionCombineDetailField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "OpenDate", &p->OpenDate);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "ComTradeID", &p->ComTradeID);
  set_obj(obj, "TradeID", &p->TradeID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "Direction", &p->Direction);
  set_obj(obj, "TotalAmt", &p->TotalAmt);
  set_obj(obj, "Margin", &p->Margin);
  set_obj(obj, "ExchMargin", &p->ExchMargin);
  set_obj(obj, "MarginRateByMoney", &p->MarginRateByMoney);
  set_obj(obj, "MarginRateByVolume", &p->MarginRateByVolume);
  set_obj(obj, "LegID", &p->LegID);
  set_obj(obj, "LegMultiple", &p->LegMultiple);
  set_obj(obj, "CombInstrumentID", &p->CombInstrumentID);
  set_obj(obj, "TradeGroupID", &p->TradeGroupID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcParkedOrderField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "OrderRef", &p->OrderRef);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "OrderPriceType", &p->OrderPriceType);
  set_obj(obj, "Direction", &p->Direction);
  set_obj(obj, "CombOffsetFlag", &p->CombOffsetFlag);
  set_obj(obj, "CombHedgeFlag", &p->CombHedgeFlag);
  set_obj(obj, "LimitPrice", &p->LimitPrice);
  set_obj(obj, "VolumeTotalOriginal", &p->VolumeTotalOriginal);
  set_obj(obj, "TimeCondition", &p->TimeCondition);
  set_obj(obj, "GTDDate", &p->GTDDate);
  set_obj(obj, "VolumeCondition", &p->VolumeCondition);
  set_obj(obj, "MinVolume", &p->MinVolume);
  set_obj(obj, "ContingentCondition", &p->ContingentCondition);
  set_obj(obj, "StopPrice", &p->StopPrice);
  set_obj(obj, "ForceCloseReason", &p->ForceCloseReason);
  set_obj(obj, "IsAutoSuspend", &p->IsAutoSuspend);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "UserForceClose", &p->UserForceClose);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParkedOrderID", &p->ParkedOrderID);
  set_obj(obj, "UserType", &p->UserType);
  set_obj(obj, "Status", &p->Status);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
  set_obj(obj, "IsSwapOrder", &p->IsSwapOrder);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcParkedOrderActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "OrderActionRef", &p->OrderActionRef);
  set_obj(obj, "OrderRef", &p->OrderRef);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "OrderSysID", &p->OrderSysID);
  set_obj(obj, "ActionFlag", &p->ActionFlag);
  set_obj(obj, "LimitPrice", &p->LimitPrice);
  set_obj(obj, "VolumeChange", &p->VolumeChange);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ParkedOrderActionID", &p->ParkedOrderActionID);
  set_obj(obj, "UserType", &p->UserType);
  set_obj(obj, "Status", &p->Status);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryParkedOrderField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryParkedOrderActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcRemoveParkedOrderField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "ParkedOrderID", &p->ParkedOrderID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcRemoveParkedOrderActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "ParkedOrderActionID", &p->ParkedOrderActionID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcInvestorWithdrawAlgorithmField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "UsingRatio", &p->UsingRatio);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "FundMortgageRatio", &p->FundMortgageRatio);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryInvestorPositionCombineDetailField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "CombInstrumentID", &p->CombInstrumentID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcMarketDataAveragePriceField *p)
{
  set_obj(obj, "AveragePrice", &p->AveragePrice);
}
static void set_obj(Local<Object>& obj, CThostFtdcVerifyInvestorPasswordField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "Password", &p->Password);
}
static void set_obj(Local<Object>& obj, CThostFtdcUserIPField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "IPMask", &p->IPMask);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcTradingNoticeInfoField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "SendTime", &p->SendTime);
  set_obj(obj, "FieldContent", &p->FieldContent);
  set_obj(obj, "SequenceSeries", &p->SequenceSeries);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcTradingNoticeField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "SequenceSeries", &p->SequenceSeries);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "SendTime", &p->SendTime);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "FieldContent", &p->FieldContent);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryTradingNoticeField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryErrOrderField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
}
static void set_obj(Local<Object>& obj, CThostFtdcErrOrderField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "OrderRef", &p->OrderRef);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "OrderPriceType", &p->OrderPriceType);
  set_obj(obj, "Direction", &p->Direction);
  set_obj(obj, "CombOffsetFlag", &p->CombOffsetFlag);
  set_obj(obj, "CombHedgeFlag", &p->CombHedgeFlag);
  set_obj(obj, "LimitPrice", &p->LimitPrice);
  set_obj(obj, "VolumeTotalOriginal", &p->VolumeTotalOriginal);
  set_obj(obj, "TimeCondition", &p->TimeCondition);
  set_obj(obj, "GTDDate", &p->GTDDate);
  set_obj(obj, "VolumeCondition", &p->VolumeCondition);
  set_obj(obj, "MinVolume", &p->MinVolume);
  set_obj(obj, "ContingentCondition", &p->ContingentCondition);
  set_obj(obj, "StopPrice", &p->StopPrice);
  set_obj(obj, "ForceCloseReason", &p->ForceCloseReason);
  set_obj(obj, "IsAutoSuspend", &p->IsAutoSuspend);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "UserForceClose", &p->UserForceClose);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
  set_obj(obj, "IsSwapOrder", &p->IsSwapOrder);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcErrorConditionalOrderField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "OrderRef", &p->OrderRef);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "OrderPriceType", &p->OrderPriceType);
  set_obj(obj, "Direction", &p->Direction);
  set_obj(obj, "CombOffsetFlag", &p->CombOffsetFlag);
  set_obj(obj, "CombHedgeFlag", &p->CombHedgeFlag);
  set_obj(obj, "LimitPrice", &p->LimitPrice);
  set_obj(obj, "VolumeTotalOriginal", &p->VolumeTotalOriginal);
  set_obj(obj, "TimeCondition", &p->TimeCondition);
  set_obj(obj, "GTDDate", &p->GTDDate);
  set_obj(obj, "VolumeCondition", &p->VolumeCondition);
  set_obj(obj, "MinVolume", &p->MinVolume);
  set_obj(obj, "ContingentCondition", &p->ContingentCondition);
  set_obj(obj, "StopPrice", &p->StopPrice);
  set_obj(obj, "ForceCloseReason", &p->ForceCloseReason);
  set_obj(obj, "IsAutoSuspend", &p->IsAutoSuspend);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "OrderLocalID", &p->OrderLocalID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "ExchangeInstID", &p->ExchangeInstID);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "OrderSubmitStatus", &p->OrderSubmitStatus);
  set_obj(obj, "NotifySequence", &p->NotifySequence);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "OrderSysID", &p->OrderSysID);
  set_obj(obj, "OrderSource", &p->OrderSource);
  set_obj(obj, "OrderStatus", &p->OrderStatus);
  set_obj(obj, "OrderType", &p->OrderType);
  set_obj(obj, "VolumeTraded", &p->VolumeTraded);
  set_obj(obj, "VolumeTotal", &p->VolumeTotal);
  set_obj(obj, "InsertDate", &p->InsertDate);
  set_obj(obj, "InsertTime", &p->InsertTime);
  set_obj(obj, "ActiveTime", &p->ActiveTime);
  set_obj(obj, "SuspendTime", &p->SuspendTime);
  set_obj(obj, "UpdateTime", &p->UpdateTime);
  set_obj(obj, "CancelTime", &p->CancelTime);
  set_obj(obj, "ActiveTraderID", &p->ActiveTraderID);
  set_obj(obj, "ClearingPartID", &p->ClearingPartID);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "UserProductInfo", &p->UserProductInfo);
  set_obj(obj, "StatusMsg", &p->StatusMsg);
  set_obj(obj, "UserForceClose", &p->UserForceClose);
  set_obj(obj, "ActiveUserID", &p->ActiveUserID);
  set_obj(obj, "BrokerOrderSeq", &p->BrokerOrderSeq);
  set_obj(obj, "RelativeOrderSysID", &p->RelativeOrderSysID);
  set_obj(obj, "ZCETotalTradedVolume", &p->ZCETotalTradedVolume);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
  set_obj(obj, "IsSwapOrder", &p->IsSwapOrder);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryErrOrderActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
}
static void set_obj(Local<Object>& obj, CThostFtdcErrOrderActionField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "OrderActionRef", &p->OrderActionRef);
  set_obj(obj, "OrderRef", &p->OrderRef);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "FrontID", &p->FrontID);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "OrderSysID", &p->OrderSysID);
  set_obj(obj, "ActionFlag", &p->ActionFlag);
  set_obj(obj, "LimitPrice", &p->LimitPrice);
  set_obj(obj, "VolumeChange", &p->VolumeChange);
  set_obj(obj, "ActionDate", &p->ActionDate);
  set_obj(obj, "ActionTime", &p->ActionTime);
  set_obj(obj, "TraderID", &p->TraderID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "OrderLocalID", &p->OrderLocalID);
  set_obj(obj, "ActionLocalID", &p->ActionLocalID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ClientID", &p->ClientID);
  set_obj(obj, "BusinessUnit", &p->BusinessUnit);
  set_obj(obj, "OrderActionStatus", &p->OrderActionStatus);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "StatusMsg", &p->StatusMsg);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "BranchID", &p->BranchID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
  set_obj(obj, "IPAddress", &p->IPAddress);
  set_obj(obj, "MacAddress", &p->MacAddress);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryExchangeSequenceField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcExchangeSequenceField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "MarketStatus", &p->MarketStatus);
}
static void set_obj(Local<Object>& obj, CThostFtdcQueryMaxOrderVolumeWithPriceField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "Direction", &p->Direction);
  set_obj(obj, "OffsetFlag", &p->OffsetFlag);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "MaxVolume", &p->MaxVolume);
  set_obj(obj, "Price", &p->Price);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryBrokerTradingParamsField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "AccountID", &p->AccountID);
}
static void set_obj(Local<Object>& obj, CThostFtdcBrokerTradingParamsField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "MarginPriceType", &p->MarginPriceType);
  set_obj(obj, "Algorithm", &p->Algorithm);
  set_obj(obj, "AvailIncludeCloseProfit", &p->AvailIncludeCloseProfit);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "OptionRoyaltyPriceType", &p->OptionRoyaltyPriceType);
  set_obj(obj, "AccountID", &p->AccountID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryBrokerTradingAlgosField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
}
static void set_obj(Local<Object>& obj, CThostFtdcBrokerTradingAlgosField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "HandlePositionAlgoID", &p->HandlePositionAlgoID);
  set_obj(obj, "FindMarginRateAlgoID", &p->FindMarginRateAlgoID);
  set_obj(obj, "HandleTradingAccountAlgoID", &p->HandleTradingAccountAlgoID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQueryBrokerDepositField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcBrokerDepositField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "PreBalance", &p->PreBalance);
  set_obj(obj, "CurrMargin", &p->CurrMargin);
  set_obj(obj, "CloseProfit", &p->CloseProfit);
  set_obj(obj, "Balance", &p->Balance);
  set_obj(obj, "Deposit", &p->Deposit);
  set_obj(obj, "Withdraw", &p->Withdraw);
  set_obj(obj, "Available", &p->Available);
  set_obj(obj, "Reserve", &p->Reserve);
  set_obj(obj, "FrozenMargin", &p->FrozenMargin);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryCFMMCBrokerKeyField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
}
static void set_obj(Local<Object>& obj, CThostFtdcCFMMCBrokerKeyField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "CreateDate", &p->CreateDate);
  set_obj(obj, "CreateTime", &p->CreateTime);
  set_obj(obj, "KeyID", &p->KeyID);
  set_obj(obj, "CurrentKey", &p->CurrentKey);
  set_obj(obj, "KeyKind", &p->KeyKind);
}
static void set_obj(Local<Object>& obj, CThostFtdcCFMMCTradingAccountKeyField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "KeyID", &p->KeyID);
  set_obj(obj, "CurrentKey", &p->CurrentKey);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryCFMMCTradingAccountKeyField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
}
static void set_obj(Local<Object>& obj, CThostFtdcBrokerUserOTPParamField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "OTPVendorsID", &p->OTPVendorsID);
  set_obj(obj, "SerialNumber", &p->SerialNumber);
  set_obj(obj, "AuthKey", &p->AuthKey);
  set_obj(obj, "LastDrift", &p->LastDrift);
  set_obj(obj, "LastSuccess", &p->LastSuccess);
  set_obj(obj, "OTPType", &p->OTPType);
}
static void set_obj(Local<Object>& obj, CThostFtdcManualSyncBrokerUserOTPField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "OTPType", &p->OTPType);
  set_obj(obj, "FirstOTP", &p->FirstOTP);
  set_obj(obj, "SecondOTP", &p->SecondOTP);
}
static void set_obj(Local<Object>& obj, CThostFtdcCommRateModelField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "CommModelID", &p->CommModelID);
  set_obj(obj, "CommModelName", &p->CommModelName);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryCommRateModelField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "CommModelID", &p->CommModelID);
}
static void set_obj(Local<Object>& obj, CThostFtdcMarginModelField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "MarginModelID", &p->MarginModelID);
  set_obj(obj, "MarginModelName", &p->MarginModelName);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryMarginModelField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "MarginModelID", &p->MarginModelID);
}
static void set_obj(Local<Object>& obj, CThostFtdcEWarrantOffsetField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "Direction", &p->Direction);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "Volume", &p->Volume);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryEWarrantOffsetField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InstrumentID", &p->InstrumentID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryInvestorProductGroupMarginField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "ProductGroupID", &p->ProductGroupID);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcInvestorProductGroupMarginField *p)
{
  set_obj(obj, "ProductGroupID", &p->ProductGroupID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "SettlementID", &p->SettlementID);
  set_obj(obj, "FrozenMargin", &p->FrozenMargin);
  set_obj(obj, "LongFrozenMargin", &p->LongFrozenMargin);
  set_obj(obj, "ShortFrozenMargin", &p->ShortFrozenMargin);
  set_obj(obj, "UseMargin", &p->UseMargin);
  set_obj(obj, "LongUseMargin", &p->LongUseMargin);
  set_obj(obj, "ShortUseMargin", &p->ShortUseMargin);
  set_obj(obj, "ExchMargin", &p->ExchMargin);
  set_obj(obj, "LongExchMargin", &p->LongExchMargin);
  set_obj(obj, "ShortExchMargin", &p->ShortExchMargin);
  set_obj(obj, "CloseProfit", &p->CloseProfit);
  set_obj(obj, "FrozenCommission", &p->FrozenCommission);
  set_obj(obj, "Commission", &p->Commission);
  set_obj(obj, "FrozenCash", &p->FrozenCash);
  set_obj(obj, "CashIn", &p->CashIn);
  set_obj(obj, "PositionProfit", &p->PositionProfit);
  set_obj(obj, "OffsetAmount", &p->OffsetAmount);
  set_obj(obj, "LongOffsetAmount", &p->LongOffsetAmount);
  set_obj(obj, "ShortOffsetAmount", &p->ShortOffsetAmount);
  set_obj(obj, "ExchOffsetAmount", &p->ExchOffsetAmount);
  set_obj(obj, "LongExchOffsetAmount", &p->LongExchOffsetAmount);
  set_obj(obj, "ShortExchOffsetAmount", &p->ShortExchOffsetAmount);
  set_obj(obj, "HedgeFlag", &p->HedgeFlag);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQueryCFMMCTradingAccountTokenField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "InvestUnitID", &p->InvestUnitID);
}
static void set_obj(Local<Object>& obj, CThostFtdcCFMMCTradingAccountTokenField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "ParticipantID", &p->ParticipantID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "KeyID", &p->KeyID);
  set_obj(obj, "Token", &p->Token);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryProductGroupField *p)
{
  set_obj(obj, "ProductID", &p->ProductID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
}
static void set_obj(Local<Object>& obj, CThostFtdcProductGroupField *p)
{
  set_obj(obj, "ProductID", &p->ProductID);
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "ProductGroupID", &p->ProductGroupID);
}
static void set_obj(Local<Object>& obj, CThostFtdcBulletinField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "BulletinID", &p->BulletinID);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "NewsType", &p->NewsType);
  set_obj(obj, "NewsUrgency", &p->NewsUrgency);
  set_obj(obj, "SendTime", &p->SendTime);
  set_obj(obj, "Abstract", &p->Abstract);
  set_obj(obj, "ComeFrom", &p->ComeFrom);
  set_obj(obj, "Content", &p->Content);
  set_obj(obj, "URLLink", &p->URLLink);
  set_obj(obj, "MarketID", &p->MarketID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryBulletinField *p)
{
  set_obj(obj, "ExchangeID", &p->ExchangeID);
  set_obj(obj, "BulletinID", &p->BulletinID);
  set_obj(obj, "SequenceNo", &p->SequenceNo);
  set_obj(obj, "NewsType", &p->NewsType);
  set_obj(obj, "NewsUrgency", &p->NewsUrgency);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqOpenAccountField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "Gender", &p->Gender);
  set_obj(obj, "CountryCode", &p->CountryCode);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "Address", &p->Address);
  set_obj(obj, "ZipCode", &p->ZipCode);
  set_obj(obj, "Telephone", &p->Telephone);
  set_obj(obj, "MobilePhone", &p->MobilePhone);
  set_obj(obj, "Fax", &p->Fax);
  set_obj(obj, "EMail", &p->EMail);
  set_obj(obj, "MoneyAccountStatus", &p->MoneyAccountStatus);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankPassWord", &p->BankPassWord);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "VerifyCertNoFlag", &p->VerifyCertNoFlag);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "CashExchangeCode", &p->CashExchangeCode);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "BankAccType", &p->BankAccType);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BankSecuAccType", &p->BankSecuAccType);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "BankSecuAcc", &p->BankSecuAcc);
  set_obj(obj, "BankPwdFlag", &p->BankPwdFlag);
  set_obj(obj, "SecuPwdFlag", &p->SecuPwdFlag);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "LongCustomerName", &p->LongCustomerName);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqCancelAccountField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "Gender", &p->Gender);
  set_obj(obj, "CountryCode", &p->CountryCode);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "Address", &p->Address);
  set_obj(obj, "ZipCode", &p->ZipCode);
  set_obj(obj, "Telephone", &p->Telephone);
  set_obj(obj, "MobilePhone", &p->MobilePhone);
  set_obj(obj, "Fax", &p->Fax);
  set_obj(obj, "EMail", &p->EMail);
  set_obj(obj, "MoneyAccountStatus", &p->MoneyAccountStatus);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankPassWord", &p->BankPassWord);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "VerifyCertNoFlag", &p->VerifyCertNoFlag);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "CashExchangeCode", &p->CashExchangeCode);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "BankAccType", &p->BankAccType);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BankSecuAccType", &p->BankSecuAccType);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "BankSecuAcc", &p->BankSecuAcc);
  set_obj(obj, "BankPwdFlag", &p->BankPwdFlag);
  set_obj(obj, "SecuPwdFlag", &p->SecuPwdFlag);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "LongCustomerName", &p->LongCustomerName);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqChangeAccountField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "Gender", &p->Gender);
  set_obj(obj, "CountryCode", &p->CountryCode);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "Address", &p->Address);
  set_obj(obj, "ZipCode", &p->ZipCode);
  set_obj(obj, "Telephone", &p->Telephone);
  set_obj(obj, "MobilePhone", &p->MobilePhone);
  set_obj(obj, "Fax", &p->Fax);
  set_obj(obj, "EMail", &p->EMail);
  set_obj(obj, "MoneyAccountStatus", &p->MoneyAccountStatus);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankPassWord", &p->BankPassWord);
  set_obj(obj, "NewBankAccount", &p->NewBankAccount);
  set_obj(obj, "NewBankPassWord", &p->NewBankPassWord);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "BankAccType", &p->BankAccType);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "VerifyCertNoFlag", &p->VerifyCertNoFlag);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "BankPwdFlag", &p->BankPwdFlag);
  set_obj(obj, "SecuPwdFlag", &p->SecuPwdFlag);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "LongCustomerName", &p->LongCustomerName);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqTransferField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankPassWord", &p->BankPassWord);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "FutureSerial", &p->FutureSerial);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "VerifyCertNoFlag", &p->VerifyCertNoFlag);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "TradeAmount", &p->TradeAmount);
  set_obj(obj, "FutureFetchAmount", &p->FutureFetchAmount);
  set_obj(obj, "FeePayFlag", &p->FeePayFlag);
  set_obj(obj, "CustFee", &p->CustFee);
  set_obj(obj, "BrokerFee", &p->BrokerFee);
  set_obj(obj, "Message", &p->Message);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "BankAccType", &p->BankAccType);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BankSecuAccType", &p->BankSecuAccType);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "BankSecuAcc", &p->BankSecuAcc);
  set_obj(obj, "BankPwdFlag", &p->BankPwdFlag);
  set_obj(obj, "SecuPwdFlag", &p->SecuPwdFlag);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "TransferStatus", &p->TransferStatus);
  set_obj(obj, "LongCustomerName", &p->LongCustomerName);
}
static void set_obj(Local<Object>& obj, CThostFtdcRspTransferField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankPassWord", &p->BankPassWord);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "FutureSerial", &p->FutureSerial);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "VerifyCertNoFlag", &p->VerifyCertNoFlag);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "TradeAmount", &p->TradeAmount);
  set_obj(obj, "FutureFetchAmount", &p->FutureFetchAmount);
  set_obj(obj, "FeePayFlag", &p->FeePayFlag);
  set_obj(obj, "CustFee", &p->CustFee);
  set_obj(obj, "BrokerFee", &p->BrokerFee);
  set_obj(obj, "Message", &p->Message);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "BankAccType", &p->BankAccType);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BankSecuAccType", &p->BankSecuAccType);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "BankSecuAcc", &p->BankSecuAcc);
  set_obj(obj, "BankPwdFlag", &p->BankPwdFlag);
  set_obj(obj, "SecuPwdFlag", &p->SecuPwdFlag);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "TransferStatus", &p->TransferStatus);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
  set_obj(obj, "LongCustomerName", &p->LongCustomerName);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqRepealField *p)
{
  set_obj(obj, "RepealTimeInterval", &p->RepealTimeInterval);
  set_obj(obj, "RepealedTimes", &p->RepealedTimes);
  set_obj(obj, "BankRepealFlag", &p->BankRepealFlag);
  set_obj(obj, "BrokerRepealFlag", &p->BrokerRepealFlag);
  set_obj(obj, "PlateRepealSerial", &p->PlateRepealSerial);
  set_obj(obj, "BankRepealSerial", &p->BankRepealSerial);
  set_obj(obj, "FutureRepealSerial", &p->FutureRepealSerial);
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankPassWord", &p->BankPassWord);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "FutureSerial", &p->FutureSerial);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "VerifyCertNoFlag", &p->VerifyCertNoFlag);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "TradeAmount", &p->TradeAmount);
  set_obj(obj, "FutureFetchAmount", &p->FutureFetchAmount);
  set_obj(obj, "FeePayFlag", &p->FeePayFlag);
  set_obj(obj, "CustFee", &p->CustFee);
  set_obj(obj, "BrokerFee", &p->BrokerFee);
  set_obj(obj, "Message", &p->Message);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "BankAccType", &p->BankAccType);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BankSecuAccType", &p->BankSecuAccType);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "BankSecuAcc", &p->BankSecuAcc);
  set_obj(obj, "BankPwdFlag", &p->BankPwdFlag);
  set_obj(obj, "SecuPwdFlag", &p->SecuPwdFlag);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "TransferStatus", &p->TransferStatus);
  set_obj(obj, "LongCustomerName", &p->LongCustomerName);
}
static void set_obj(Local<Object>& obj, CThostFtdcRspRepealField *p)
{
  set_obj(obj, "RepealTimeInterval", &p->RepealTimeInterval);
  set_obj(obj, "RepealedTimes", &p->RepealedTimes);
  set_obj(obj, "BankRepealFlag", &p->BankRepealFlag);
  set_obj(obj, "BrokerRepealFlag", &p->BrokerRepealFlag);
  set_obj(obj, "PlateRepealSerial", &p->PlateRepealSerial);
  set_obj(obj, "BankRepealSerial", &p->BankRepealSerial);
  set_obj(obj, "FutureRepealSerial", &p->FutureRepealSerial);
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankPassWord", &p->BankPassWord);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "FutureSerial", &p->FutureSerial);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "VerifyCertNoFlag", &p->VerifyCertNoFlag);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "TradeAmount", &p->TradeAmount);
  set_obj(obj, "FutureFetchAmount", &p->FutureFetchAmount);
  set_obj(obj, "FeePayFlag", &p->FeePayFlag);
  set_obj(obj, "CustFee", &p->CustFee);
  set_obj(obj, "BrokerFee", &p->BrokerFee);
  set_obj(obj, "Message", &p->Message);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "BankAccType", &p->BankAccType);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BankSecuAccType", &p->BankSecuAccType);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "BankSecuAcc", &p->BankSecuAcc);
  set_obj(obj, "BankPwdFlag", &p->BankPwdFlag);
  set_obj(obj, "SecuPwdFlag", &p->SecuPwdFlag);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "TransferStatus", &p->TransferStatus);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
  set_obj(obj, "LongCustomerName", &p->LongCustomerName);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqQueryAccountField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankPassWord", &p->BankPassWord);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "FutureSerial", &p->FutureSerial);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "VerifyCertNoFlag", &p->VerifyCertNoFlag);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "BankAccType", &p->BankAccType);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BankSecuAccType", &p->BankSecuAccType);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "BankSecuAcc", &p->BankSecuAcc);
  set_obj(obj, "BankPwdFlag", &p->BankPwdFlag);
  set_obj(obj, "SecuPwdFlag", &p->SecuPwdFlag);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "LongCustomerName", &p->LongCustomerName);
}
static void set_obj(Local<Object>& obj, CThostFtdcRspQueryAccountField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankPassWord", &p->BankPassWord);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "FutureSerial", &p->FutureSerial);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "VerifyCertNoFlag", &p->VerifyCertNoFlag);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "BankAccType", &p->BankAccType);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BankSecuAccType", &p->BankSecuAccType);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "BankSecuAcc", &p->BankSecuAcc);
  set_obj(obj, "BankPwdFlag", &p->BankPwdFlag);
  set_obj(obj, "SecuPwdFlag", &p->SecuPwdFlag);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "BankUseAmount", &p->BankUseAmount);
  set_obj(obj, "BankFetchAmount", &p->BankFetchAmount);
  set_obj(obj, "LongCustomerName", &p->LongCustomerName);
}
static void set_obj(Local<Object>& obj, CThostFtdcFutureSignIOField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "TID", &p->TID);
}
static void set_obj(Local<Object>& obj, CThostFtdcRspFutureSignInField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
  set_obj(obj, "PinKey", &p->PinKey);
  set_obj(obj, "MacKey", &p->MacKey);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqFutureSignOutField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "TID", &p->TID);
}
static void set_obj(Local<Object>& obj, CThostFtdcRspFutureSignOutField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqQueryTradeResultBySerialField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "Reference", &p->Reference);
  set_obj(obj, "RefrenceIssureType", &p->RefrenceIssureType);
  set_obj(obj, "RefrenceIssure", &p->RefrenceIssure);
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankPassWord", &p->BankPassWord);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "TradeAmount", &p->TradeAmount);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "LongCustomerName", &p->LongCustomerName);
}
static void set_obj(Local<Object>& obj, CThostFtdcRspQueryTradeResultBySerialField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
  set_obj(obj, "Reference", &p->Reference);
  set_obj(obj, "RefrenceIssureType", &p->RefrenceIssureType);
  set_obj(obj, "RefrenceIssure", &p->RefrenceIssure);
  set_obj(obj, "OriginReturnCode", &p->OriginReturnCode);
  set_obj(obj, "OriginDescrInfoForReturnCode", &p->OriginDescrInfoForReturnCode);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankPassWord", &p->BankPassWord);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "TradeAmount", &p->TradeAmount);
  set_obj(obj, "Digest", &p->Digest);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqDayEndFileReadyField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "FileBusinessCode", &p->FileBusinessCode);
  set_obj(obj, "Digest", &p->Digest);
}
static void set_obj(Local<Object>& obj, CThostFtdcReturnResultField *p)
{
  set_obj(obj, "ReturnCode", &p->ReturnCode);
  set_obj(obj, "DescrInfoForReturnCode", &p->DescrInfoForReturnCode);
}
static void set_obj(Local<Object>& obj, CThostFtdcVerifyFuturePasswordField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankPassWord", &p->BankPassWord);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
}
static void set_obj(Local<Object>& obj, CThostFtdcVerifyCustInfoField *p)
{
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "LongCustomerName", &p->LongCustomerName);
}
static void set_obj(Local<Object>& obj, CThostFtdcVerifyFuturePasswordAndCustInfoField *p)
{
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "LongCustomerName", &p->LongCustomerName);
}
static void set_obj(Local<Object>& obj, CThostFtdcDepositResultInformField *p)
{
  set_obj(obj, "DepositSeqNo", &p->DepositSeqNo);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "Deposit", &p->Deposit);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "ReturnCode", &p->ReturnCode);
  set_obj(obj, "DescrInfoForReturnCode", &p->DescrInfoForReturnCode);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqSyncKeyField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Message", &p->Message);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "TID", &p->TID);
}
static void set_obj(Local<Object>& obj, CThostFtdcRspSyncKeyField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Message", &p->Message);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
}
static void set_obj(Local<Object>& obj, CThostFtdcNotifyQueryAccountField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankPassWord", &p->BankPassWord);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "FutureSerial", &p->FutureSerial);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "VerifyCertNoFlag", &p->VerifyCertNoFlag);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "BankAccType", &p->BankAccType);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BankSecuAccType", &p->BankSecuAccType);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "BankSecuAcc", &p->BankSecuAcc);
  set_obj(obj, "BankPwdFlag", &p->BankPwdFlag);
  set_obj(obj, "SecuPwdFlag", &p->SecuPwdFlag);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "BankUseAmount", &p->BankUseAmount);
  set_obj(obj, "BankFetchAmount", &p->BankFetchAmount);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
  set_obj(obj, "LongCustomerName", &p->LongCustomerName);
}
static void set_obj(Local<Object>& obj, CThostFtdcTransferSerialField *p)
{
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BankAccType", &p->BankAccType);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "FutureAccType", &p->FutureAccType);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "InvestorID", &p->InvestorID);
  set_obj(obj, "FutureSerial", &p->FutureSerial);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "TradeAmount", &p->TradeAmount);
  set_obj(obj, "CustFee", &p->CustFee);
  set_obj(obj, "BrokerFee", &p->BrokerFee);
  set_obj(obj, "AvailabilityFlag", &p->AvailabilityFlag);
  set_obj(obj, "OperatorCode", &p->OperatorCode);
  set_obj(obj, "BankNewAccount", &p->BankNewAccount);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryTransferSerialField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
}
static void set_obj(Local<Object>& obj, CThostFtdcNotifyFutureSignInField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
  set_obj(obj, "PinKey", &p->PinKey);
  set_obj(obj, "MacKey", &p->MacKey);
}
static void set_obj(Local<Object>& obj, CThostFtdcNotifyFutureSignOutField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
}
static void set_obj(Local<Object>& obj, CThostFtdcNotifySyncKeyField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Message", &p->Message);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "RequestID", &p->RequestID);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryAccountregisterField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
}
static void set_obj(Local<Object>& obj, CThostFtdcAccountregisterField *p)
{
  set_obj(obj, "TradeDay", &p->TradeDay);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "OpenOrDestroy", &p->OpenOrDestroy);
  set_obj(obj, "RegDate", &p->RegDate);
  set_obj(obj, "OutDate", &p->OutDate);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "BankAccType", &p->BankAccType);
  set_obj(obj, "LongCustomerName", &p->LongCustomerName);
}
static void set_obj(Local<Object>& obj, CThostFtdcOpenAccountField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "Gender", &p->Gender);
  set_obj(obj, "CountryCode", &p->CountryCode);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "Address", &p->Address);
  set_obj(obj, "ZipCode", &p->ZipCode);
  set_obj(obj, "Telephone", &p->Telephone);
  set_obj(obj, "MobilePhone", &p->MobilePhone);
  set_obj(obj, "Fax", &p->Fax);
  set_obj(obj, "EMail", &p->EMail);
  set_obj(obj, "MoneyAccountStatus", &p->MoneyAccountStatus);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankPassWord", &p->BankPassWord);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "VerifyCertNoFlag", &p->VerifyCertNoFlag);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "CashExchangeCode", &p->CashExchangeCode);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "BankAccType", &p->BankAccType);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BankSecuAccType", &p->BankSecuAccType);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "BankSecuAcc", &p->BankSecuAcc);
  set_obj(obj, "BankPwdFlag", &p->BankPwdFlag);
  set_obj(obj, "SecuPwdFlag", &p->SecuPwdFlag);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
  set_obj(obj, "LongCustomerName", &p->LongCustomerName);
}
static void set_obj(Local<Object>& obj, CThostFtdcCancelAccountField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "Gender", &p->Gender);
  set_obj(obj, "CountryCode", &p->CountryCode);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "Address", &p->Address);
  set_obj(obj, "ZipCode", &p->ZipCode);
  set_obj(obj, "Telephone", &p->Telephone);
  set_obj(obj, "MobilePhone", &p->MobilePhone);
  set_obj(obj, "Fax", &p->Fax);
  set_obj(obj, "EMail", &p->EMail);
  set_obj(obj, "MoneyAccountStatus", &p->MoneyAccountStatus);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankPassWord", &p->BankPassWord);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "VerifyCertNoFlag", &p->VerifyCertNoFlag);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "CashExchangeCode", &p->CashExchangeCode);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "BankAccType", &p->BankAccType);
  set_obj(obj, "DeviceID", &p->DeviceID);
  set_obj(obj, "BankSecuAccType", &p->BankSecuAccType);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "BankSecuAcc", &p->BankSecuAcc);
  set_obj(obj, "BankPwdFlag", &p->BankPwdFlag);
  set_obj(obj, "SecuPwdFlag", &p->SecuPwdFlag);
  set_obj(obj, "OperNo", &p->OperNo);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
  set_obj(obj, "LongCustomerName", &p->LongCustomerName);
}
static void set_obj(Local<Object>& obj, CThostFtdcChangeAccountField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "Gender", &p->Gender);
  set_obj(obj, "CountryCode", &p->CountryCode);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "Address", &p->Address);
  set_obj(obj, "ZipCode", &p->ZipCode);
  set_obj(obj, "Telephone", &p->Telephone);
  set_obj(obj, "MobilePhone", &p->MobilePhone);
  set_obj(obj, "Fax", &p->Fax);
  set_obj(obj, "EMail", &p->EMail);
  set_obj(obj, "MoneyAccountStatus", &p->MoneyAccountStatus);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankPassWord", &p->BankPassWord);
  set_obj(obj, "NewBankAccount", &p->NewBankAccount);
  set_obj(obj, "NewBankPassWord", &p->NewBankPassWord);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "BankAccType", &p->BankAccType);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "VerifyCertNoFlag", &p->VerifyCertNoFlag);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "BankPwdFlag", &p->BankPwdFlag);
  set_obj(obj, "SecuPwdFlag", &p->SecuPwdFlag);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
  set_obj(obj, "LongCustomerName", &p->LongCustomerName);
}
static void set_obj(Local<Object>& obj, CThostFtdcSecAgentACIDMapField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "BrokerSecAgentID", &p->BrokerSecAgentID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQrySecAgentACIDMapField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
}
static void set_obj(Local<Object>& obj, CThostFtdcUserRightsAssignField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "DRIdentityID", &p->DRIdentityID);
}
static void set_obj(Local<Object>& obj, CThostFtdcBrokerUserRightAssignField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "DRIdentityID", &p->DRIdentityID);
  set_obj(obj, "Tradeable", &p->Tradeable);
}
static void set_obj(Local<Object>& obj, CThostFtdcDRTransferField *p)
{
  set_obj(obj, "OrigDRIdentityID", &p->OrigDRIdentityID);
  set_obj(obj, "DestDRIdentityID", &p->DestDRIdentityID);
  set_obj(obj, "OrigBrokerID", &p->OrigBrokerID);
  set_obj(obj, "DestBrokerID", &p->DestBrokerID);
}
static void set_obj(Local<Object>& obj, CThostFtdcFensUserInfoField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "LoginMode", &p->LoginMode);
}
static void set_obj(Local<Object>& obj, CThostFtdcCurrTransferIdentityField *p)
{
  set_obj(obj, "IdentityID", &p->IdentityID);
}
static void set_obj(Local<Object>& obj, CThostFtdcLoginForbiddenUserField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "IPAddress", &p->IPAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryLoginForbiddenUserField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
}
static void set_obj(Local<Object>& obj, CThostFtdcMulticastGroupInfoField *p)
{
  set_obj(obj, "GroupIP", &p->GroupIP);
  set_obj(obj, "GroupPort", &p->GroupPort);
  set_obj(obj, "SourceIP", &p->SourceIP);
}
static void set_obj(Local<Object>& obj, CThostFtdcTradingAccountReserveField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Reserve", &p->Reserve);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryLoginForbiddenIPField *p)
{
  set_obj(obj, "IPAddress", &p->IPAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryIPListField *p)
{
  set_obj(obj, "IPAddress", &p->IPAddress);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryUserRightsAssignField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
}
static void set_obj(Local<Object>& obj, CThostFtdcReserveOpenAccountConfirmField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "Gender", &p->Gender);
  set_obj(obj, "CountryCode", &p->CountryCode);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "Address", &p->Address);
  set_obj(obj, "ZipCode", &p->ZipCode);
  set_obj(obj, "Telephone", &p->Telephone);
  set_obj(obj, "MobilePhone", &p->MobilePhone);
  set_obj(obj, "Fax", &p->Fax);
  set_obj(obj, "EMail", &p->EMail);
  set_obj(obj, "MoneyAccountStatus", &p->MoneyAccountStatus);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankPassWord", &p->BankPassWord);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "VerifyCertNoFlag", &p->VerifyCertNoFlag);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "BankAccType", &p->BankAccType);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "BankReserveOpenSeq", &p->BankReserveOpenSeq);
  set_obj(obj, "BookDate", &p->BookDate);
  set_obj(obj, "BookPsw", &p->BookPsw);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
}
static void set_obj(Local<Object>& obj, CThostFtdcReserveOpenAccountField *p)
{
  set_obj(obj, "TradeCode", &p->TradeCode);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankBranchID", &p->BankBranchID);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerBranchID", &p->BrokerBranchID);
  set_obj(obj, "TradeDate", &p->TradeDate);
  set_obj(obj, "TradeTime", &p->TradeTime);
  set_obj(obj, "BankSerial", &p->BankSerial);
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "PlateSerial", &p->PlateSerial);
  set_obj(obj, "LastFragment", &p->LastFragment);
  set_obj(obj, "SessionID", &p->SessionID);
  set_obj(obj, "CustomerName", &p->CustomerName);
  set_obj(obj, "IdCardType", &p->IdCardType);
  set_obj(obj, "IdentifiedCardNo", &p->IdentifiedCardNo);
  set_obj(obj, "Gender", &p->Gender);
  set_obj(obj, "CountryCode", &p->CountryCode);
  set_obj(obj, "CustType", &p->CustType);
  set_obj(obj, "Address", &p->Address);
  set_obj(obj, "ZipCode", &p->ZipCode);
  set_obj(obj, "Telephone", &p->Telephone);
  set_obj(obj, "MobilePhone", &p->MobilePhone);
  set_obj(obj, "Fax", &p->Fax);
  set_obj(obj, "EMail", &p->EMail);
  set_obj(obj, "MoneyAccountStatus", &p->MoneyAccountStatus);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "BankPassWord", &p->BankPassWord);
  set_obj(obj, "InstallID", &p->InstallID);
  set_obj(obj, "VerifyCertNoFlag", &p->VerifyCertNoFlag);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
  set_obj(obj, "Digest", &p->Digest);
  set_obj(obj, "BankAccType", &p->BankAccType);
  set_obj(obj, "BrokerIDByBank", &p->BrokerIDByBank);
  set_obj(obj, "TID", &p->TID);
  set_obj(obj, "ReserveOpenAccStas", &p->ReserveOpenAccStas);
  set_obj(obj, "ErrorID", &p->ErrorID);
  set_obj(obj, "ErrorMsg", &p->ErrorMsg);
}
static void set_obj(Local<Object>& obj, CThostFtdcAccountPropertyField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "AccountID", &p->AccountID);
  set_obj(obj, "BankID", &p->BankID);
  set_obj(obj, "BankAccount", &p->BankAccount);
  set_obj(obj, "OpenName", &p->OpenName);
  set_obj(obj, "OpenBank", &p->OpenBank);
  set_obj(obj, "IsActive", &p->IsActive);
  set_obj(obj, "AccountSourceType", &p->AccountSourceType);
  set_obj(obj, "OpenDate", &p->OpenDate);
  set_obj(obj, "CancelDate", &p->CancelDate);
  set_obj(obj, "OperatorID", &p->OperatorID);
  set_obj(obj, "OperateDate", &p->OperateDate);
  set_obj(obj, "OperateTime", &p->OperateTime);
  set_obj(obj, "CurrencyID", &p->CurrencyID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQryCurrDRIdentityField *p)
{
  set_obj(obj, "DRIdentityID", &p->DRIdentityID);
}
static void set_obj(Local<Object>& obj, CThostFtdcCurrDRIdentityField *p)
{
  set_obj(obj, "DRIdentityID", &p->DRIdentityID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQrySecAgentCheckModeField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "InvestorID", &p->InvestorID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQrySecAgentTradeInfoField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "BrokerSecAgentID", &p->BrokerSecAgentID);
}
static void set_obj(Local<Object>& obj, CThostFtdcUserSystemInfoField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "ClientSystemInfoLen", &p->ClientSystemInfoLen);
  set_obj(obj, "ClientSystemInfo", &p->ClientSystemInfo);
  set_obj(obj, "ClientPublicIP", &p->ClientPublicIP);
  set_obj(obj, "ClientIPPort", &p->ClientIPPort);
  set_obj(obj, "ClientLoginTime", &p->ClientLoginTime);
  set_obj(obj, "ClientAppID", &p->ClientAppID);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqUserAuthMethodField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
}
static void set_obj(Local<Object>& obj, CThostFtdcRspUserAuthMethodField *p)
{
  set_obj(obj, "UsableAuthMethod", &p->UsableAuthMethod);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqGenUserCaptchaField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
}
static void set_obj(Local<Object>& obj, CThostFtdcRspGenUserCaptchaField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "CaptchaInfoLen", &p->CaptchaInfoLen);
  set_obj(obj, "CaptchaInfo", &p->CaptchaInfo);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqGenUserTextField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
}
static void set_obj(Local<Object>& obj, CThostFtdcRspGenUserTextField *p)
{
  set_obj(obj, "UserTextSeq", &p->UserTextSeq);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqUserLoginWithCaptchaField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "UserProductInfo", &p->UserProductInfo);
  set_obj(obj, "InterfaceProductInfo", &p->InterfaceProductInfo);
  set_obj(obj, "ProtocolInfo", &p->ProtocolInfo);
  set_obj(obj, "MacAddress", &p->MacAddress);
  set_obj(obj, "ClientIPAddress", &p->ClientIPAddress);
  set_obj(obj, "LoginRemark", &p->LoginRemark);
  set_obj(obj, "Captcha", &p->Captcha);
  set_obj(obj, "ClientIPPort", &p->ClientIPPort);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqUserLoginWithTextField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "UserProductInfo", &p->UserProductInfo);
  set_obj(obj, "InterfaceProductInfo", &p->InterfaceProductInfo);
  set_obj(obj, "ProtocolInfo", &p->ProtocolInfo);
  set_obj(obj, "MacAddress", &p->MacAddress);
  set_obj(obj, "ClientIPAddress", &p->ClientIPAddress);
  set_obj(obj, "LoginRemark", &p->LoginRemark);
  set_obj(obj, "Text", &p->Text);
  set_obj(obj, "ClientIPPort", &p->ClientIPPort);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqUserLoginWithOTPField *p)
{
  set_obj(obj, "TradingDay", &p->TradingDay);
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "Password", &p->Password);
  set_obj(obj, "UserProductInfo", &p->UserProductInfo);
  set_obj(obj, "InterfaceProductInfo", &p->InterfaceProductInfo);
  set_obj(obj, "ProtocolInfo", &p->ProtocolInfo);
  set_obj(obj, "MacAddress", &p->MacAddress);
  set_obj(obj, "ClientIPAddress", &p->ClientIPAddress);
  set_obj(obj, "LoginRemark", &p->LoginRemark);
  set_obj(obj, "OTPPassword", &p->OTPPassword);
  set_obj(obj, "ClientIPPort", &p->ClientIPPort);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqApiHandshakeField *p)
{
  set_obj(obj, "CryptoKeyVersion", &p->CryptoKeyVersion);
}
static void set_obj(Local<Object>& obj, CThostFtdcRspApiHandshakeField *p)
{
  set_obj(obj, "FrontHandshakeDataLen", &p->FrontHandshakeDataLen);
  set_obj(obj, "FrontHandshakeData", &p->FrontHandshakeData);
  set_obj(obj, "IsApiAuthEnabled", &p->IsApiAuthEnabled);
}
static void set_obj(Local<Object>& obj, CThostFtdcReqVerifyApiKeyField *p)
{
  set_obj(obj, "ApiHandshakeDataLen", &p->ApiHandshakeDataLen);
  set_obj(obj, "ApiHandshakeData", &p->ApiHandshakeData);
}
static void set_obj(Local<Object>& obj, CThostFtdcDepartmentUserField *p)
{
  set_obj(obj, "BrokerID", &p->BrokerID);
  set_obj(obj, "UserID", &p->UserID);
  set_obj(obj, "InvestorRange", &p->InvestorRange);
  set_obj(obj, "InvestorID", &p->InvestorID);
}
static void set_obj(Local<Object>& obj, CThostFtdcQueryFreqField *p)
{
  set_obj(obj, "QueryFreq", &p->QueryFreq);
}

};
#endif



