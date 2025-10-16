#pragma once
#include "BaseScene.h"
#include <string>

/// <summary>
/// 文字列を入力してシーンを生成する基底クラス
/// </summary>
class AbstractSceneFactory {
public:
	//仮想デストラクタ
	virtual ~AbstractSceneFactory() = default;
	//シーン生成
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;
};

