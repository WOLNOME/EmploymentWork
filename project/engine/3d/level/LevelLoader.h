#pragma once
#include <LevelObject.h>
#include <JsonUtil.h>
#include <list>
#include <string>
#include <memory>

namespace Norm {

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
			Vector3 translation; //位置
			Vector3 rotation; //回転角
		};
		/// <summary>
		/// 敵スポーンデータ
		/// </summary>
		struct EnemySpawnData {
			std::string fileName;	//ファイル名(敵の区別をするため)
			Vector3 translation;	//平行移動
			Vector3 rotation;		//回転角
		};

		/// <summary>
		/// レベルデータ
		/// </summary>
		struct LevelData {
			//オブジェクトのコンテナ
			std::list<std::unique_ptr<LevelObject>> objects;
			//自キャラコンテナ
			std::list<PlayerSpawnData> players;
			//敵キャラコンテナ
			std::list<EnemySpawnData> enemies;
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

}