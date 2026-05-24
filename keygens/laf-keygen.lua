-- #############################################################################
-- Interpreter: luajit
-- Dependencies: Lua Bitop (https://bitop.luajit.org/)
-- Description: Keygen for exercise (https://github.com/DazzleCatDuo/X86-SOFTWARE-REVERSE-ENGINEERING-CRACKING-AND-COUNTER-MEASURES/tree/main/Lab%20-%20Cracking%20LaFarge)
-- Author: Ky9oss
-- #############################################################################


local name = "cracker"
local i_406328 = "aa89c4fe46"

-- Rectangle = {}
-- function Rectangle:calculateArea() return self.length * self.width end
--
-- Rectangle.mt = {               
--   __index = Rectangle,
--   __lt = function(a, b)
--     return a:calculateArea() < b:calculateArea()
--   end
-- }
--
-- function Rectangle.new(length, width)
--   local obj = { length = length, width = width }
--   setmetatable(obj, Rectangle.mt)   
--   return obj
-- end
--
--
-- setmetatable(Rectangle, {
--   __call = function(_, length, width)
--     return Rectangle.new(length, width)
--   end
-- })
--
-- local rectangles = { Rectangle(2, 5), Rectangle(7, 9), Rectangle(1, 3),
--   Rectangle(3, 4), Rectangle(9, 10), Rectangle(2, 1) }
-- table.sort(rectangles)
-- for i,v in ipairs(rectangles) do
--   print(tostring(i)..": "..tostring(v:calculateArea()).." in "..tostring(v.length).."x"..tostring(v.width))
-- end

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
