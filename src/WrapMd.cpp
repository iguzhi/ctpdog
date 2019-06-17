/////////////////////////////////////////////////////////////////////////
///@system ctp 行情nodejs addon
///@company 慧网基金
///@file WrapMd.cpp
///@brief js回调接口
///@history 
///20160326	dreamyzhang		创建该文件
/////////////////////////////////////////////////////////////////////////

#include "WrapMd.h"

namespace md
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


set<string>         WrapMd::m_event;                //可以注册的回调事件
Persistent<Function> WrapMd::constructor;           //主动请求函数映射js name


WrapMd::WrapMd()
{

}

WrapMd::~WrapMd()
{

}


void WrapMd::Init(Isolate* isolate) 
{
    //主动请求函数的映射
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "WrapMd"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    // AUTOCODE
    #22
    NODE_SET_PROTOTYPE_METHOD(tpl, "On", On);
    constructor.Reset(isolate, tpl->GetFunction());

    //注册回调函数的映射更新
    // AUTOCODE
    #22
}

void WrapMd::New(const FunctionCallbackInfo<Value>& args)
{
    Isolate* isolate = args.GetIsolate();
    if(args.IsConstructCall())
    {
        // Invoked as constructor: `new MyObject(...)`
        WrapMd* obj = new WrapMd();
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

void WrapMd::NewInstance(const FunctionCallbackInfo<Value>& args)
{
    Isolate* isolate = args.GetIsolate();
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> instance  = cons->NewInstance(context, 0, NULL).ToLocalChecked();
    args.GetReturnValue().Set(instance);
}

void WrapMd::On(const FunctionCallbackInfo<Value>& args)
{
    Isolate* isolate = args.GetIsolate();
    WrapMd* obj = node::ObjectWrap::Unwrap<WrapMd>(args.Holder());

    if (args[0]->IsUndefined() || args[1]->IsUndefined()) 
    {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments->event name or function")));
        return;
    }

    //WrapMd* obj = node::ObjectWrap::Unwrap<WrapMd>(args.Holder());

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

#define REQ_WITH_REQID(req) \
    WrapMd* obj = node::ObjectWrap::Unwrap<WrapMd>(args.Holder());\
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
#22


/////////////////////////////on回调函数///////////////////////////////////////////////////////////
// AUTOCODE
#22

}





