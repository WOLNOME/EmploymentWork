#pragma once
#include "IJetEnemyState.h"
#include <Vector3.h>
#include <string>

class MessageUI;

/// <summary>
/// ジェットエネミーのパトロール状態を管理するクラス
/// </summary>
class JetEnemyPatrolState : public IJetEnemyState {
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
	/// メッセージUIを設定する
	/// </summary>
	/// <param name="messageUI">メッセージUIのポインタ</param>
	void SetMessageUI(MessageUI* messageUI) { messageUI_ = messageUI; }

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// パトロール処理の更新
	/// </summary>
	/// <param name="enemy"></param>
	void UpdatePatrol(IBaseJetEnemy* enemy);

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//メッセージUI
	MessageUI* messageUI_ = nullptr;

	//目標ポイント
	Norm::Vector3 targetPosition_ = {};
	bool isDecidedTargetPoint_ = false;

	//状態継続最低時間
	const float kStateContinueTime_ = 2.0f;
	float stateContinueTimer_ = 0.0f;

};

