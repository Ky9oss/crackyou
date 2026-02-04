-- luajit
--
-- For https://github.com/DazzleCatDuo/X86-SOFTWARE-REVERSE-ENGINEERING-CRACKING-AND-COUNTER-MEASURES/tree/main/Lab%20-%20Cracking%20LaFarge
-- Author: Ky9oss


local name = "cracker"
local i_406328 = "aa89c4fe46"

print('[edx]')
for i = 1, #name do
    local ii = i % 5 ~= 0 and i % 5 or 5
    local xor_hex = tonumber(i_406328:sub(2*ii-1, 2*ii), 16)
    print(string.format("0x%X", bit.bxor(string.byte(name:sub(ii, ii)), xor_hex)))
end

print('[0x406328]')
for i = #name-5+1, #name do
    print(string.format("0x%X", string.byte(name:sub(i, i))))
end
