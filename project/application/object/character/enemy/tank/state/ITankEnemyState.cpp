#include "ITankEnemyState.h"

//アプリケーション
#include <application/object/character/enemy/tank/base/IBaseTankEnemy.h>
#include <application/object/character/player/Player.h>

void ITankEnemyState::TransitionDeadState(IBaseTankEnemy* enemy) {
	if (enemy->GetHP() <= 0) {
		enemy->ChangeState("Dead");
	}
}

void ITankEnemyState::UpdateRotate(IBaseTankEnemy* enemy) {
	//移動方向に向かって回転->現在の向きを求める
	Vector3 currentDir = {
		std::sinf(enemy->GetWorldTransform().rotate.y),
		0.0f,
		std::cosf(enemy->GetWorldTransform().rotate.y)
	};
	currentDir.Normalize();
	//プレイヤーへの方向を求める
	Vector3 dirToPlayer = enemy->GetPlayer()->GetWorldTransform().translate - enemy->GetWorldTransform().translate;
	//目標の向きを求める
	Vector3 targetDir = dirToPlayer.Normalized();
	//回転の差を求める(float型)
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
	//-π ～ π に正規化
	if (currentRotate.y > pi) {
		currentRotate.y -= 2.0f * pi;
	}
	else if (currentRotate.y < -pi) {
		currentRotate.y += 2.0f * pi;
	}
	//結果をセット
	enemy->SetRotate(currentRotate);
}
