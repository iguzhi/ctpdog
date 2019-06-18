/////////////////////////////////////////////////////////////////////////
///@system ctp trader addon
///@company 慧网基金
///@file WrapTd.cpp
///@brief 定义js接口
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////

#include "WrapTd.h"

namespace td
{
using v8::HandleScope;
using v8::Exception;
using v8::Null;
using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;
using v8::Int32;
using v8::Boolean;
using v8::Handle;


set<string>         WrapTd::m_event;                //可以注册的回调事件
Persistent<Function> WrapTd::constructor;           //主动请求函数映射js name

WrapTd::WrapTd()
{

}

WrapTd::~WrapTd()
{

}
void WrapTd::Init(v8::Isolate* isolate)
{
	//主动请求函数的映射
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "WrapTd"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    // AUTOCODE
    {{wrapTd_setJsMethods}}
    NODE_SET_PROTOTYPE_METHOD(tpl, "On", On);
    NODE_SET_PROTOTYPE_METHOD(tpl, "Dispose", Dispose);
    constructor.Reset(isolate, tpl->GetFunction());

    //注册回调函数的映射更新
    // AUTOCODE
    {{wrapTd_addRspEvent}}
}

void WrapTd::New(const FunctionCallbackInfo<Value>& args)
{
    Isolate* isolate = args.GetIsolate();
    if(args.IsConstructCall())
    {
        // Invoked as constructor: `new MyObject(...)`
        WrapTd* obj = new WrapTd();
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    }
    else
    {
        // Invoked as plain function `MyObject(...)`, turn into construct call
        Local<Function> cons = Local<Function>::New(isolate, constructor);
        Local<Context> context = isolate->GetCurrentContext();
        Local<Object> instance = cons->NewInstance(context, 0, NULL).ToLocalChecked();
        args.GetReturnValue().Set(instance);
    }
}

void WrapTd::NewInstance(const FunctionCallbackInfo<Value>& args)
{
    Isolate* isolate = args.GetIsolate();
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> instance  = cons->NewInstance(context, 0, NULL).ToLocalChecked();
    args.GetReturnValue().Set(instance);
}

void WrapTd::On(const FunctionCallbackInfo<Value>& args)
{
    Isolate* isolate = args.GetIsolate();
    WrapTd* obj = node::ObjectWrap::Unwrap<WrapTd>(args.Holder());

    if (args[0]->IsUndefined() || args[1]->IsUndefined()) 
    {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments->event name or function")));
        return;
    }

    //WrapTd* obj = node::ObjectWrap::Unwrap<WrapTd>(args.Holder());

    Local<String> eventName = args[0]->ToString();
    Local<Function> cb = Local<Function>::Cast(args[1]);

    String::Utf8Value eNameUtf8(eventName);
    std::set<string>::iterator eit = m_event.find(*eNameUtf8);
    if (eit == m_event.end()) 
    {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "System has no register this event")));
        return;
    }

    if(obj->CanCallback(*eit))
    {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Callback is defined before")));
        return;
    }

    obj->SetCallback(*eNameUtf8, cb, isolate);

    args.GetReturnValue().Set(Number::New(isolate, 0));
}

void WrapTd::Dispose(const FunctionCallbackInfo<Value>& args)
{
    WrapTd* obj = node::ObjectWrap::Unwrap<WrapTd>(args.Holder());
    Isolate* isolate = args.GetIsolate();
    obj->dispose();
    args.GetReturnValue().Set(Undefined(isolate));
}


#define REQ_WITH_REQID(req) \
    WrapTd* obj = node::ObjectWrap::Unwrap<WrapTd>(args.Holder());\
    Isolate* isolate = args.GetIsolate();\
    if (args[0]->IsUndefined() || !args[0]->IsObject() || args[1]->IsUndefined())\
    {\
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));\
        return;\
    }\
    memset(&req, 0, sizeof(req));\
    Local<Object> objjs = args[0]->ToObject();\
    CSFunction::set_struct(objjs, &req);\
    int reqid = args[1]->Int32Value();
    
// AUTOCODE
{{wrapTd_reqMethodsImpl_def}}
		

////////////////////////////////ctp on 回调////////////////////////////////////////////////
#define CONTEXT() \
Isolate* isolate = Isolate::GetCurrent();\
HandleScope scope(isolate);\
string k = string(_FUNCTION_).substr(4);\
__callback_iter_type it = callback_map.find(k);\
if(it == callback_map.end()) return;


#define CONTEXT_WITH_1(rsp) \
Isolate* isolate = Isolate::GetCurrent();\
HandleScope scope(isolate);\
string k = string(_FUNCTION_).substr(4);\
__callback_iter_type it = callback_map.find(k);\
if(it == callback_map.end()) return;\
Local<Value> argv[1];\
if(rsp != NULL)\
{\
    Local<Object> obj = Object::New(isolate);\
    CSFunction::set_obj(obj, rsp);\
    argv[0] = obj;\
}\
else\
{\
    argv[0] = Local<Value>::New(isolate, Undefined(isolate));\
}\
Local<Function> cb = Local<Function>::New(isolate, it->second);\
cb->Call(Null(isolate), 1, argv);  

#define CONTEXT_WITH_2(rsp) \
Isolate* isolate = Isolate::GetCurrent();\
HandleScope scope(isolate);\
string k = string(_FUNCTION_).substr(4);\
__callback_iter_type it = callback_map.find(k);\
if(it == callback_map.end()) return;\
Local<Value> argv[2];\
if(rsp != NULL)\
{\
    Local<Object> obj = Object::New(isolate);\
    CSFunction::set_obj(obj, rsp);\
    argv[0] = obj;\
}\
else\
{\
    argv[0] = Local<Value>::New(isolate, Undefined(isolate));\
}\
argv[1] = PkgRspInfo(pRspInfo);\
Local<Function> cb = Local<Function>::New(isolate, it->second);\
cb->Call(Null(isolate), 2, argv);  


#define CONTEXT_WITH_4(rsp) do{\
Isolate* isolate = Isolate::GetCurrent();\
HandleScope scope(isolate);\
string k = string(_FUNCTION_).substr(4);\
__callback_iter_type it = callback_map.find(k);\
if(it == callback_map.end()) return;\
Local<Value> argv[4];\
if(rsp != NULL)\
{\
    Local<Object> obj = Object::New(isolate);\
    CSFunction::set_obj(obj, rsp);\
    argv[0] = obj;\
}\
else\
{\
    argv[0] = Local<Value>::New(isolate, Undefined(isolate));\
}\
argv[1] = PkgRspInfo(pRspInfo);\
argv[2] = Local<Value>::New(isolate, Int32::New(isolate, nRequestID));\
argv[3] = Boolean::New(isolate, bIsLast)->ToBoolean();\
Local<Function> cb = Local<Function>::New(isolate, it->second);\
cb->Call(Null(isolate), 4, argv);\
}while(0);

#define CONTEXT_WITH_LAST_3() \
Isolate* isolate = Isolate::GetCurrent();\
HandleScope scope(isolate);\
string k = string(_FUNCTION_).substr(4);\
__callback_iter_type it = callback_map.find(k);\
if(it == callback_map.end()) return;\
Local<Value> argv[3];\
argv[0] = PkgRspInfo(pRspInfo);\
argv[1] = Local<Value>::New(isolate, Int32::New(isolate, nRequestID));\
argv[2] = Boolean::New(isolate, bIsLast)->ToBoolean();\
Local<Function> cb = Local<Function>::New(isolate, it->second);\
cb->Call(Null(isolate), 3, argv);  

// AUTOCODE
{{wrapTd_mainRspMethodsImpl_def}}

}




