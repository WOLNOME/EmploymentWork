#pragma once
#include <array>
#include <string>
#include <Sprite.h>
#include <memory>
#include <Handle.h>

//前方宣言
class Player;
class PlayerWeaponManager;

/// <summary>
/// 目標の種類
/// </summary>
enum class ObjectiveType {
	None,			//未定
	CollectItem,	//アイテムの収集
	CollectSpecial,	//必殺弾の収集
	HitCannon,		//砲弾を当てる
	HitBullet,		//銃弾を当てる
	DefeatEnemy,	//敵の撃破
};

/// <summary>
/// 目標情報の構造体
/// </summary>
struct Objective {
	ObjectiveType type;
	std::unique_ptr<Norm::Sprite> checkMark;
	std::unique_ptr<Norm::Sprite> text;
	Norm::Handle textHandle;
	int current = 0;
	int targetCount = 0;
	bool completed = false;
};

/// <summary>
/// 目標システム
/// </summary>
class ObjectiveSystem {
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
	/// 全ての目標が達成されているかを確認
	/// </summary>
	/// <returns></returns>
	bool CheckAllObjectiveComplete();

	/// <summary>
	/// 目標のクリア（削除）
	/// </summary>
	void ClearObjective();

	/// <summary>
	/// 収集目標の追加
	/// </summary>
	/// <param name="type">種類</param>
	/// <param name="targetCount">目標数</param>
	void AddCollectObjective(const std::string& type, int targetCount);
	/// <summary>
	/// 攻撃を当てる目標の追加
	/// </summary>
	/// <param name="type">種類</param>
	/// <param name="targetCount">目標数</param>
	void AddHitObjective(const std::string& type, int targetCount);

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// プレイヤーの設定
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void SetPlayer(Player* player) { player_ = player; }
	/// <summary>
	/// プレイヤー武器マネージャーの設定
	/// </summary>
	/// <param name="manager">プレイヤー武器マネージャー</param>
	void SetPlayerWeaponManager(PlayerWeaponManager* manager) { playerWeaponManager_ = manager; }

private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	Player* player_ = nullptr;
	PlayerWeaponManager* playerWeaponManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	std::array<Objective, 5> objectives_;
};
