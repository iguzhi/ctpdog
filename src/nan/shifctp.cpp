#include <signal.h>
// #include <node.h>
#include <napi.h>
#include "WrapMd.h"
#include "WrapTd.h"



namespace ctp
{
  using namespace Napi;
// using v8::FunctionCallbackInfo;
// using v8::Isolate;
// using v8::Local;
// using v8::Object;
// using v8::Value;
// using v8::Value;

void signal_segv(int sig)
{
    printf("catch SIGSEGV\n");
}

void CreateMd(const CallbackInfo& args)
{
    md::WrapMd::NewInstance(args);
}

void CreateTd(const CallbackInfo& args)
{
    td::WrapTd::NewInstance(args);
}


void Init(Env env, Object exports)
{
    //signal(SIGSEGV, signal_segv);
    md::WrapMd::Init(env);
    td::WrapTd::Init(env);
    exports.Set(String::New(env, "createMd"), Function::New(env, CreateMd));
    exports.Set(String::New(env, "createTd"), Function::New(env, CreateTd));
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)

