/////////////////////////////////////////////////////////////////////////
///@system ctp行情nodejs addon
///@company 慧网基金
///@file ThostFtdcMdApiSpiI.cpp
///@brief 定义了ctp线程及回调
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////

#include "ThostFtdcMdSpiI.h"

namespace md
{


CThostFtdcMdSpiI::CThostFtdcMdSpiI()
{
    task_size = 10000;
    task_position = 0;
    ptask = new taskdata* [task_size];
    for(uint32_t i=0; i<task_size; i++)
    {
        ptask[i] = new taskdata(this);
        uv_async_init(uv_default_loop(), &ptask[i]->handle, on_async_cb);
    }

    //m_pApi = CThostFtdcMdApi::CreateFtdcMdApi();
    //m_pApi->RegisterSpi(this);         
}

CThostFtdcMdSpiI::~CThostFtdcMdSpiI()
{
    for(uint32_t i=0; i<task_size; i++)
    {
        uv_close((uv_handle_t*) & ptask[i]->handle, on_uv_close_cb);
    }
    delete ptask;
    
    m_pApi->RegisterSpi(NULL);
    m_pApi->Release();
    m_pApi = NULL;
}

void CThostFtdcMdSpiI::on_uv_close_cb(uv_handle_t* handle) 
{
    delete (taskdata*)(((uv_async_t*)handle)->data);
}

void CThostFtdcMdSpiI::on_async_cb(uv_async_t* handle)
{
    taskdata* task = (taskdata*)handle->data;
    //printf("on_async_cb api=%s\n", task->api.c_str());
    do{
    // AUTOCODE: 调用响应函数
  if(task->api == "OnRtnDepthMarketData") { task->pmd->MainOnRtnDepthMarketData(&task->data.DepthMarketData); continue;}
  if(task->api == "OnFrontConnected") { task->pmd->MainOnFrontConnected(); continue;}
  if(task->api == "OnFrontDisconnected") { task->pmd->MainOnFrontDisconnected(task->nReason); continue;}
  if(task->api == "OnHeartBeatWarning") { task->pmd->MainOnHeartBeatWarning(task->nTimeLapse); continue;}
  if(task->api == "OnRspUserLogin") { task->pmd->MainOnRspUserLogin(&task->data.RspUserLogin, &task->RspInfo, task->nRequestID, task->bIsLast); continue;}
  if(task->api == "OnRspUserLogout") { task->pmd->MainOnRspUserLogout(&task->data.UserLogout, &task->RspInfo, task->nRequestID, task->bIsLast); continue;}
  if(task->api == "OnRspError") { task->pmd->MainOnRspError(&task->RspInfo, task->nRequestID, task->bIsLast); continue;}
  if(task->api == "OnRspSubMarketData") { task->pmd->MainOnRspSubMarketData(&task->data.SpecificInstrument, &task->RspInfo, task->nRequestID, task->bIsLast); continue;}
  if(task->api == "OnRspUnSubMarketData") { task->pmd->MainOnRspUnSubMarketData(&task->data.SpecificInstrument, &task->RspInfo, task->nRequestID, task->bIsLast); continue;}
  if(task->api == "OnRspSubForQuoteRsp") { task->pmd->MainOnRspSubForQuoteRsp(&task->data.SpecificInstrument, &task->RspInfo, task->nRequestID, task->bIsLast); continue;}
  if(task->api == "OnRspUnSubForQuoteRsp") { task->pmd->MainOnRspUnSubForQuoteRsp(&task->data.SpecificInstrument, &task->RspInfo, task->nRequestID, task->bIsLast); continue;}
  if(task->api == "OnRtnForQuoteRsp") { task->pmd->MainOnRtnForQuoteRsp(&task->data.ForQuoteRsp); continue;}

        printf("ERROR:%s _on_completed\n", task->api.c_str());
    }while(0);
    //uv_close((uv_handle_t*)handle, on_uv_close_cb);
    task->reinit();
}

#define GET_TASK(func)  \
    taskdata* t = get_task();\
    if(t->api != "") {printf("%s process fail. task queue is full.\n", func);return;}\
    t->api = func;\

/////////////////////////////on回调函数///////////////////////////////////////////////////////////
// AUTOCODE: 实现响应函数
void CThostFtdcMdSpiI::OnFrontConnected() 
{
  GET_TASK(_FUNCTION_);

  uv_async_send_s(&t->handle);
}
void CThostFtdcMdSpiI::OnFrontDisconnected(int nReason) 
{
  GET_TASK(_FUNCTION_);
  t->nReason = nReason;
  uv_async_send_s(&t->handle);
}
void CThostFtdcMdSpiI::OnHeartBeatWarning(int nTimeLapse) 
{
  GET_TASK(_FUNCTION_);
  t->nTimeLapse = nTimeLapse;
  uv_async_send_s(&t->handle);
}
void CThostFtdcMdSpiI::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
  GET_TASK(_FUNCTION_);
  t->data.RspUserLogin = *pRspUserLogin;
  t->RspInfo = *pRspInfo;
  t->nRequestID = nRequestID;
  t->bIsLast = bIsLast;
  uv_async_send_s(&t->handle);
}
void CThostFtdcMdSpiI::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
  GET_TASK(_FUNCTION_);
  t->data.UserLogout = *pUserLogout;
  t->RspInfo = *pRspInfo;
  t->nRequestID = nRequestID;
  t->bIsLast = bIsLast;
  uv_async_send_s(&t->handle);
}
void CThostFtdcMdSpiI::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
  GET_TASK(_FUNCTION_);
  t->RspInfo = *pRspInfo;
  t->nRequestID = nRequestID;
  t->bIsLast = bIsLast;
  uv_async_send_s(&t->handle);
}
void CThostFtdcMdSpiI::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
  GET_TASK(_FUNCTION_);
  t->data.SpecificInstrument = *pSpecificInstrument;
  t->RspInfo = *pRspInfo;
  t->nRequestID = nRequestID;
  t->bIsLast = bIsLast;
  uv_async_send_s(&t->handle);
}
void CThostFtdcMdSpiI::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
  GET_TASK(_FUNCTION_);
  t->data.SpecificInstrument = *pSpecificInstrument;
  t->RspInfo = *pRspInfo;
  t->nRequestID = nRequestID;
  t->bIsLast = bIsLast;
  uv_async_send_s(&t->handle);
}
void CThostFtdcMdSpiI::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
  GET_TASK(_FUNCTION_);
  t->data.SpecificInstrument = *pSpecificInstrument;
  t->RspInfo = *pRspInfo;
  t->nRequestID = nRequestID;
  t->bIsLast = bIsLast;
  uv_async_send_s(&t->handle);
}
void CThostFtdcMdSpiI::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
  GET_TASK(_FUNCTION_);
  t->data.SpecificInstrument = *pSpecificInstrument;
  t->RspInfo = *pRspInfo;
  t->nRequestID = nRequestID;
  t->bIsLast = bIsLast;
  uv_async_send_s(&t->handle);
}
void CThostFtdcMdSpiI::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) 
{
  GET_TASK(_FUNCTION_);
  t->data.DepthMarketData = *pDepthMarketData;
  uv_async_send_s(&t->handle);
}
void CThostFtdcMdSpiI::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) 
{
  GET_TASK(_FUNCTION_);
  t->data.ForQuoteRsp = *pForQuoteRsp;
  uv_async_send_s(&t->handle);
}

}


