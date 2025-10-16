#pragma once
#include "JsonUtil.h"

//アプリケーション
#include "detail/EnemyHPUI.h"

class GameCamera;
class EnemyManager;

/// <summary>
/// 敵に関するUI全てを管理するクラス
/// </summary>
class EnemyUI {
public:
	//初期化
	void Initialize();
	//更新
	void Update();

	//デバッグ用ImGui
	void DebugWithImGui();

public://setter
	void SetEnemyManager(EnemyManager* _enemyManager);
	void SetGameCamera(GameCamera* _camera);
private:
	//HPUI
	std::unique_ptr<EnemyHPUI> enemyHPUI_ = nullptr;

private://データ駆動設計用パラメータ
	json param_;

};

