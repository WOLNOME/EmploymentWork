#pragma once
#include <JsonUtil.h>
#include <Vector3.h>
#include <list>
#include <string>
#include <memory>

//アプリケーション
#include <application/object/level/tree/LevelTree.h>
#include <application/object/level/bigTree/LevelBigTree.h>
#include <application/object/level/rock/LevelRock.h>
#include <application/object/level/bigRock/LevelBigRock.h>
#include <application/object/level/fence/LevelFence.h>
#include <application/object/level/tutorial_fence/LevelTutorialFence.h>
#include <application/object/level/sealedBox/LevelSealedBox.h>
#include <application/object/level/brokenTank/LevelBrokenTank.h>

/// <summary>
/// レベルデータの読み込みと管理を行うクラス
/// </summary>
class LevelLoader {
private:
	/// ============================== ///
	///		構造体
	/// ============================== ///

	/// <summary>
	/// プレイヤースポーンデータ
	/// </summary>
	struct PlayerSpawnData {
		Norm::Vector3 translation; //位置
		Norm::Vector3 rotation; //回転角
	};
	/// <summary>
	/// 敵スポーンデータ
	/// </summary>
	struct EnemySpawnData {
		std::string type;			//タイプ(敵の区別をするため)
		Norm::Vector3 translation;	//平行移動
		Norm::Vector3 rotation;		//回転角
	};

	/// <summary>
	/// レベルデータ
	/// </summary>
	struct LevelData {
		//自キャラコンテナ
		std::list<PlayerSpawnData> players;
		//敵キャラコンテナ
		std::list<EnemySpawnData> enemies;
		//ツリー
		std::unique_ptr<LevelTree> tree;
		//巨大ツリー
		std::unique_ptr<LevelBigTree> bigTree;
		//岩
		std::unique_ptr<LevelRock> rock;
		//巨大岩
		std::unique_ptr<LevelBigRock> bigRock;
		//柵
		std::unique_ptr<LevelFence> fence;
		//チュートリアル柵
		std::unique_ptr<LevelTutorialFence> tutorialFence;
		//封印ボックス
		std::unique_ptr<LevelSealedBox> sealedBox;
		//破壊された戦車
		std::unique_ptr<LevelBrokenTank> brokenTank;

	};

public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_filePath">レベルデータのファイルパス</param>
	void Initialize(const std::string& _filePath);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// ImGui操作
	/// </summary>
	void DebugWithImGui();

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// プレイヤースポーンデータの取得
	/// </summary>
	/// <returns>プレイヤースポーンデータ</returns>
	const std::list<PlayerSpawnData>& GetPlayerSpawnData() const { return levelData_.players; }
	/// <summary>
	/// 敵スポーンデータの取得
	/// </summary>
	/// <returns>敵スポーンデータ</returns>
	const std::list<EnemySpawnData>& GetEnemySpawnData() const { return levelData_.enemies; }
	/// <summary>
	/// ツリーデータの取得
	/// </summary>
	LevelTree* GetTreeData() const { return levelData_.tree.get(); }
	/// <summary>
	/// 巨大ツリーデータの取得
	/// </summary>
	LevelBigTree* GetBigTreeData() const { return levelData_.bigTree.get(); }
	/// <summary>
	/// 岩データの取得
	/// </summary>
	LevelRock* GetRockData() const { return levelData_.rock.get(); }
	/// <summary>
	/// 巨大岩データの取得
	/// </summary>
	LevelBigRock* GetBigRockData() const { return levelData_.bigRock.get(); }
	/// <summary>
	/// 柵データの取得
	/// </summary>
	LevelFence* GetFenceData() const { return levelData_.fence.get(); }
	/// <summary>
	/// 柵データの取得
	/// </summary>
	LevelTutorialFence* GetTurorialFenceData() const { return levelData_.tutorialFence.get(); }
	/// <summary>
	/// 封印ボックスデータの取得
	/// </summary>
	LevelSealedBox* GetSealedBoxData() const { return levelData_.sealedBox.get(); }
	/// <summary>
	/// 破壊された戦車データの取得
	/// </summary>
	LevelBrokenTank* GetBrokenTankData() const { return levelData_.brokenTank.get(); }

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// レベルデータの読み込み
	/// </summary>
	/// <param name="object">オブジェクトデータの入っているjson型変数</param>
	void ScanObjectData(json& object);

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//レベルデータ
	LevelData levelData_;

};
