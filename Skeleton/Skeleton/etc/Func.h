#pragma once
#include <string>

#define PI 3.14159265f
#define RAD(X) (X) * PI / 180.0f

namespace func
{
	// ワイド文字列の変換
	std::wstring ChangeWString(const std::string& st);

	// 文字列の検索
	std::string FindString(const std::string& path, const char& find, const unsigned int& offset = 1, const bool& end = true);
}
