#pragma once
#include "GameCamera.h"
#include "SceneLight.h"
#include <list>
#include <memory>

//アプリケーション
#include "application/object/character/enemy/Canota.h"
#include "application/object/character/enemy/Boss.h"

class Player;
class EnemyManager {
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();

	//デバッグ用パラメーター調整
	void DebugWithImGui();

public://getter
	//キャノ太のコンテナ
	const std::list<std::unique_ptr<Canota>>& GetCanotas() const { return canotas_; }
	//ボスのコンテナ
	const std::list<std::unique_ptr<Boss>>& GetBosses() const { return bosses_; }

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
	//キャノ太のコンテナ
	std::list<std::unique_ptr<Canota>> canotas_;
	int numCanotas_ = 4;
	//ボスのコンテナ
	std::list<std::unique_ptr<Boss>> bosses_;
	int numBosses_ = 1;


};

