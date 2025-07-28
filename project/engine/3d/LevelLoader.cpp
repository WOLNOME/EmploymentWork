#include "LevelLoader.h"
#include "Object3dManager.h"
#include <assert.h>

void LevelLoader::Initialize() {
	//jsonデータを読み込む
	json data = JsonUtil::GetJsonData("Resources/levelData/levelData");
	//全オブジェクトデータのロード
	for (json& object : data["objects"]) {
		ScanObjectData(object);
	}
}

void LevelLoader::Update() {
}

void LevelLoader::DebugWithImGui() {
}

void LevelLoader::ScanObjectData(json& object) {
	//"type"データがない場合不正データのため警告
	assert(object.contains("type"));

	//種別を取得
	std::string type = object["type"].get<std::string>();

	//メッシュオブジェクトの処理
	if (type.compare("MESH") == 0) {
		std::unique_ptr<Object3d> addObject;
		addObject = std::make_unique<Object3d>();
		addObject->Initialize(ModelTag{},Object3dManager::GetInstance()->GenerateName("levelLoader"), object["file_name"]);

		//トランスフォームパラメータの読み込み
		json& transform = object["transform"];
		//平行移動
		addObject->worldTransform.translate.x = (float)transform["translation"][0];
		addObject->worldTransform.translate.y = (float)transform["translation"][2];
		addObject->worldTransform.translate.z = (float)transform["translation"][1];
		//回転角
		addObject->worldTransform.rotate.x = -(float)transform["rotation"][0];
		addObject->worldTransform.rotate.y = -(float)transform["rotation"][2];
		addObject->worldTransform.rotate.z = -(float)transform["rotation"][1];
		//スケーリング
		addObject->worldTransform.scale.x = (float)transform["scaling"][0];
		addObject->worldTransform.scale.y = (float)transform["scaling"][2];
		addObject->worldTransform.scale.z = (float)transform["scaling"][1];

		//<!>コライダーは今はスキップ(システム考案中)

		//オブジェクトリストに登録
		levelData_.objects.push_back(std::move(addObject));
	}
	//自キャラ発生ポイント
	else if (type.compare("PlayerSpawn") == 0) {
		PlayerSpawnData data;
		
		//トランスフォームパラメータの読み込み
		json& transform = object["transform"];
		//平行移動
		data.translation.x = (float)transform["translation"][0];
		data.translation.y = (float)transform["translation"][2];
		data.translation.z = (float)transform["translation"][1];
		//回転角
		data.rotation.x = -(float)transform["rotation"][0];
		data.rotation.y = -(float)transform["rotation"][2];
		data.rotation.z = -(float)transform["rotation"][1];

		//自キャラコンテナに登録
		levelData_.players.push_back(data);
	}
	//敵キャラ発生ポイント
	else if (type.compare("EnemySpawn") == 0) {
		EnemySpawnData data;
		//トランスフォームパラメータの読み込み
		json& transform = object["transform"];
		//平行移動
		data.translation.x = (float)transform["translation"][0];
		data.translation.y = (float)transform["translation"][2];
		data.translation.z = (float)transform["translation"][1];
		//回転角
		data.rotation.x = -(float)transform["rotation"][0];
		data.rotation.y = -(float)transform["rotation"][2];
		data.rotation.z = -(float)transform["rotation"][1];
		//ファイル名を取得
		data.fileName = object["file_name"].get<std::string>();
		//敵キャラコンテナに登録
		levelData_.enemies.push_back(data);
	}

	//子オブジェクトがある場合はその処理も行う(ペアレント処理は割愛→必要に応じて付け加える)
	if (object.contains("children")) {
		for (json& childObject : object["children"]) {
			ScanObjectData(childObject);
		}
	}
}
