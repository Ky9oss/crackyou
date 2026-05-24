-- #############################################################################
-- Interpreter: luajit
-- Dependencies: luvit (https://github.com/luvit/luvit)
-- Description: Batch Check in VirusTotal
-- Author: Ky9oss
-- #############################################################################

local https = require('https')

local options = {
  host = "luvit.io",
  port = 443,
  path = "/"
}

local req = https.request(options, function (res)
  res:on('data', function (chunk)
    p("ondata", {chunk=chunk})
  end)
end)
req:done()
