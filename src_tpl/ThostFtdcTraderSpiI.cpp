/////////////////////////////////////////////////////////////////////////
///@system node ctp addon
///@company 慧网基金
///@file ThostFtdcTraderSpiI.cpp
///@brief ctp接口
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////

#include "ThostFtdcTraderSpiI.h"

namespace td
{

CThostFtdcTraderSpiI::CThostFtdcTraderSpiI()
{
    task_size = 10000;
    task_position = 0;
    ptask = new taskdata* [task_size];
    for(uint32_t i=0; i<task_size; i++)
    {
        ptask[i] = new taskdata(this);
        uv_async_init(uv_default_loop(), &ptask[i]->handle, on_async_cb);
    }
    
    //m_pApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
    //m_pApi->RegisterSpi(this);
}

CThostFtdcTraderSpiI::~CThostFtdcTraderSpiI()
{
    for(uint32_t i=0; i<task_size; i++)
    {
        uv_close((uv_handle_t*) & ptask[i]->handle, on_uv_close_cb);
    }
    delete ptask;
    
    m_pApi->Release(); 
    m_pApi = NULL;
}

void CThostFtdcTraderSpiI::on_uv_close_cb(uv_handle_t* handle) 
{
    delete (taskdata*)(((uv_async_t*)handle)->data);
}

void CThostFtdcTraderSpiI::on_async_cb(uv_async_t* handle)
{
    //调用js中的回调函数 在初始化的时候注册的
    taskdata* task = (taskdata*)handle->data;
    //printf("on_async_cb api=%s\n", task->api.c_str());
    do{
        // AUTOCODE: 调用响应函数
        {{traderSpi_rspMethods_call}}
    }while(0); 
    task->reinit();
    //uv_close((uv_handle_t*)handle, on_uv_close_cb);
}

// AUTOCODE: 实现响应函数
{{traderSpi_rspMethodsImpl_def}}

}



