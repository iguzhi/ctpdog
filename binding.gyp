{
  "targets": [
    {
      "target_name": "shifctp",
      "sources": [ "src/shifctp.cpp",  "src/ThostFtdcMdSpiI.cpp", "src/WrapMd.cpp",  "src/ThostFtdcTraderSpiI.cpp", "src/WrapTd.cpp"],
      "conditions": [
        ["OS=='linux'", {
          "libraries":["$(CURDIR)/../api/tradeapi_linux64/thostmduserapi.so", "$(CURDIR)/../api/tradeapi_linux64/thosttraderapi.so"],
          "include_dirs":["api/tradeapi_linux64"]
        }],
        ["OS=='win'", {
          "conditions": [
            ["target_arch=='ia32'", {
              "libraries":["../api/tradeapi_windows/thostmduserapi.lib", "../api/tradeapi_windows/thosttraderapi.lib"],
              "include_dirs":["api/tradeapi_windows"]
            }, { # target_arch=="x64"
              "libraries":["../api/tradeapi_windows64/thostmduserapi.lib", "../api/tradeapi_windows64/thosttraderapi.lib"],
              "include_dirs":["api/tradeapi_windows64"]
            }]
          ]
        }]
      ]
    }
  ]
}