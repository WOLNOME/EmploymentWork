#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include "JsonUtil.h"
#include "interface/INode.h"
#include "blackBoard/BlackBoard.h"
#include <memory>

//前方宣言
class Player;
class EnemyWeaponManager;

/// <summary>
/// ボスクラス
/// </summary>
class Boss : public BaseCharacter {
public:
	/// ============================== ///
	///		メンバ関数（public）
	/// ============================== ///
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Boss() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Boss();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// デバッグ用パラメータ調整
	/// </summary>
	void DebugWithImGui() override;

	/// <summary>
	/// スポーン処理
	/// </summary>
	/// <param name="_position">座標</param>
	void Spawn(const Vector3& _position);


	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// プレイヤーのセット
	/// </summary>
	/// <param name="_player">プレイヤーのポインタ</param>
	void SetPlayer(Player* _player) { player_ = _player; }
	/// <summary>
	/// 敵武器マネージャーのセット
	/// </summary>
	/// <param name="_enemyWeaponManager">敵武器マネージャーのポインタ</param>
	void SetEnemyWeaponManager(EnemyWeaponManager* _enemyWeaponManager);

private:
	/// ============================== ///
	///		メンバ関数（private）
	/// ============================== ///

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="attribute">相手の属性</param>
	/// <param name="subjectPos">相手の座標</param>
	void OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) override;

	/// <summary>
	/// 定数情報のブラックボードへの登録
	/// </summary>
	void ConstantInfoToBlackBoard();
	
	/// <summary>
	/// 変数情報のブラックボードへの登録
	/// </summary>
	/// <param name="_isInit">初期化時か？</param>
	void VariableInfoToBlackBoard(bool _isInit);
	/// <summary>
	/// ブラックボードから変数情報の取得
	/// </summary>
	void BlackBoardToVariableInfo();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//プレイヤー
	Player* player_ = nullptr;
	//敵武器マネージャー
	EnemyWeaponManager* enemyWeaponManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数(private)
	/// ============================== ///
	
	//パラメーター
	json param_;

	//ビヘイビアツリー
	std::unique_ptr<INode> behaviorTreeRoot_ = nullptr;
	//ブラックボード
	std::unique_ptr<BlackBoard> blackBoard_ = nullptr;

	//HP
	int hp_ = 0;
	//生きているか
	bool isAlive_ = false;

};

