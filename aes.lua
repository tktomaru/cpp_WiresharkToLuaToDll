-- 今回作成したライブラリの読み込み

-- 下記の関数（tableToStringなど）は以下のサイトのコードを改造した。
-- 改造前のコードはコメントアウトした
-- http://www.lifeaether.com/overtaker/blog/?p=883
function tableToString( t )
	if t[1] then
		return arryaToString( t )
	else
		return dictionaryToString( t )
	end
end

function arryaToString( t )
	-- local s = { "{" }
	local s = {}
	-- local i = 2
	local i = 1
	for j = 1, #t do
		s[i] = valueToString( t[j] )
		-- s[i+1] = ","
		-- i = i + 2
		i = i + 1
	end
	-- s[i-1] = "}"
	return table.concat( s )
end

function dictionaryToString( t )
	local s = { "{" }
	local i = 2
	for key, val in pairs( t ) do
		s[i] = key
		s[i+1] = "="
		s[i+2] = valueToString( val )
		s[i+3] = ","
		i = i + 4
	end
	s[i-1] = "}"
	return table.concat( s )
end

function valueToString( val )
	if type( val ) == "string" then
		return [["]] .. val .. [["]]
	elseif type( val ) == "table" then
		return tableToString( val )
	elseif type( val ) == "number" or type(val) == "boolean" then
		-- return tostring( val )
		return string.format("%02x", val)
	else
		error( "the table contains thread, function or userdata value." )
	end
end

-- 以下の関数は次のサイトから流用した。
-- https://gist.github.com/yi/01e3ab762838d567e65d
function string.fromhex(str)
    return (str:gsub('..', function (cc)
        return string.char(tonumber(cc, 16))
    end))
end

function string.tohex(str)
    return (str:gsub('.', function (c)
        return string.format('%02X', string.byte(c))
    end))
end

function str2hex(str)

  local dd={}
  dd["0"] =  0 dd["1"] =  1
  dd["2"] =  2 dd["3"] =  3
  dd["4"] =  4 dd["5"] =  5
  dd["6"] =  6 dd["7"] =  7
  dd["8"] =  8 dd["9"] =  9
  dd["a"] = 10 dd["b"] = 11
  dd["c"] = 12 dd["d"] = 13
  dd["e"] = 14 dd["f"] = 15
  dd["A"] = 10 dd["B"] = 11
  dd["C"] = 12 dd["D"] = 13
  dd["E"] = 14 dd["F"] = 15

  local obj={}
  local temp
  j = 1
  for i=1,#str,2 do
    temp = dd[string.sub(str,i,i)]
    obj[j]=dd[string.sub(str,i+1,i+1)]
    obj[j] = temp*16 + obj[j]
    j=j+1
  end

  return obj

end

function TableConcat(t1,t2)
    for i=1,#t2 do
        t1[#t1+1] = t2[i]
    end
    return t1
end

--f = package.loadlib("MFCLibrary1.dll", "luaopen_TTaes")
f = package.loadlib("WiresharkUtil.dll", "luaopen_TTaes_enc")
-- f = package.loadlib("WiresharkUtil.dll", "luaopen_TTaes_dec")



function menu_func()

    local tw = TextWindow.new("Test Program")
     -- KeySize
     -- BlockSize
     -- CBC  1  ECB  2 OFB  3  CFB  4 CTS  5
     -- ANSIX923 4 ISO10126 5 None 1 PKCS7 2 Zeros 3
     -- key length
     -- iv length
     -- data length
    encdectable = TableConcat({128, 128, 1, 1, 16, 16, 16,
    0x07,0x01,0x02,0x03,0x04,0x05,0x06,0x02,0x07,0x01,0x02,0x03,0x04,0x05,0x06,0x02, -- key
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, -- iv
    -- 0xAD,0x78,0x0E,0x95,0xC3,0x70,0xA5,0x3C,0xC7,0xC0,0xE2,0xD2,0x79,0x84,0x8A,0x77  -- data
    (str2hex("Ad780e95c370a53cc7c0e2d279848a77")) )
    
    local res = f( encdectable )
    if type(res) == "number" then
      tw:set("Error  !  ".. res .. "\n" .. tableToString(encdectable)) 
    else
      tw:set("Success!  ".. tableToString(res) .. "\n" .. tableToString(encdectable) )
    end

end

-- Register a menu item in one of the main menus.
register_menu("Menu item name", menu_func, MENU_TOOLS_UNSORTED)

