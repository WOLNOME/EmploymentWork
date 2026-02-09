#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include "JsonUtil.h"
#include "interface/INode.h"
#include "blackBoard/BlackBoard.h"
#include <memory>

//アプリケーション
#include <application/object/character/enemy/boss/Barrier.h>

//前方宣言
class Player;
class EnemyWeaponManager;
class EnemyManager;
class EnemyUI;

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
	~Boss() override;
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
	/// <param name="_initPos">初期座標</param>
	/// <param name="_initRotate">初期回転</param>
	void Spawn(const Norm::Vector3& _initPos,const Norm::Vector3& _initRotate);


	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// パラメーターの取得
	/// </summary>
	/// <returns>パラメーター</returns>
	const json& GetParam() { return param_; }

	/// <summary>
	/// HPを取得
	/// </summary>
	/// <returns>HP</returns>
	int GetHP() { return hp_; }
	/// <summary>
	/// 最大HPを取得
	/// </summary>
	/// <returns>最大HP</returns>
	int GetMaxHP() { return maxHP_; }

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
	/// <summary>
	/// 敵マネージャーのセット
	/// </summary>
	/// <param name="_enemyManager">敵マネージャーのポインタ</param>
	void SetEnemyManager(EnemyManager* _enemyManager);
	/// <summary>
	/// 敵UIのセット
	/// </summary>
	/// <param name="_enemyUI">敵UIのポインタ</param>
	void SetEnemyUI(EnemyUI* _enemyUI);

	/// <summary>
	/// HPのセット
	/// </summary>
	/// <param name="_hp">HP</param>
	void SetHP(int _hp) { hp_ = _hp; }

private:
	/// ============================== ///
	///		メンバ関数（private）
	/// ============================== ///

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

	/// <summary>
	/// 変数の更新
	/// </summary>
	void VariableUpdate();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//プレイヤー
	Player* player_ = nullptr;
	//敵武器マネージャー
	EnemyWeaponManager* enemyWeaponManager_ = nullptr;
	//敵マネージャー
	EnemyManager* enemyManager_ = nullptr;
	//敵UI
	EnemyUI* enemyUI_ = nullptr;

	//バリア
	std::unique_ptr<Barrier> barrier_ = nullptr;

	/// ============================== ///
	///		メンバ変数(private)
	/// ============================== ///
	
	//パラメーター
	json param_;

	//ビヘイビアツリー
	std::unique_ptr<Norm::INode> behaviorTreeRoot_ = nullptr;
	//ブラックボード
	std::unique_ptr<Norm::BlackBoard> blackBoard_ = nullptr;

	//HP
	int hp_ = 0;
	int maxHP_ = 0;

};

