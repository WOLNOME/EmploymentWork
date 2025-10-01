#include "JetEnemyApproachState.h"
#include <cassert>

//アプリケーション
#include <application/object/character/enemy/jet/base/IBaseJetEnemy.h>
#include <application/object/character/player/Player.h>
#include <application/ui/message/MessageUI.h>

void JetEnemyApproachState::Enter(IBaseJetEnemy* enemy) {
	//目標点を更新(プレイヤーの座標)
	targetPosition_ = enemy->GetPlayer()->GetWorldTransform().translate;
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
	if (Vector3(targetPosition_ - enemy->GetWorldTransform().translate).Length() < searchPlayerDistanceAttack) {
		enemy->ChangeState("Attack");

		//攻撃範囲に入った通知をする
		messageUI_->AddMessage(L"<注意> ジェットの攻撃範囲に入った！");
	}
	//接近範囲から外れたらパトロール状態に切り替え(状態継続最低時間を超えている時のみ)
	if (stateContinueTimer_ > stateContinueTime_) {
		float searchPlayerDistanceApproach = enemy->GetParam()["searchPlayerDistanceApproach"];
		if (Vector3(enemy->GetPlayer()->GetWorldTransform().translate - enemy->GetWorldTransform().translate).Length() > searchPlayerDistanceApproach) {
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
	targetPosition_ = enemy->GetPlayer()->GetWorldTransform().translate;
	float height = enemy->GetParam()["height"];
	targetPosition_.y = height;

	//回転処理
	{
		//現在の向きを求める
		Vector3 currentDir = {
			std::sinf(enemy->GetWorldTransform().rotate.y),
			0.0f,
			std::cosf(enemy->GetWorldTransform().rotate.y)
		};
		currentDir.Normalize();
		//目標ポイントへの方向を求める
		Vector3 targetDir = targetPosition_ - enemy->GetWorldTransform().translate;
		targetDir.Normalize();
		//回転の差を求める
		float angle = std::atan2f(targetDir.x, targetDir.z) - std::atan2f(currentDir.x, currentDir.z);
		//angleを-pi~piでクランプする
		if (angle > pi) {
			angle -= 2 * pi;
		}
		else if (angle < -pi) {
			angle += 2 * pi;
		}
		//回転スピードを決める
		float usingRotateSpeed;
		float rotateSpeed = enemy->GetParam()["rotateSpeed"];
		if (std::abs(angle) < rotateSpeed * kDeltaTime) {
			//仕上げの角度加算
			usingRotateSpeed = angle;
		}
		else {
			//回転スピードを使う場合、符号を揃える
			usingRotateSpeed = (angle > 0) ? rotateSpeed * kDeltaTime : -rotateSpeed * kDeltaTime;
		}
		//現在の回転取得
		Vector3 currentRotate = enemy->GetWorldTransform().rotate;
		//回転加算
		currentRotate.y += usingRotateSpeed;
		//-π~πにクランプ
		if (currentRotate.y > pi) {
			currentRotate.y -= 2.0f * pi;
		}
		else if (currentRotate.y < -pi) {
			currentRotate.y += 2.0f * pi;
		}
		//結果をセット
		enemy->SetRotate(currentRotate);
	}

	//移動処理
	{
		//現在の向きを求める
		Vector3 currentDir = {
			std::sinf(enemy->GetWorldTransform().rotate.y),
			0.0f,
			std::cosf(enemy->GetWorldTransform().rotate.y)
		};
		currentDir.Normalize();
		//スピードを求める
		float speed = enemy->GetParam()["speed"];
		//速度を求める
		Vector3 currentVelocity = currentDir * speed;

		//速度を加算
		Vector3 currentTranslate = enemy->GetWorldTransform().translate;
		currentTranslate.x += currentVelocity.x * kDeltaTime;
		currentTranslate.z += currentVelocity.z * kDeltaTime;

		//速度と平行移動成分をセット
		enemy->SetVelocity(currentVelocity);
		enemy->SetTranslate(currentTranslate);
	}


}
