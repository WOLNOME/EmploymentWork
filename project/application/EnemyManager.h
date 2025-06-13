#pragma once
#include "BaseCamera.h"
#include "SceneLight.h"
#include <list>
#include <memory>

#include "application/object/enemy/Enemy.h"
#include "application/object/player/Player.h"
class EnemyManager {
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//線描画
	void DrawLine();

	//デバッグ用パラメーター調整
	void DebugWithImGui();

public://setter
	//カメラセット
	void SetCamera(BaseCamera* _camera);
	//ライトセット
	void SetLight(SceneLight* _light);
	//プレイヤーセット
	void SetPlayer(Player* _player);

private:
	//カメラ
	BaseCamera* camera_ = nullptr;
	//シーンライト
	SceneLight* sceneLight_ = nullptr;
	//プレイヤー
	Player* player_ = nullptr;

private:
	//敵のコンテナ
	std::list<std::unique_ptr<Enemy>> enemies_;
	int numEnemies_ = 4;

};

