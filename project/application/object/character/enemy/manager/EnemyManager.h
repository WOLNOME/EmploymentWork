#pragma once
#include <list>
#include <memory>

//アプリケーション
#include "application/object/character/enemy/tank/Canota.h"
#include "application/object/character/enemy/tank/Boss.h"
#include "application/object/character/enemy/jet/Jet.h"

class LevelLoader;
class Player;
class ItemManager;
class MessageUI;
class EnemyManager {
public:
	//初期化
	void Initialize();
	//更新
	void Update();

	//デバッグ用パラメーター調整
	void DebugWithImGui();

public://getter
	//キャノ太のコンテナ
	const std::list<std::unique_ptr<Canota>>& GetCanotas() const { return canotas_; }
	//ボスのコンテナ
	const std::list<std::unique_ptr<Boss>>& GetBosses() const { return bosses_; }
	//ジェットのコンテナ
	const std::list<std::unique_ptr<Jet>>& GetJets() const { return jets_; }

public://setter
	//レベルローダーセット
	void SetLevelLoader(LevelLoader* _levelLoader);
	//プレイヤーセット
	void SetPlayer(Player* _player);
	//アイテムマネージャーセット
	void SetItemManager(ItemManager* _itemManager);
	//メッセージUIセット
	void SetMessageUI(MessageUI* _messageUI);

private:
	//プレイヤー
	Player* player_ = nullptr;
	//アイテムマネージャー
	ItemManager* itemManager_ = nullptr;

private:
	//キャノ太のコンテナ
	std::list<std::unique_ptr<Canota>> canotas_;
	//ボスのコンテナ
	std::list<std::unique_ptr<Boss>> bosses_;
	//ジェットのコンテナ
	std::list<std::unique_ptr<Jet>> jets_;


};

