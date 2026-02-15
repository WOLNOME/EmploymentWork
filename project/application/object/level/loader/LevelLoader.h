#pragma once
#include <JsonUtil.h>
#include <Vector3.h>
#include <list>
#include <string>
#include <memory>

//前方宣言
class LevelTree;

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
		std::string fileName;	//ファイル名(敵の区別をするため)
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
		//std::unique_ptr<LevelBigTree> tree;
		//岩
		//std::unique_ptr<LevelRock> tree;
		//柵
		//std::unique_ptr<LevelFence> tree;
		//封印ボックス
		//std::unique_ptr<LevelSealedBox> tree;
	};

public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
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
	/// <returns>ツリーデータ</returns>
	const std::unique_ptr<LevelTree>& GetTreeData() const { return levelData_.tree; }

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
