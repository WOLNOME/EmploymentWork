#include "LevelLoader.h"
#include <assert.h>

using namespace Norm;

void LevelLoader::Initialize(const std::string& _filePath) {
	//jsonデータを読み込む
	json data = JsonUtil::GetJsonData(_filePath);

	//各オブジェクトの生成
	levelData_.tree = std::make_unique<LevelTree>();
	levelData_.bigTree = std::make_unique<LevelBigTree>();
	levelData_.rock = std::make_unique<LevelRock>();
	levelData_.bigRock = std::make_unique<LevelBigRock>();
	levelData_.fence = std::make_unique<LevelFence>();
	levelData_.tutorialFence = std::make_unique<LevelTutorialFence>();
	levelData_.sealedBox = std::make_unique<LevelSealedBox>();
	levelData_.brokenTank = std::make_unique<LevelBrokenTank>();

	//各オブジェクトの初期化
	levelData_.tree->Initialize("LevelTree");
	levelData_.bigTree->Initialize("LevelBigTree");
	levelData_.rock->Initialize("LevelRock");
	levelData_.bigRock->Initialize("LevelBigRock");
	levelData_.fence->Initialize("LevelFence");
	levelData_.tutorialFence->Initialize("LevelTutorialFence");
	levelData_.sealedBox->Initialize("LevelSealedBox");
	levelData_.brokenTank->Initialize("LevelBrokenTank");

	//全オブジェクトデータのロード
	for (json& object : data["objects"]) {
		ScanObjectData(object);
	}
}

void LevelLoader::Update() {
	//オブジェクトの更新
	levelData_.tree->Update();
	levelData_.bigTree->Update();
	levelData_.rock->Update();
	levelData_.bigRock->Update();
	levelData_.fence->Update();
	levelData_.tutorialFence->Update();
	levelData_.sealedBox->Update();
	levelData_.brokenTank->Update();
}

void LevelLoader::DebugWithImGui() {
#ifdef _DEBUG
	//全オブジェクトのデバッグ処理
	levelData_.tree->DebugWithImGui();
	levelData_.bigTree->DebugWithImGui();
	levelData_.rock->DebugWithImGui();
	levelData_.bigRock->DebugWithImGui();
	levelData_.fence->DebugWithImGui();
	levelData_.tutorialFence->DebugWithImGui();
	levelData_.sealedBox->DebugWithImGui();
	levelData_.brokenTank->DebugWithImGui();
#endif // _DEBUG

}

void LevelLoader::ScanObjectData(json& object) {
	//ラムダ式でエネミー生成
	auto createEnemy = [&](const std::string& type) {
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
		data.type = type;
		//敵キャラコンテナに登録
		levelData_.enemies.push_back(data);

		};
	//ラムダ式でオブジェクト生成
	auto createObject = [&](const std::string& type) {
		//トランスフォームパラメータの読み込み
		TransformEuler transform = {};
		//平行移動
		transform.translate.x = (float)object["transform"]["translation"][0];
		transform.translate.y = (float)object["transform"]["translation"][2];
		transform.translate.z = (float)object["transform"]["translation"][1];
		//回転角
		transform.rotate.x = -(float)object["transform"]["rotation"][0] / 180.0f * pi;
		transform.rotate.y = -(float)object["transform"]["rotation"][2] / 180.0f * pi;
		transform.rotate.z = -(float)object["transform"]["rotation"][1] / 180.0f * pi;
		//スケーリング
		transform.scale.x = (float)object["transform"]["scaling"][0];
		transform.scale.y = (float)object["transform"]["scaling"][2];
		transform.scale.z = (float)object["transform"]["scaling"][1];
		//コライダーのパラメータの読み込み
		bool isCollider = false;
		Vector3 center = {};
		Vector3 size = {};
		if (object.contains("collider")) {
			isCollider = true;
			//コリジョン情報の読み込み
			json& collision = object["collider"];
			center = { (float)collision["center"][0], (float)collision["center"][2], (float)collision["center"][1] };
			size = { (float)collision["size"][0], (float)collision["size"][2], (float)collision["size"][1] };
		}

		//オブジェクトテーブルの作成
		std::map<std::string, IBaseLevelObject*> objectTable = {
			{"TreeObject", levelData_.tree.get()},
			{"BigTreeObject", levelData_.bigTree.get()},
			{"RockObject", levelData_.rock.get()},
			{"BigRockObject", levelData_.bigRock.get()},
			{"FenceObject", levelData_.fence.get()},
			{"Tutorial_FenceObject", levelData_.tutorialFence.get()},
			{"SealedBoxObject", levelData_.sealedBox.get()},
			{"BrokenTankObject", levelData_.brokenTank.get() }
		};

		//タイプからテーブルのキーを検索
		auto it = objectTable.find(type);
		//該当のテーブルで処理を行う
		if (it != objectTable.end()) {
			auto* object = it->second;
			//トランスフォームの設定
			uint32_t handle = object->SetTransformInfo(transform);
			//コライダーの設定
			if (isCollider) {
				object->SetCollisionInfo(handle, center, size);
			}
		}

		};


	//"type"データがない場合不正データのため警告
	assert(object.contains("type"));

	//種別を取得
	std::string type = object["type"].get<std::string>();

	//自キャラ発生ポイント
	if (type.compare("PlayerSpawn") == 0) {
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
	//キャノ太発生ポイント
	else if (type.compare("CanotaSpawn") == 0) {
		createEnemy("Canota");
	}
	//キーキャノ太発生ポイント
	else if (type.compare("KeyCanotaSpawn") == 0) {
		createEnemy("KeyCanota");
	}
	//ジェット発生ポイント
	else if (type.compare("JetSpawn") == 0) {
		createEnemy("Jet");
	}
	//ツリー生成ポイント
	else if (type.compare("TreeObject") == 0) {
		createObject("TreeObject");
	}
	//巨大ツリー生成ポイント
	else if (type.compare("BigTreeObject") == 0) {
		createObject("BigTreeObject");
	}
	//岩生成ポイント
	else if (type.compare("RockObject") == 0) {
		createObject("RockObject");
	}
	//巨大岩生成ポイント
	else if (type.compare("BigRockObject") == 0) {
		createObject("BigRockObject");
	}
	//柵生成ポイント
	else if (type.compare("FenceObject") == 0) {
		createObject("FenceObject");
	}
	//チュートリアル柵生成ポイント
	else if (type.compare("Tutorial_FenceObject") == 0) {
		createObject("Tutorial_FenceObject");
	}
	//封印ボックス生成ポイント
	else if (type.compare("SealedBoxObject") == 0) {
		createObject("SealedBoxObject");
	}
	//破壊された戦車生成ポイント
	else if (type.compare("BrokenTankObject") == 0) {
		createObject("BrokenTankObject");
		levelData_.brokenTank->InitParticle();
	}

	//子オブジェクトがある場合はその処理も行う(ペアレント処理は割愛→必要に応じて付け加える)
	if (object.contains("children")) {
		for (json& childObject : object["children"]) {
			ScanObjectData(childObject);
		}
	}
}
