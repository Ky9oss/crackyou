Rectangle = {}
function Rectangle:calculateArea() return self.length * self.width end

Rectangle.mt = {               
  __index = Rectangle,
  __lt = function(a, b)
    return a:calculateArea() < b:calculateArea()
  end
}

function Rectangle.new(length, width)
  local obj = { length = length, width = width }
  setmetatable(obj, Rectangle.mt)   
  return obj
end


setmetatable(Rectangle, {
  __call = function(_, length, width)
    return Rectangle.new(length, width)
  end
})

local rectangles = { Rectangle(2, 5), Rectangle(7, 9), Rectangle(1, 3),
  Rectangle(3, 4), Rectangle(9, 10), Rectangle(2, 1) }
table.sort(rectangles)
for i,v in ipairs(rectangles) do
  print(tostring(i)..": "..tostring(v:calculateArea()).." in "..tostring(v.length).."x"..tostring(v.width))
end
