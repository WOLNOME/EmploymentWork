#pragma once
#include <string>

/// <summary>
/// ランダムな文字列生成用クラス
/// </summary>
class RandomStringUtil {
public:
	// 乱数生成
	static std::string GenerateRandomString(size_t length);
};

