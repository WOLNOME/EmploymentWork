#pragma once
#include "IJetEnemyState.h"
#include <Vector3.h>

//前方宣言
class MessageUI;
class EnemyUI;

/// <summary>
/// ジェットエネミーの接近状態を管理するクラス
/// </summary>
class JetEnemyApproachState : public IJetEnemyState {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// 切り替え時の最初の処理
	/// </summary>
	/// <param name="enemy">ジェットエネミーのポインタ</param>
	void Enter(IBaseJetEnemy* enemy) override;
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="enemy">ジェットエネミーのポインタ</param>
	void Update(IBaseJetEnemy* enemy) override;
	/// <summary>
	/// 切り替え時の最後の処理
	/// </summary>
	/// <param name="enemy">ジェットエネミーのポインタ</param>
	void Exit(IBaseJetEnemy* enemy) override;

	/// ============================== ///
	///		setter
	/// ============================== ///
	
	/// <summary>
	/// メッセージUIのセット
	/// </summary>
	/// <param name="messageUI">メッセージUIのポインタ</param>
	void SetMessageUI(MessageUI* _messageUI) { messageUI_ = _messageUI; }
	/// <summary>
	/// 敵UIのセット
	/// </summary>
	/// <param name="_enemyUI">敵UIのポインタ</param>
	void SetEnemyUI(EnemyUI* _enemyUI) { enemyUI_ = _enemyUI; }

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// 接近の更新処理
	/// </summary>
	/// <param name="enemy">ジェットエネミーのポインタ</param>
	void UpdateApproach(IBaseJetEnemy* enemy);

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	EnemyUI* enemyUI_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	MessageUI* messageUI_ = nullptr;

	//目標点
	Norm::Vector3 targetPosition_ = {0.0f,0.0f,0.0f};

	//状態継続最低時間
	const float kStateContinueTime_ = 2.0f;
	float stateContinueTimer_ = 0.0f;

};

