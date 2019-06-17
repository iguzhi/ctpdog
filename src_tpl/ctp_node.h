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

static void set_struct(Local<Object>& obj, const char* key, float* dest, int len=0)
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

{{ctpNode_setterMethods_def}}

};
#endif



