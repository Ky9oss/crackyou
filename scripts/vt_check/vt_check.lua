-- #############################################################################
-- Run: nvim --headless -l vt_check.lua
-- Dependencies: 
--   luvit (https://github.com/luvit/luvit)
-- Description: Batch Check in VirusTotal
-- Author: Ky9oss
-- #############################################################################

local https = require('https')

local url = https://www.virustotal.com/api/v3/files/{id}
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

function get_file_hash()
end

function get_unique_files()
end

