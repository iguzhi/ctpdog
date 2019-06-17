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
        {{mdSpi_rspMethods_call}}
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
{{mdSpi_rspMethodsImpl_def}}

}


