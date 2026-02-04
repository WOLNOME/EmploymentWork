#include "JetEnemyApproachState.h"
#include <cassert>

//アプリケーション
#include <application/object/character/enemy/jet/base/IBaseJetEnemy.h>
#include <application/object/character/player/Player.h>
#include <application/ui/message/MessageUI.h>

using namespace Norm;

void JetEnemyApproachState::Enter(IBaseJetEnemy* enemy) {
	//目標点を更新(プレイヤーの座標)
	targetPosition_ = enemy->GetPlayer()->GetWorldTransform().GetTranslate();
	float height = enemy->GetParam()["height"];
	targetPosition_.y = height;
	stateContinueTimer_ = 0.0f;
}

void JetEnemyApproachState::Update(IBaseJetEnemy* enemy) {
	//メッセージUIの確認
	assert(messageUI_ && "メッセージUIがセットされていません");

	//状態継続タイマーを更新
	stateContinueTimer_ += kDeltaTime;

	//死亡状態に切り替え
	TransitionDeadState(enemy);
	//攻撃範囲に入ったら攻撃状態に切り替え
	float searchPlayerDistanceAttack = enemy->GetParam()["searchPlayerDistanceAttack"];
	if (Vector3(targetPosition_ - enemy->GetWorldTransform().GetTranslate()).Length() < searchPlayerDistanceAttack) {
		enemy->ChangeState("Attack");

		//攻撃範囲に入った通知をする
		messageUI_->AddMessage(L"<注意> ジェットの攻撃範囲に入った！");
	}
	//接近範囲から外れたらパトロール状態に切り替え(状態継続最低時間を超えている時のみ)
	if (stateContinueTimer_ > kStateContinueTime_) {
		float searchPlayerDistanceApproach = enemy->GetParam()["searchPlayerDistanceApproach"];
		if (Vector3(enemy->GetPlayer()->GetWorldTransform().GetTranslate() - enemy->GetWorldTransform().GetTranslate()).Length() > searchPlayerDistanceApproach) {
			enemy->ChangeState("Patrol");

			//接近範囲から外れた通知をする
			messageUI_->AddMessage(L"ジェットの追跡から逃れた！");
		}
	}
	//接近の更新処理
	UpdateApproach(enemy);
}

void JetEnemyApproachState::Exit(IBaseJetEnemy* enemy) {
}

void JetEnemyApproachState::UpdateApproach(IBaseJetEnemy* enemy) {
	//目標点を更新(プレイヤーの座標)
	targetPosition_ = enemy->GetPlayer()->GetWorldTransform().GetTranslate();
	float height = enemy->GetParam()["height"];
	targetPosition_.y = height;

	//回転処理
	{
		// 現在のY軸回転角から、XZ平面上の前方向ベクトルを算出
		Vector3 currentDir = {
			std::sinf(enemy->GetWorldTransform().GetRotate().y),
			0.0f,
			std::cosf(enemy->GetWorldTransform().GetRotate().y)
		};
		currentDir.Normalize();

		// 敵から目標ポイントへの方向ベクトル
		Vector3 targetDir = targetPosition_ - enemy->GetWorldTransform().GetTranslate();
		targetDir.Normalize();

		// 各方向をヨー角に変換し、その差分を回転角として求める
		float angle =
			std::atan2f(targetDir.x, targetDir.z) -
			std::atan2f(currentDir.x, currentDir.z);

		// 角度差を -π～π に正規化し、最短回転方向にする
		if (angle > pi) {
			angle -= 2.0f * pi;
		}
		else if (angle < -pi) {
			angle += 2.0f * pi;
		}

		// 角度差が小さい場合は直接合わせ、それ以外は一定速度で回転
		float rotateSpeed = enemy->GetParam()["rotateSpeed"];
		float usingRotateSpeed =
			(std::abs(angle) < rotateSpeed * kDeltaTime)
			? angle
			: ((angle > 0.0f) ? rotateSpeed * kDeltaTime : -rotateSpeed * kDeltaTime);

		// 回転を加算し、角度を -π～π に収める
		Vector3 currentRotate = enemy->GetWorldTransform().GetRotate();
		currentRotate.y += usingRotateSpeed;

		//-π～π に正規化
		if (currentRotate.y > pi) {
			currentRotate.y -= 2.0f * pi;
		}
		else if (currentRotate.y < -pi) {
			currentRotate.y += 2.0f * pi;
		}

		// 計算結果を送信
		enemy->SetRotate(currentRotate);
	}

	//移動処理
	{
		//現在の向きを求める
		Vector3 currentDir = {
			std::sinf(enemy->GetWorldTransform().GetRotate().y),
			0.0f,
			std::cosf(enemy->GetWorldTransform().GetRotate().y)
		};
		currentDir.Normalize();
		//スピードを求める
		float speed = enemy->GetParam()["speed"];
		//速度を求める
		Vector3 currentVelocity = currentDir * speed;

		//速度を加算
		Vector3 currentTranslate = enemy->GetWorldTransform().GetTranslate();
		currentTranslate.x += currentVelocity.x * kDeltaTime;
		currentTranslate.z += currentVelocity.z * kDeltaTime;

		//速度と平行移動成分をセット
		enemy->SetVelocity(currentVelocity);
		enemy->SetTranslate(currentTranslate);
	}


}
