#include "pch.h"
#include "Common.h"
#include "fileUtil.h"


int isExists(const fs::path& p, fs::file_status s = fs::file_status{})
{
	if (fs::status_known(s) ? fs::exists(s) : fs::exists(p))
		return 1;
	else
		// does not exist
		return 0;
}

/**
* @brief �t�H���_�ȉ��̃t�@�C���ꗗ���擾����֐�
* @param[in]    folderPath  �t�H���_�p�X
* @param[out]   file_names  �t�@�C�����ꗗ
* return        true:����, false:���s
*/
int getFileNames(lua_State* L)
//bool getFileNames(std::string folderPath, std::vector<std::string>& file_names)
{
	using namespace std::filesystem;
	std::error_code err;

	std::string		folderPath;				// ���Ұ�:̧�ٖ���
	int			iAnswer = 0;			// �߂�l
	std::vector<std::string> file_names;
	directory_iterator iter, end;
	int folderExist=0;

	//----------------------------------------------
	// �����̐�������
	//----------------------------------------------
	int n = lua_gettop(L);
	if (n <= 0 || n > 1) {
		// �����͈͊O
		iAnswer = 1;
		goto ExitBlock;
	}
	//----------------------------------------------
	// �����̉�́��擾
	//----------------------------------------------
	// ����1 ̧�ٖ��̂̌^����
	
	if (lua_isstring(L, 1) != 1) {
		// ̧�ٖ��̕�����F���װ
		iAnswer = 2;
		goto ExitBlock;
	}

	folderPath = lua_tostring(L, 1);

	folderExist=isExists(folderPath);

	if (folderExist != 1) {
		// �t�H���_�F���װ
		iAnswer = 3;
		goto ExitBlock;
	}

	iter = std::filesystem::directory_iterator(folderPath);
	
	for (; iter != end && !err; iter.increment(err)) {
		const directory_entry entry = *iter;
		std::string fileName = entry.path().string();
		file_names.push_back(entry.path().string());
	}
	lua_pushVectorString(L, file_names);

ExitBlock:
	if (iAnswer > 0) {
		// �װ�����������Ұ����
		lua_pushnumber(L, iAnswer);
		// ���Ұ�����Ԃ�
		return 1;
	}
	// ���Ұ�����Ԃ�
	return 1;
}

vector<string> split(string& input, char delimiter)
{
	istringstream stream(input);
	string field;
	vector<string> result;
	while (getline(stream, field, delimiter)) {
		result.push_back(field);
	}
	return result;
}

void lua_pushtableTRET2(lua_State* L, tret2 ary)
{
	BYTE* lpBuffer = NULL;
	int sum = 0;
	DWORD ivec = 0;
	int optnum = 0;

	lua_newtable(L);
	int c = lua_gettop(L);
	for (; ivec < ary.col.size(); ivec++) {
		optnum = 1;
		lua_pushnumber(L, ivec + 1);
		lua_newtable(L);

		if (ary.col[ivec].filename != "") {
			lua_pushString(L, "filename");
			lua_pushString(L, ary.col[ivec].filename);
			lua_settable(L, -3);
		}

		lua_pushString(L, "help");
		lua_pushString(L, ary.col[ivec].help);
		lua_settable(L, -3);

		lua_pushString(L, "count");
		lua_pushnumber(L, ary.col[ivec].count);
		lua_settable(L, -3);

		lua_pushString(L, "maxcount");
		lua_pushnumber(L, ary.col[ivec].maxcount);
		lua_settable(L, -3);

		lua_pushString(L, "str");
		lua_pushString(L, ary.col[ivec].str);
		lua_settable(L, -3);

		if (ary.col[ivec].format != "") {
			lua_pushString(L, "format");
			lua_pushString(L, ary.col[ivec].format);
			lua_settable(L, -3);
		}
		if(ary.col[ivec].formatvalue != "") {
			lua_pushString(L, "formatvalue");
			lua_pushString(L, ary.col[ivec].formatvalue);
			lua_settable(L, -3);
		}

		lua_pushString(L, "start");
		lua_pushnumber(L, ary.col[ivec].start);
		lua_settable(L, -3);

		lua_pushString(L, "length");
		lua_pushnumber(L, ary.col[ivec].length);
		lua_settable(L, -3);

		if (ary.col[ivec].int1) {
			lua_pushnumber(L, optnum++);
			lua_pushnumber(L, ary.col[ivec].int1);
			lua_settable(L, -3);
		}
		if (ary.col[ivec].int2) {
			lua_pushnumber(L, optnum++);
			lua_pushnumber(L, ary.col[ivec].int2);
			lua_settable(L, -3);
		}
		if (ary.col[ivec].str1 != "") {
			lua_pushnumber(L, optnum++);
			lua_pushString(L, ary.col[ivec].str1);
			lua_settable(L, -3);
		}
		if (ary.col[ivec].str2 != "") {
			lua_pushnumber(L, optnum++);
			lua_pushString(L, ary.col[ivec].str2);
			lua_settable(L, -3);
		}

		lua_settable(L, c);
	}
}

void lua_pushtableTRET(lua_State* L, tret ary)
{
	BYTE* lpBuffer = NULL;
	int sum = 0;
	int ivec = -1;
	int optnum = 0;

	lua_newtable(L);
	lua_pushnumber(L, 1);
	lua_newtable(L);
	lua_settable(L, -3);
	int c = lua_gettop(L);
	for (ivec = 0; ivec < ary.col.size(); ivec++) {
		lua_pushnumber(L, ivec + 1);
		lua_newtable(L);

		if (ary.col[ivec].filename != "") {
			lua_pushString(L, "filename");
			lua_pushString(L, ary.col[ivec].filename);
			lua_settable(L, -3);
		}

		if (ary.col[ivec].help != "") {
			lua_pushString(L, "help");
			lua_pushString(L, ary.col[ivec].help);
			lua_settable(L, -3);
		}

		if (ary.col[ivec].count != NULL) {
			lua_pushString(L, "count");
			lua_pushnumber(L, ary.col[ivec].count);
			lua_settable(L, -3);
		}

		if (ary.col[ivec].maxcount != NULL) {
			lua_pushString(L, "maxcount");
			lua_pushnumber(L, ary.col[ivec].maxcount);
			lua_settable(L, -3);
		}


		if (ary.col[ivec].int1) {
			lua_pushnumber(L, optnum++);
			lua_pushnumber(L, ary.col[ivec].int1);
			lua_settable(L, -3);
		}
		if (ary.col[ivec].int2) {
			lua_pushnumber(L, optnum++);
			lua_pushnumber(L, ary.col[ivec].int2);
			lua_settable(L, -3);
		}
		if (ary.col[ivec].str1 != "") {
			lua_pushnumber(L, optnum++);
			lua_pushString(L, ary.col[ivec].str1);
			lua_settable(L, -3);
		}
		if (ary.col[ivec].str2 != "") {
			lua_pushnumber(L, optnum++);
			lua_pushString(L, ary.col[ivec].str2);
			lua_settable(L, -3);
		}

		lua_pushString(L, "value");
		lua_newtable(L);
		int c2 = lua_gettop(L);
		for (DWORD ival = 0; ival < ary.col[ivec].value.size(); ival++) {
			lua_pushnumber(L, ival + 1);
			lua_newtable(L);

			lua_pushString(L, "start");
			lua_pushnumber(L, ary.col[ivec].value[ival].start);
			lua_settable(L, -3);

			lua_pushString(L, "length");
			lua_pushnumber(L, ary.col[ivec].value[ival].length);
			lua_settable(L, -3);

			lua_pushString(L, "value");
			lua_pushnumber(L, ary.col[ivec].value[ival].value);
			// lua_setfield(L, -2, ivec + 1);
			lua_settable(L, -3);

			lua_pushString(L, "str");
			lua_pushString(L, ary.col[ivec].value[ival].str);
			// lua_setfield(L, -2, ivec + 1);
			lua_settable(L, -3);

			lua_settable(L, c2);
		}
		lua_settable(L, -3);
		lua_settable(L, c);
	}
}


string reg(string s)
{
	string out;
	std::smatch match;
	if ( std::regex_search(s, match, std::regex("\".*?\"")) ) {
		out = match.str();      //  (�}�b�`����������)
		//auto position = results.position(); //   (�}�b�`����������̈ʒu)
		//auto length = results.length();   //   (�}�b�`����������̒���)
		out = ReplaceString(out, "\"", "");
	}
	if (std::regex_search(s, match, std::regex("\'.*?\'"))) {
		out = match.str();      //  (�}�b�`����������)
		//auto position = results.position(); // (�}�b�`����������̈ʒu)
		//auto length = results.length();   //   (�}�b�`����������̒���)
		out = ReplaceString(out, "\'", "");
	}
	return out;
}


/*
	string���̓��蕶�����string�Œu������i��肩���j
*/
std::string ReplaceString
(
	std::string StringTarget   // �u�������Ώ�
	, std::string StringFrom   // �����Ώ�
	, std::string StringTo   // �u����������e
)
{
	string strE{'"'};
	string strQ{"'"};

	std::string::size_type  Pos(StringTarget.find(StringFrom));

	while (Pos != std::string::npos)
	{
		// �u��
		StringTarget.replace(Pos, StringFrom.length(), StringTo);
		Pos = StringTarget.find(StringFrom, Pos + StringTo.length());
	}

	return StringTarget;
}

int UtilStoi(const std::string& str, LONG64* p_value, std::size_t* pos = 0, int base = 10) {
	// wrapping std::stoi because it may throw an exception

	try {
		*p_value = std::stoll(str, pos, base);
		return 0;
	}

	catch (const std::invalid_argument& ia) {
		//std::cerr << "Invalid argument: " << ia.what() << std::endl;
		return 5;
	}

	catch (const std::out_of_range& oor) {
		//std::cerr << "Out of Range error: " << oor.what() << std::endl;
		return 6;
	}

	catch (const std::exception& e)
	{
		//std::cerr << "Undefined error: " << e.what() << std::endl;
		return 7;
	}
}

int getIniFile(lua_State* L)
{
	using namespace std::filesystem;
	std::error_code err;

	fs::path 		inifilePath;				// ���Ұ�:̧�ٖ���
	int			iAnswer = 0;			// �߂�l
	std::vector<std::string> file_names;
	directory_iterator iter, end;
	int folderExist = 0;
	std::ifstream   ifs;
	std::string  strline="";
	vector<string> setting;
	tret ret;
	int sret;
	string errorbuf;
	
	ret.col.clear();

	//----------------------------------------------
	// �����̐�������
	//----------------------------------------------
	int n = lua_gettop(L);
	if (n <= 0 || n > 1) {
		// �����͈͊O
		iAnswer = 1;
		goto ExitBlock;
	}
	//----------------------------------------------
	// �����̉�́��擾
	//----------------------------------------------
	// ����1 ̧�ٖ��̂̌^����

	if (lua_isstring(L, 1) != 1) {
		// ̧�ٖ��̕�����F���װ
		iAnswer = 2;
		goto ExitBlock;
	}

	inifilePath = fs::u8path(lua_tostring(L, 1));
	lua_pop(L, 1);

	folderExist = isExists(inifilePath);

	if (folderExist != 1) {
		// �t�H���_�F���װ
		iAnswer = 3;
		goto ExitBlock;
	}

	ifs.open(inifilePath.c_str());

	if (ifs.fail())
	{
		// �t�@�C���F���װ
		iAnswer = 3;
		goto ExitBlock;
	}

	// ��s�ڂ̐ݒ��ǂݍ���
	int ilineNum = 0;
	LONG64 ivectnum = 0;
	s_mapStringValues["filename"] = evStringValue1;
	s_mapStringValues["num"] = evStringValue2;
	s_mapStringValues["dec"] = evStringValue3;
	s_mapStringValues["hex"] = evStringValue4;
	s_mapStringValues["start"] = evStringValue5;
	s_mapStringValues["length"] = evStringValue6;
	s_mapStringValues["help"] = evStringValue7;
	s_mapStringValues["int1"] = evStringValue8;
	s_mapStringValues["int2"] = evStringValue9;
	s_mapStringValues["str1"] = evStringValue10;
	s_mapStringValues["str2"] = evStringValue11;
	s_mapStringValues["count"] = evStringValue12;
	s_mapStringValues["maxcount"] = evStringValue13;
	s_mapStringValues["str"] = evStringValue14;

	while (getline(ifs, strline))
	{
		// strline�Ɉ�s�̕����񂪓����Ă���
		// �R�����g�s�͓ǂݔ�΂�
		if (strline.substr(0, 1) == ";") {
			continue;
		}

		if (strline.empty()) {
			continue;
		}
		if (strline == "") {
			continue;
		}

		if (ilineNum == 0) {
			// ,��؂�ŕ�������
			// �s�͐ݒ肪�����Ă���
			ilineNum = 1;
			strline = ReplaceString(strline, " ", "");
			setting = split(strline, ',');
		}
		else {
			ilineNum++;
			ivectnum = -1;
			int hexOrDecNum = 0;
			// ���ۂ̒l�𔻕ʂ���
			tcol col1;
			tval value = { 0,0,0,"" };
			strline = strline;
			vector<string> strvec = split(strline, ',');
			for (int ii = 0; ii < strvec.size() && ii < setting.size(); ii++) {

				string strcolumn = reg(strvec[ii]);

				if (strcolumn.empty()) {
					strcolumn = ReplaceString(strvec[ii], " ", "");
				}
				switch (s_mapStringValues[setting[ii]]) {
				case evStringValue1:
					// filename
					col1.filename = strcolumn;
					break;
				case evStringValue2:
					// num
					sret = UtilStoi(strcolumn, &ivectnum);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue3:
					// dec
					sret = UtilStoi(strcolumn, &value.value);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					hexOrDecNum++;
					break;
				case evStringValue4:
					// hex
					hexOrDecNum++;
					sret = UtilStoi(strcolumn, &value.value, nullptr, 16);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue5:
					// start
					sret = UtilStoi(strcolumn, &value.start);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue6:
					// length
					sret = UtilStoi(strcolumn, &value.length);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue7:
					// help
					col1.help = strcolumn;
					break;
				case evStringValue8:
					// int1
					sret = UtilStoi(strcolumn, &col1.int1);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue9:
					// int2
					sret = UtilStoi(strcolumn, &col1.int2);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue10:
					// str1
					col1.str1 = strcolumn;
					break;
				case evStringValue11:
					// str2
					col1.str2 = strcolumn;
					break;
				case evStringValue12:
					// count
					sret = UtilStoi(strcolumn, &col1.count);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue13:
					// maxcount
					sret = UtilStoi(strcolumn, &col1.maxcount);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				default:
					// �\�����Ȃ�������̓G���[
					iAnswer = 4;
					errorbuf = setting[ii];
					goto ExitBlock;
					break;
				}
				if (value.length != 0 && value.start != 0 && value.value != 0) {
					col1.value.push_back(value);
					value = { 0,0,0,"" };
				}
			}
			if (col1.filename == "") {
				iAnswer = 8;
				goto ExitBlock;
			}
			if (col1.value.empty()) {
				iAnswer = 9;
				goto ExitBlock;
			}
			ret.col.push_back(col1);
			col1.value.clear();
		}
	}
	lua_pushtableTRET(L, ret);

ExitBlock:
	if (iAnswer == 4) {
		// �װ�����������Ұ����
		lua_pushstring(L, errorbuf.c_str());
		// ���Ұ�����Ԃ�
		return 1;
	}
	if (iAnswer > 0) {
		// �װ�����������Ұ����
		lua_pushnumber(L, iAnswer);
		// ���Ұ�����Ԃ�
		return 1;
	}
	// ���Ұ�����Ԃ�
	return 1;
}

// getIniFile�̃t�@�C�������̋�`�F�b�N���Ȃ�
int getFile(lua_State* L)
{
	using namespace std::filesystem;
	std::error_code err;

	fs::path 		inifilePath;				// ���Ұ�:̧�ٖ���
	int			iAnswer = 0;			// �߂�l
	std::vector<std::string> file_names;
	directory_iterator iter, end;
	int folderExist = 0;
	std::ifstream ifs;
	std::string strline;
	vector<string> setting;
	tret2 ret = {};
	int sret;
	string errorbuf;


	//----------------------------------------------
	// �����̐�������
	//----------------------------------------------
	int n = lua_gettop(L);
	if (n <= 0 || n > 1) {
		// �����͈͊O
		iAnswer = 1;
		goto ExitBlock;
	}
	//----------------------------------------------
	// �����̉�́��擾
	//----------------------------------------------
	// ����1 ̧�ٖ��̂̌^����

	if (lua_isstring(L, 1) != 1) {
		// ̧�ٖ��̕�����F���װ
		iAnswer = 2;
		goto ExitBlock;
	}

	inifilePath = fs::u8path(lua_tostring(L, 1));

	folderExist = isExists(inifilePath);

	if (folderExist != 1) {
		// �t�H���_�F���װ
		iAnswer = 3;
		goto ExitBlock;
	}

	ifs.open(inifilePath.c_str());

	if (ifs.fail())
	{
		// �t�@�C���F���װ
		iAnswer = 3;
		goto ExitBlock;
	}

	// ��s�ڂ̐ݒ��ǂݍ���
	int ilineNum = 0;
	LONG64 ivectnum = 0;
	s_mapStringValues["filename"] = evStringValue1;
	s_mapStringValues["num"] = evStringValue2;
	s_mapStringValues["dec"] = evStringValue3;
	s_mapStringValues["hex"] = evStringValue4;
	s_mapStringValues["start"] = evStringValue5;
	s_mapStringValues["length"] = evStringValue6;
	s_mapStringValues["help"] = evStringValue7;
	s_mapStringValues["int1"] = evStringValue8;
	s_mapStringValues["int2"] = evStringValue9;
	s_mapStringValues["str1"] = evStringValue10;
	s_mapStringValues["str2"] = evStringValue11;
	s_mapStringValues["count"] = evStringValue12;
	s_mapStringValues["maxcount"] = evStringValue13;
	s_mapStringValues["str"] = evStringValue14;
	s_mapStringValues["format"] = evStringValue15;
	s_mapStringValues["formatvalue"] = evStringValue16;

	while (getline(ifs, strline))
	{
		// strline�Ɉ�s�̕����񂪓����Ă���
		// �R�����g�s�͓ǂݔ�΂�
		if (strline.substr(0, 1) == ";") {
			continue;
		}
		if (strline.empty()) {
			continue;
		}
		if (strline == "") {
			continue;
		}

		if (ilineNum == 0) {
			// ,��؂�ŕ�������
			// �s�͐ݒ肪�����Ă���
			strline = ReplaceString(strline, " ", "");
			setting = split(strline, u8',');
		}
		else {
			ivectnum = -1;
			// ���ۂ̒l�𔻕ʂ���
			tcol2 col1 = { "","",0,0,"","",0,0,"",0,0,0,""};
			tval value = { -1,0,0,"" };
			strline = strline;
			vector<string> strvec = split(strline, ',');
			for (int ii = 0; ii < strvec.size() && ii < setting.size(); ii++) {

				string strcolumn = reg(strvec[ii]);

				if (strcolumn.empty()) {
					strcolumn = ReplaceString(strvec[ii], " ", "");
				}
				switch (s_mapStringValues[setting[ii]]) {
				case evStringValue1:
					// filename
					col1.filename = strcolumn;
					break;
				case evStringValue2:
					// num
					sret = UtilStoi(strcolumn, &ivectnum);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue3:
					// dec
					sret = UtilStoi(strcolumn, &col1.value);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue4:
					// hex
					sret = UtilStoi(strcolumn, &col1.value, nullptr, 16);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue5:
					// start
					sret = UtilStoi(strcolumn, &col1.start);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue6:
					// length
					sret = UtilStoi(strcolumn, &col1.length);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue7:
					// help
					col1.help = strcolumn;
					break;
				case evStringValue8:
					// int1
					sret = UtilStoi(strcolumn, &col1.int1);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue9:
					// int2
					sret = UtilStoi(strcolumn, &col1.int2);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue10:
					// str1
					col1.str1 = strcolumn;
					break;
				case evStringValue11:
					// str2
					col1.str2 = strcolumn;
					break;
				case evStringValue12:
					// str2
					sret = UtilStoi(strcolumn, &col1.count);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue13:
					// maxcount
					sret = UtilStoi(strcolumn, &col1.maxcount);
					if (sret != 0) {
						iAnswer = sret;
						goto ExitBlock;
					}
					break;
				case evStringValue14:
					// str
					col1.str = strcolumn;
					break;
				case evStringValue15:
					// format
					col1.format = strcolumn;
					break;
				case evStringValue16:
					// formatvalue
					col1.formatvalue = strcolumn;
					break;
				default:
					// �\�����Ȃ�������̓G���[
					iAnswer = 4;
					errorbuf = setting[ii];
					goto ExitBlock;
					break;
				}
				//if (value.length != 0 && value.start != -1 && value.str != "") {
				//	col1.value.push_back(value);
				//	value = { 0,0,0 };
				//}
			}
			/*
			if (col1.filename == "") {
				iAnswer = 8;
				goto ExitBlock;
			}*/
			//if (col1.value.empty()) {
			//	iAnswer = 9;
			//	goto ExitBlock;
			//}
			ret.col.push_back(col1);
			//col1.value.clear();
		}
		ilineNum++;
		lua_pushtableTRET2(L, ret);
	}

ExitBlock:
	if (iAnswer == 4) {
		// �װ�����������Ұ����
		lua_pushstring(L, errorbuf.c_str());
		// ���Ұ�����Ԃ�
		return 1;
	}
	if (iAnswer > 0) {
		// �װ�����������Ұ����
		lua_pushnumber(L, iAnswer);
		// ���Ұ�����Ԃ�
		return 1;
	}
	// ���Ұ�����Ԃ�
	return 1;
}