#pragma once
#include <string>

#define PI 3.14159265f
#define RAD(X) (X) * PI / 180.0f

namespace func
{
	// ワイド文字列の変換
	std::wstring ChangeWString(const std::string& st);
}
