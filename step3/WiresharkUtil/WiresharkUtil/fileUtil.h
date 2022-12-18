#pragma once

#include "pch.h"
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <map>
#include <iostream> // std::cout
#include <regex>


using namespace std;
namespace fs = filesystem;

typedef struct _tval
{
	LONG64     start;
	LONG64     length;
	LONG64     value;
	string     str;
}tval;

typedef struct _tcol2
{
	string       filename;
	string       help;
	LONG64       count;
	LONG64       maxcount;
//	vector<tval> value;
	string       str1;
	string       str2;
	LONG64       int1;
	LONG64       int2;
	string       format;
	LONG64       start;
	LONG64       length;
	LONG64       value;
	string       str;
	string       formatvalue;
} tcol2;

typedef struct _tcol
{
	string       filename;
	string       help;
	LONG64       count;
	LONG64       maxcount;
	vector<tval> value;
	string       str1;
	string       str2;
	LONG64       int1;
	LONG64       int2;
	string       format;
} tcol;

typedef struct _tret
{
	vector< tcol > col;
} tret;

typedef struct _tret2
{
	vector< tcol2 > col;
} tret2;


// Value-Defintions of the different String values
static enum StringValue {
	evNotDefined,
	evStringValue1,
	evStringValue2,
	evStringValue3,
	evStringValue4,
	evStringValue5,
	evStringValue6,
	evStringValue7,
	evStringValue8,
	evStringValue9,
	evStringValue10,
	evStringValue11,
	evStringValue12,
	evStringValue13,
	evStringValue14,
	evStringValue15,
	evStringValue16,
	evEnd
};

// Map to associate the strings with the enum values
static std::map<std::string, StringValue> s_mapStringValues;
int getFileNames(lua_State* L);
vector<string> split(string& input, char delimiter);
void lua_pushtableARY(lua_State* L, tret ary);
std::string ReplaceString
(
	std::string StringTarget   // íuÇ´ä∑Ç¶ëŒè€
	, std::string StringFrom   // åüçıëŒè€
	, std::string StringTo   // íuÇ´ä∑Ç¶ÇÈì‡óe
);
string reg(string s);
int UtilStoi(const std::string& str, LONG64* p_value, std::size_t* pos , int base );
int getIniFile(lua_State* L);
int getFile(lua_State* L);