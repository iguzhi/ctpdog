{
  "targets": [
    {
      "target_name": "shifctp",
      "sources": [ "src/shifctp.cpp",  "src/ThostFtdcMdSpiI.cpp", "src/WrapMd.cpp",  "src/ThostFtdcTraderSpiI.cpp", "src/WrapTd.cpp"],
      "conditions": [
        ["OS=='linux'", {
          "libraries":["$(CURDIR)/../v6.3.11_20180109_api_tradeapi_linux64/thostmduserapi.so", "$(CURDIR)/../v6.3.11_20180109_api_tradeapi_linux64/thosttraderapi.so"],
          "include_dirs":["v6.3.11_20180109_api_tradeapi_linux64", "src"]
        }],
        ["OS=='win'", {
          "conditions": [
            ["target_arch=='ia32'", {
              "libraries":["../20180109_tradeapi_windows/thostmduserapi.lib", "../20180109_tradeapi_windows/thosttraderapi.lib"],
              "include_dirs":["20180109_tradeapi_windows", "src"]
            }, { # target_arch=="x64"
              "libraries":["../20180109_tradeapi64_windows/thostmduserapi.lib", "../20180109_tradeapi64_windows/thosttraderapi.lib"],
              "include_dirs":["20180109_tradeapi64_windows", "src"]
            }]
          ]
        }, { # OS != "win"
          "libraries":["$(CURDIR)/../v6.3.11_20180109_api_tradeapi_linux64/thostmduserapi.so", "$(CURDIR)/../v6.3.11_20180109_api_tradeapi_linux64/thosttraderapi.so"],
          "include_dirs":["v6.3.11_20180109_api_tradeapi_linux64", "src"]
        }]
      ]
    }
  ]
}