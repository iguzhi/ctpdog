# warning
目前仅支持linux64和win64平台(win32平台未测试，理论上应该也支持)

# compile
windows平台需要安装编译工具：npm install --global --production windows-build-tools
```
node-gyp rebuild

```

# execute
run example
```
node ./example/ctpapp.js

```

# Example
请参看[example](https://github.com/iamweilee/nodectp-example.git)

# version
穿透式


### 新增接口
///查询用户当前支持的认证模式的回复
virtual void OnRspUserAuthMethod(CThostFtdcRspUserAuthMethodField *pRspUserAuthMethod, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///获取图形验证码请求的回复
virtual void OnRspGenUserCaptcha(CThostFtdcRspGenUserCaptchaField *pRspGenUserCaptcha, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///获取短信验证码请求的回复
virtual void OnRspGenUserText(CThostFtdcRspGenUserTextField *pRspGenUserText, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///请求查询二级代理商信息响应
virtual void OnRspQrySecAgentTradeInfo(CThostFtdcSecAgentTradeInfoField *pSecAgentTradeInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

///注册用户终端信息，用于中继服务器多连接模式
///需要在终端认证成功后，用户登录前调用该接口
virtual int RegisterUserSystemInfo(CThostFtdcUserSystemInfoField *pUserSystemInfo) = 0;

///上报用户终端信息，用于中继服务器操作员登录模式
///操作员登录后，可以多次调用该接口上报客户信息
virtual int SubmitUserSystemInfo(CThostFtdcUserSystemInfoField *pUserSystemInfo) = 0;

///查询用户当前支持的认证模式
virtual int ReqUserAuthMethod(CThostFtdcReqUserAuthMethodField *pReqUserAuthMethod, int nRequestID) = 0;

///用户发出获取图形验证码请求
virtual int ReqGenUserCaptcha(CThostFtdcReqGenUserCaptchaField *pReqGenUserCaptcha, int nRequestID) = 0;

///用户发出获取短信验证码请求
virtual int ReqGenUserText(CThostFtdcReqGenUserTextField *pReqGenUserText, int nRequestID) = 0;

///用户发出带有图片验证码的登陆请求
virtual int ReqUserLoginWithCaptcha(CThostFtdcReqUserLoginWithCaptchaField *pReqUserLoginWithCaptcha, int nRequestID) = 0;

///用户发出带有短信验证码的登陆请求
virtual int ReqUserLoginWithText(CThostFtdcReqUserLoginWithTextField *pReqUserLoginWithText, int nRequestID) = 0;

///用户发出带有动态口令的登陆请求
virtual int ReqUserLoginWithOTP(CThostFtdcReqUserLoginWithOTPField *pReqUserLoginWithOTP, int nRequestID) = 0;

///请求查询二级代理商信息
virtual int ReqQrySecAgentTradeInfo(CThostFtdcQrySecAgentTradeInfoField *pQrySecAgentTradeInfo, int nRequestID) = 0;


### 删除接口
///登录请求2
virtual int ReqUserLogin2(CThostFtdcReqUserLoginField *pReqUserLogin, int nRequestID) = 0;

///用户口令更新请求2
virtual int ReqUserPasswordUpdate2(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID) = 0;

### 如何更新ctp api?
1. 从上期技术官网下载最新的库到api目录, 分别替换api目录三个子目录下的文件
2. 运行`yarn src`或`npm run src`, 该脚本会根据src_tpl下的文件模板自动生成src目录下的文件
2. 运行`yarn dict`或`npm run dict`, 该脚本会根据api下的ThostFtdcUserApiDataType.h文件生成字典js类
3. 运行`yarn rebuild`或`npm run rebuild`, 该脚本会重新编译生成ctp的node addon