{
  "targets": [
    {
      "target_name": "shifctp",
      "sources": [ "src/shifctp.cpp",  "src/ThostFtdcMdSpiI.cpp", "src/WrapMd.cpp",  "src/ThostFtdcTraderSpiI.cpp", "src/WrapTd.cpp"],
      "conditions": [
        ["OS=='linux'", {
          "libraries":["$(CURDIR)/api/tradeapi_linux64/thostmduserapi.so", "$(CURDIR)/api/tradeapi_linux64/thosttraderapi.so"],
          "include_dirs":["tradeapi_linux64", "src"]
        }],
        ["OS=='win'", {
          "conditions": [
            ["target_arch=='ia32'", {
              "libraries":["api/tradeapi_windows/thostmduserapi.lib", "api/tradeapi_windows/thosttraderapi.lib"],
              "include_dirs":["tradeapi_windows", "src"]
            }, { # target_arch=="x64"
              "libraries":["api/tradeapi_windows64/thostmduserapi.lib", "api/tradeapi_windows64/thosttraderapi.lib"],
              "include_dirs":["tradeapi_windows64", "src"]
            }]
          ]
        }, { # OS != "win"
          "libraries":["$(CURDIR)/api/tradeapi_linux64/thostmduserapi.so", "$(CURDIR)/api/tradeapi_linux64/thosttraderapi.so"],
          "include_dirs":["tradeapi_linux64", "src"]
        }]
      ]
    }
  ]
}