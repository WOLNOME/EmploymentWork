#pragma once
#include <array>
#include <string>
#include <Sprite.h>
#include <memory>
#include <Handle.h>

//前方宣言
class Player;

/// <summary>
/// 目標の種類
/// </summary>
enum class ObjectiveType {
	None,			//未定
	CollectItem		//アイテムの収集
};

/// <summary>
/// 目標情報の構造体
/// </summary>
struct Objective {
	ObjectiveType type;
	std::unique_ptr<Norm::Sprite> chcekMark;
	std::unique_ptr<Norm::Sprite> text;
	Handle textHandle;
	int current = 0;
	int target = 0;
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
	/// <param name="target"></param>
	void AddCollectObjective(int target);

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// プレイヤーの設定
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void SetPlayer(Player* player) { player_ = player; }

private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	Player* player_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	std::array<Objective, 5> objectives_;
};
