#pragma once
#include "BaseCamera.h"
#include "Object3d.h"
#include "JsonUtil.h"
#include <list>
#include <string>
#include <memory>

class LevelObject {
private://構造体
	//自キャラの生成データ
	struct PlayerSpawnData {
		Vector3 translation; //位置
		Vector3 rotation; //回転角
	};
	//レベルデータ
	struct LevelData {
		//オブジェクトのコンテナ
		std::list<std::unique_ptr<Object3d>> objects;
		//自キャラコンテナ
		std::list<PlayerSpawnData> players;
	};


public:
	//初期化(.jsonは省略)
	void Initialize(const std::string& _filePath);
	//更新
	void Update();
	//デバッグ
	void DebugWithImGui();

public://setter
	void SetCamera(BaseCamera* _camera) {
		camera_ = _camera;
	}

private://クラス内限定処理
	//オブジェクトデータの走査
	void ScanObjectData(json& object);

private:
	//カメラ
	BaseCamera* camera_ = nullptr;

	//レベルデータ
	LevelData levelData_;

	//<!>テクスチャはレベルエディターが対応していないため実装不可

};

