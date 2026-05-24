-- #############################################################################
-- Run: luajit ioli-keygen.lua
-- Dependencies: Lua Bitop (https://bitop.luajit.org/)
-- Description: Keygen for exercise (https://github.com/radareorg/radare2-book/raw/master/src/crackmes/ioli/IOLI-crackme.tar.gz)
-- Author: Ky9oss
-- #############################################################################

local bit = require("bit")

local function random_string(length)
    local chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
    -- local chars = "0123456789"
    local res = {}
    for _ = 1, length do
        local idx = math.random(#chars)
        res[#res + 1] = chars:sub(idx, idx)
    end

    res[1] = "0"
    res[2] = "x"
    res[3] = "k"
    res[4] = "9"
    return table.concat(res)
end

function gen03()
    return (90 + 492) ^ 2
end

function gen04(counter)
    local counter = counter or 0
    if counter >= 30 then
        return "Too many repeats"
    end

    local sum = 0
    local last_number = 0

    local str = random_string(6)

    for i = 1, #str do
        local char = str:sub(i, i)
        if char:match("%d") ~= nil then
            sum = sum + char
            last_number = char
        else
            sum = sum + last_number
        end
    end

    if str:sub(1, 1):match("%d") ~= nil then
        if sum == 15 then
            return str
        elseif sum > 6 and sum < 15 then
            a = 15 - sum
            return (str .. a)
        end
    end

    counter = counter + 1
    return gen04(counter)
end

function gen05(counter)
    local counter = counter or 0
    if counter >= 100 then
        return "Too many repeats"
    end

    local sum = 0
    local last_number = 0

    local str = random_string(6)

    for i = 1, #str do
        local char = str:sub(i, i)
        if char:match("%d") ~= nil then
            sum = sum + char
            last_number = char
        else
            sum = sum + last_number
        end
    end

    if str:sub(1, 1):match("%d") ~= nil then
        if sum > 7 and sum <= 16 then

            if sum ~= 16 then
                a = 16 - sum
                str = str .. a
            end

            if str:sub(#str, #str):match("%d") ~= nil then
                if str:sub(#str, #str):match("%d") % 2 == 0 then
                    return str
                end
            end
        end
    end

    counter = counter + 1
    return gen05(counter)
end

-- Add env var named LOLxxxx and enter the key
function gen06(counter)
    local counter = counter or 0
    if counter >= 500 then
        return "Too many repeats"
    end

    local sum = 0
    local last_number = 0

    local str = random_string(6) -- < 9

    for i = 1, #str do
        local char = str:sub(i, i)
        if char:match("%d") ~= nil then
            sum = sum + char
            last_number = char
        else
            sum = sum + last_number
        end
    end

    if str:sub(1, 1):match("%d") ~= nil then
        if sum > 7 and sum <= 16 then

            if sum ~= 16 then
                a = 16 - sum
                str = str .. a
            end

            if str:sub(#str, #str):match("%d") ~= nil then
                if str:sub(#str, #str):match("%d") % 2 == 0 then
                    return "Add env var named LOLxxxx and enter the key: " .. str
                end
            end
        end
    end

    counter = counter + 1
    return gen06(counter)
end

function gen07()
    return gen06()
end

function gen08()
    return gen06()
end

function gen09()
    return gen06()
end
