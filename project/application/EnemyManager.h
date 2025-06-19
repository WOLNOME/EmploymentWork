#pragma once
#include "GameCamera.h"
#include "SceneLight.h"
#include <list>
#include <memory>

//アプリケーション
#include "application/object/enemy/Enemy.h"
#include "application/object/enemy/Boss.h"

class Player;
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

public://getter
	//敵のコンテナ
	const std::list<std::unique_ptr<Enemy>>& GetEnemies() const { return enemies_; }
	//ボスのポインタ
	Boss* GetBoss() const { return boss_.get(); }

public://setter
	//カメラセット
	void SetCamera(GameCamera* _camera);
	//ライトセット
	void SetLight(SceneLight* _light);
	//プレイヤーセット
	void SetPlayer(Player* _player);

private:
	//カメラ
	GameCamera* camera_ = nullptr;
	//シーンライト
	SceneLight* sceneLight_ = nullptr;
	//プレイヤー
	Player* player_ = nullptr;

private:
	//敵のコンテナ
	std::list<std::unique_ptr<Enemy>> enemies_;
	int numEnemies_ = 4;
	std::unique_ptr<Boss> boss_ = nullptr;

};

