#pragma once
#include "JsonUtil.h"
#include <vector>
#include <memory>

//アプリケーション
#include "application/object/character/enemy/tank/Canota.h"
#include "application/object/character/enemy/tank/KeyCanota.h"
#include "application/object/character/enemy/jet/Jet.h"
#include "application/object/character/enemy/boss/Boss.h"

class LevelLoader;
class Player;
class ItemManager;
class MessageUI;

/// <summary>
/// エネミー全体の管理を行うクラス
/// </summary>
class EnemyManager {
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
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui();

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// キャノ太のコンテナ取得
	/// </summary>
	/// <returns>キャノ太のコンテナ</returns>
	const std::vector<std::unique_ptr<Canota>>& GetCanotas() const { return canotas_; }
	/// <summary>
	/// キーキャノ太のコンテナ取得
	/// </summary>
	/// <returns>キーキャノ太のコンテナ</returns>
	const std::vector<std::unique_ptr<KeyCanota>>& GetKeyCanotas() const { return keyCanotas_; }
	/// <summary>
	/// ジェットのコンテナ取得
	/// </summary>
	/// <returns>ジェットのコンテナ</returns>
	const std::vector<std::unique_ptr<Jet>>& GetJets() const { return jets_; }
	/// <summary>
	/// ボスのコンテナ取得
	/// </summary>
	/// <returns></returns>
	const std::unique_ptr<Boss>& GetBoss() const { return boss_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// レベルローダーのセット
	/// </summary>
	/// <param name="_levelLoader">レベルローダーのポインタ</param>
	void SetLevelLoader(LevelLoader* _levelLoader);
	/// <summary>
	/// プレイヤーのセット
	/// </summary>
	/// <param name="_player">プレイヤーのポインタ</param>
	void SetPlayer(Player* _player);
	/// <summary>
	/// アイテムマネージャーのセット
	/// </summary>
	/// <param name="_itemManager">アイテムマネージャーのポインタ</param>
	void SetItemManager(ItemManager* _itemManager);
	/// <summary>
	/// メッセージUIのセット
	/// </summary>
	/// <param name="_messageUI">メッセージUIのポインタ</param>
	void SetMessageUI(MessageUI* _messageUI);

private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//プレイヤー
	Player* player_ = nullptr;
	//アイテムマネージャー
	ItemManager* itemManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///
	
	//パラメーター
	json param_;

	//キャノ太のコンテナ
	std::vector<std::unique_ptr<Canota>> canotas_;
	//キーキャノ太のコンテナ
	std::vector<std::unique_ptr<KeyCanota>> keyCanotas_;
	//ジェットのコンテナ
	std::vector<std::unique_ptr<Jet>> jets_;
	//ボスのインスタンス
	std::unique_ptr<Boss> boss_ = nullptr;

};

