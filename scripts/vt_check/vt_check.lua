-- #############################################################################
-- Run: luvit vt_check.lua
-- Dependencies:
--   luafilesystem (https://lunarmodules.github.io)
--   lua-http (https://github.com/daurnimator/lua-http/)
-- Description: Batch Check in VirusTotal
-- Author: Ky9oss
--
-- Maintained: NO
-- WARNING: This script is dropped beacuse https/tls1.3 in lua is sucks.
--
-- #############################################################################

local http = require("socket.http")
local lfs = require("lfs")

local handle = io.open("./apikey.txt", r)
local all_files = {}
local apikey

if handle then
  apikey = io.input(handle):read("*a")
else
  print("apikey.txt not found")
  exit(1)
end


function get_all_files(path, all_files)
	for file in lfs.dir(path) do
		if file ~= "." and file ~= ".." then
			local f = path .. "/" .. file
			local attr = lfs.attributes(f)
			assert(type(attr) == "table")
			if attr.mode == "directory" then
				get_all_files(f, all_files)
			elseif attr.mode == "file" then
        table.insert(all_files, f)
				print("\t " .. f)
			end
		end
	end
end

get_all_files("..", all_files)

function get_file_hash() end

function get_unique_files() end

-- function hash2report(hash)
-- 	local options = {
-- 		host = "www.virustotal.com",
-- 		port = 443,
-- 		path = "/api/v3/files/" .. hash,
-- 	}
--
-- 	local req = https.request(options, function(res)
-- 		res:on("data", function(chunk)
-- 			p("ondata", { chunk = chunk })
-- 		end)
-- 	end)
-- 	req:done()
-- end
--
function upload2report() end

local request = require "http.request"

local req = request.new_from_uri("https://www.baidu.com")

-- req.headers:upsert("user-agent", "lua-http/1.0")
req.headers:upsert("accept", "application/json")

local headers, stream, err = req:go()
if not headers then
    error("Request failed: " .. tostring(err))
end

local body = assert(stream:get_body_as_string())

print("Status:", headers:get(":status"))
print("Body:", body)
