-- 今回作成したライブラリの読み込み

-- 下記の関数（tableToStringなど）は以下のサイトのコードを改造した。
-- 改造前のコードはコメントアウトした
-- http://www.lifeaether.com/overtaker/blog/?p=883
function tableToString( t )
	if t[1] then
		return arryaToString2( t )
	else
		return dictionaryToString2( t )
	end
end

function tableToStringTop( t )
	return "dict "..dictionaryToString( t )
end

function arryaToString2( t )
	local s = { "{" }
	-- local s = { "" }
	local i = 2
	-- local i = 1
	for j = 1, #t do
		s[i] = valueToString2( t[j] )
		s[i+1] = ","
		i = i + 2
		-- i = i + 1
	end
	 s[i-1] = "}"
	return table.concat( s )
end

function arryaToString( t )
	local s = { "{" }
	-- local s = { "" }
	local i = 2
	-- local i = 1
	for j = 1, #t do
		s[i] = valueToString( t[j] )
		s[i+1] = ","
		i = i + 2
		-- i = i + 1
	end
	 s[i-1] = "}"
	return table.concat( s )
end

function dictionaryToString2( t )
	local s = { "{" }
	local i = 2
	for key, val in pairs( t ) do
		s[i] = key
		s[i+1] = "="
		s[i+2] = valueToString2( val )
		s[i+3] = ","
		i = i + 4
	end
	s[i-1] = "}"
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

function valueToString2( val )
	if type( val ) == "string" then
		return [["]] .. val .. [["]]
	elseif type( val ) == "table" then
		-- return dictionaryToString( val )
		return tableToString( val )
	elseif type( val ) == "number" or type(val) == "boolean" then
		return tostring( val )
		--return string.format("%02x", val)
	elseif type( val ) == "nil" then
		return "nil"
	else
		error( "the table contains thread, function or userdata value." )
	end
end

function valueToString( val )
	if type( val ) == "string" then
		return [["]] .. val .. [["]]
	elseif type( val ) == "table" then
		return dictionaryToString( val )
		-- return tableToString( val )
	elseif type( val ) == "number" or type(val) == "boolean" then
		return tostring( val )
		--return string.format("%02x", val)
	elseif type( val ) == "nil" then
		return "nil"
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

-- デバッグ用の再起表示関数
function saiki( s, ret )
    if ( type(ret)=="number" ) then
       s = tostring(ret)
    elseif (type(ret)=="string" ) then
       s = tostring(ret)
    elseif (type(ret)=="nil" ) then
       s = "nil"
    else
      for i=1, #ret do
         s = s .. saiki( s, ret[i] )
      end
    end
    return s
end

-- デバッグ用の再起表示関数
function saiki2( s, ret )
    if ( type(ret)=="number" ) then
       s = tostring(s)
    elseif (type(ret)=="string" ) then
       s = tostring(s)
    else
      if ret[0] ~= nil then
         s1 = saiki( s, ret[0] )
         s = s .. s1
      end
      if ret[1] ~= nil then
         s1 = saiki( s, ret[1] )
         s = s .. s1
      end
      if ret[2] ~= nil then
         s1 = saiki( s, ret[2] )
         s = s .. s1
      end
      if ret[3] ~= nil then
         s1 = saiki( s, ret[3] )
         s = s .. s1
      end
      if ret[4] ~= nil then
         s1 = saiki( s, ret[4] )
         s = s .. s1
      end
    end
    return s
end

function globalIniPath()
   return "D:\\github\\WiresharkToLuaToDll\\lua\\"
end

function getIni()
   getIniFile = package.loadlib("WiresharkUtil.dll", "luaopen_getIniFile")
   getFile = package.loadlib("WiresharkUtil.dll", "luaopen_getFile")
   ini = getIniFile(globalIniPath() .. "setting-CommandList.ini")
   -- ini = getIniFile(globalIniPath() .. "setting.ini")

   local tw = TextWindow.new("Test Program")
   if ( type(ini)=="number" ) then
      tw:set("Error  !  ".. "\n ErrorCode = " .. tableToString(ini)) 
   else
      s = ""
      -- tw:set("Success!  ".. "\n table = " .. tableToStringTop(ini) ) 
      
      for i=1 , #ini do
         s = s .. ini[i]["filename"] .. "\n"
         command = getFile(ini[i]["filename"])
         
         if ( type(command)=="number" ) then
            s = "Error" .. command
            break
         end
         s = s .. tableToStringTop(command) .. "\n\n"
      end
      tw:set("Success!  ".. "\n" .. s) 
   end
end

    aes = package.loadlib("WiresharkUtil.dll", "luaopen_TTaes_enc")
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
    res ="aaa"
    local res = aes( encdectable )
    if type(res) == "nil" then
      tw:set("Error  ! res=nil ") 
    elseif type(res) == "number" then
      tw:set("Error  !  ".. res .. "\n" .. tableToString(encdectable)) 
    else
      tw:set("Success!  \n".. tableToString(res) .. "\n" .. tableToString(encdectable) )
    end

end

-- Register a menu item in one of the main menus.
register_menu("Menu item aes", menu_func, MENU_TOOLS_UNSORTED)
register_menu("Menu item getIni", getIni, MENU_TOOLS_UNSORTED)


-- foo protocol example
-- プロトコルの定義
foo_proto = Proto("foo","Foo","Foo protocol")

-- the new fields that contain the extracted data (one in string form, one in hex)
local exfield_string = ProtoField.new("Extracted String Value", "extract.string", ftypes.STRING)
local exfield_hex    = ProtoField.new("Extracted Hex Value", "extract.hex", ftypes.STRING, base.HEX)
full_coll_name = ProtoField.string("mongodb.full_coll_name", "fullCollectionName", base.UNICODE)

-- register the new fields into our fake protocol
foo_proto.fields = { exfield_string, exfield_hex, full_coll_name }
  
-- パース用の関数定義
function foo_proto.dissector(buffer,pinfo,tree)
    pinfo.cols.protocol = "Foo"
    pinfo.cols.info:append(" " .. tostring("にゃるらと").." -> "..tostring(pinfo.src))
    
   getIniFile = package.loadlib("WiresharkUtil.dll", "luaopen_getIniFile")
   getFile = package.loadlib("WiresharkUtil.dll", "luaopen_getFile")
   inifilename = globalIniPath() .. "setting-CommandList.ini"
   ini = getIniFile(inifilename)

   local subCommandTree = tree:add(foo_proto,buffer(),"Foo Protocol Data")
   if ( type(ini)=="number" ) then
      local tw = TextWindow.new("ini File Open Error (" .. inifilename .. ")")
      tw:set("Error  !  ".. "\n ErrorCode = " .. tableToString(ini)) 
   else
      s = ""
      
      for i=1 , #ini do
         
         local hex = ini[i]["value"][1]["value"]
         local filename = ini[i]["filename"]
         
         --local tw = TextWindow.new("Test Program1")
         --tw:set("Success  !  ".. "\n filename = " .. tableToString(ini[i]["value"][1]) ) 
            
         if( hex == 0x0101 ) then
         
            command = getFile(filename)
            
            --local tw = TextWindow.new("Test Program2")
            --tw:set("Success  !  ".. "\n filename = " .. tableToString(command)) 
            
            if ( type(command)=="number" ) then
                 local tw = TextWindow.new("File Open Error (" .. filename .. ")")
                 tw:set("Error  !  ".. "\n ErrorCode = " .. tableToString(command)) 
               break
            else
               for j=1, #command do
                  for k=1, #command[j]["value"] do
                     length = command[j]["value"][k]["length"]
                     start = command[j]["value"][k]["start"]
                     str = command[j]["value"][k]["str"]
                     subCommandTree:add(buffer(start,length), str)
                  end
               end
            end
         end
         
      end
   end
    
    tree:add(exfield_hex, 0xEE)
    
    tree:add(full_coll_name, "にゃるらと")
    tree:add(full_coll_name, buffer(0,1), "にゃるらと")
    
    local subtree = tree:add(foo_proto,buffer(),"Foo Protocol Data")
    
    subtree:add(exfield_string, buffer(0,1), "cmdコマンド=" .. ("ヘルスチェック") .. buffer(0,1):uint() )
    subtree:add(buffer(0,1),"cmd_id(コマンド): " .. buffer(0,1):uint())
    subtree:add(buffer(1,1),"req_id: " .. buffer(1,1):le_uint())
    subtree:add(buffer(2,6),"date: " .. buffer(2,1):le_uint() .. "/" .. buffer(3,1):le_uint() .. "/"  .. buffer(4,1):le_uint() .. " "  .. buffer(5,1):le_uint() .. ":"  .. buffer(6,1):le_uint() .. ":"  .. buffer(7,1):le_uint())
    subtree:add(buffer(8,2),"seq_no: " .. buffer(8,2):le_uint())
    subtree:add(buffer(10,1),"rty_cnt: " .. buffer(10,1):le_uint())
end

-- tcp.portテーブルのロード
tcp_table = DissectorTable.get("tcp.port")
-- ポート9999番とプロトコルの紐付けをする
tcp_table:add(1024,foo_proto)


