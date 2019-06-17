/////////////////////////////////////////////////////////////////////////
///@system node ctp addon
///@company 慧网基金
///@file ThostFtdcTraderSpiI.h
///@brief ctp 接口
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////
#ifndef __THOSTFTDCTRADERSPI_H__
#define __THOSTFTDCTRADERSPI_H__

//#include <sys/socket.h>
//#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <math.h>
//#include <pthread.h>
#include <map>
#include <uv.h>

#include "comm.h"

#include "ThostFtdcUserApiDataType.h"                       
#include "ThostFtdcUserApiStruct.h"                         
#include "ThostFtdcTraderApi.h"                                 

using namespace std;
		
namespace td
{
class CThostFtdcTraderSpiI;

//以api来划分结构体
struct taskdata
{
    taskdata(CThostFtdcTraderSpiI* p){handle.data = this; ptd = p;}
    CThostFtdcTraderSpiI* ptd;
    string api;             //表示是那个api回调
    uv_async_t handle;
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
       // AUTOCODE: struct
       #11
    }data;
    int nRequestID;
    bool bIsLast;
    CThostFtdcRspInfoField RspInfo; //公用返回
};

//一个实例对用一个用户
class CThostFtdcTraderSpiI : public CThostFtdcTraderSpi
{
	public:
		CThostFtdcTraderSpiI();
		virtual ~CThostFtdcTraderSpiI();
		CThostFtdcTraderApi* GetTdApi(){ return m_pApi;}
    void setTdApi(CThostFtdcTraderApi* _m_pApi){ m_pApi = _m_pApi;}
    void RegisterSpi(){ m_pApi->RegisterSpi(this);}
    void dispose() {
      m_pApi->RegisterSpi(NULL);
      m_pApi->Release();
      m_pApi = NULL;
    }
    
        void uv_async_send_s(uv_async_t* handle)
        {
            //uv_async_init(uv_default_loop(), handle, on_async_cb);
            uv_async_send(handle);
        }

        inline void  QUEUEPUSH(const char* api, void* p=NULL, int len=0, CThostFtdcRspInfoField* pRspInfo=NULL, int nRequestID=0, int bIsLast=0)
        {
            taskdata* t = get_task();
            if(t->api != "") {printf("%s process fail. task queue is full.\n", api);return;}
            t->api = api;

            //taskdata* t = new taskdata(this);
            if(len > 0 && p!=NULL) 
            {
                memcpy((char*)&t->data, p, len); 
            }
            else
            {
                memset((char*)&t->data, 0, len);
            }
            memset(&t->RspInfo, 0, sizeof(t->RspInfo));
            if(pRspInfo != NULL)t->RspInfo = *pRspInfo;    
            t->nRequestID = nRequestID;
            t->bIsLast = bIsLast;
            //uv_queue_work(uv_default_loop(), &t->work, _on_async_queue, _on_completed);
            uv_async_send_s(&t->handle);
        }

	    // AUTOCODE
        #11
        
        #11
        
    private:
 
        taskdata* get_task()
        {
            if(task_position >= task_size) task_position = 0;
            return ptask[task_position++];
        }

        taskdata** ptask; 
        uint32_t task_size;
        uint32_t task_position;

        static void on_uv_close_cb(uv_handle_t* handle); 
        static void on_async_cb(uv_async_t* handle);

        CThostFtdcTraderApi*    		m_pApi;        //交易请求结构体
};

}


#endif



