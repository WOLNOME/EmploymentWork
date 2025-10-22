#pragma once
#include "IJetEnemyState.h"
#include <Vector3.h>

class MessageUI;

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
	void SetMessageUI(MessageUI* messageUI) { messageUI_ = messageUI; }

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
	///		メンバ変数
	/// ============================== ///

	MessageUI* messageUI_ = nullptr;

	//目標点
	Vector3 targetPosition_ = {0.0f,0.0f,0.0f};

	//状態継続最低時間
	const float stateContinueTime_ = 2.0f;
	float stateContinueTimer_ = 0.0f;

};

