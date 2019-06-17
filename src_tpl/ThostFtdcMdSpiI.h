/////////////////////////////////////////////////////////////////////////
///@system ctp行情nodejs addon
///@company 慧网基金
///@file ThostFtdcMdApiSpi.h
///@brief ctp线程及回调
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#ifndef __THOSTFTDCMDSPI_H__
#define __THOSTFTDCMDSPI_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//#include <pthread.h>
//#include <semaphore.h>
#include <string.h>
#include <stdint.h>
#include <set>
#include <map>
#include <string>
#include <uv.h>

#include "comm.h"

// CTP
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcMdApi.h"

using namespace std;

namespace md
{

class CThostFtdcMdSpiI;

//以api来划分结构体
struct taskdata
{
    taskdata(CThostFtdcMdSpiI* p){handle.data = this; pmd = p;}
    CThostFtdcMdSpiI* pmd;
    uv_async_t handle;
    string api;             //表示是那个api回调
    
    void reinit()
    {
        api = "";
        memset(&data, 0, sizeof(data));
        nRequestID = 0;
        bIsLast = 0;
        memset(&RspInfo, 0, sizeof(RspInfo));
    }

    union _data 
    {
        int nReason;
        int nTimeLapse;
        // AUTOCODE: Struct
        {{mdSpi_struct}}
    }data;
    int nRequestID;
    bool bIsLast;
    CThostFtdcRspInfoField RspInfo; //公用返回
};

class CThostFtdcMdSpiI : public CThostFtdcMdSpi 
{

    public:
        CThostFtdcMdSpiI();
        ~CThostFtdcMdSpiI();

        void uv_async_send_s(uv_async_t* handle)
        {
            //printf("uv_async_send api=%s\n", ((taskdata*)handle->data)->api.c_str());
            //uv_async_init(uv_default_loop(), handle, on_async_cb);
            uv_async_send(handle);
        }

        //主线程回调js处理
        // AUTOCODE

        {{mdSpi_mainRspMethodsInterface_def}}
        
        {{mdSpi_rspMethodsInterface_def}}
        
        CThostFtdcMdApi* GetMdApi(){return m_pApi;}
        void setMdApi(CThostFtdcMdApi*  _m_pApi){ m_pApi = _m_pApi;}
        void RegisterSpi(){ m_pApi->RegisterSpi(this);}
        void dispose() {
          m_pApi->RegisterSpi(NULL);
          m_pApi->Release();
          m_pApi = NULL;
        }
       
    private:
        static void on_uv_close_cb(uv_handle_t* handle); 
        static void on_async_cb(uv_async_t* handle);

        taskdata* get_task()
        {
            if(task_position >= task_size) task_position = 0;
            return ptask[task_position++];
        }

        taskdata** ptask; 
        uint32_t task_size;
        uint32_t task_position;

        CThostFtdcMdApi* 	m_pApi; 		        //交易请求结构体
};
}

#endif






