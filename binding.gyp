{
  "targets": [
    {
      "target_name": "shifctp",
      "sources": [ "src/shifctp.cpp",  "src/ThostFtdcMdSpiI.cpp", "src/WrapMd.cpp",  "src/ThostFtdcTraderSpiI.cpp", "src/WrapTd.cpp"],
      "conditions": [
        ["OS=='linux'", {
          "libraries":["$(CURDIR)/lib/tradeapi_linux64/thostmduserapi.so", "$(CURDIR)/lib/tradeapi_linux64/thosttraderapi.so"],
          "include_dirs":["tradeapi_linux64", "src"]
        }],
        ["OS=='win'", {
          "conditions": [
            ["target_arch=='ia32'", {
              "libraries":["lib/tradeapi_windows/thostmduserapi.lib", "lib/tradeapi_windows/thosttraderapi.lib"],
              "include_dirs":["tradeapi_windows", "src"]
            }, { # target_arch=="x64"
              "libraries":["lib/tradeapi_windows64/thostmduserapi.lib", "lib/tradeapi_windows64/thosttraderapi.lib"],
              "include_dirs":["tradeapi_windows64", "src"]
            }]
          ]
        }, { # OS != "win"
          "libraries":["$(CURDIR)/lib/tradeapi_linux64/thostmduserapi.so", "$(CURDIR)/lib/tradeapi_linux64/thosttraderapi.so"],
          "include_dirs":["tradeapi_linux64", "src"]
        }]
      ]
    }
  ]
}