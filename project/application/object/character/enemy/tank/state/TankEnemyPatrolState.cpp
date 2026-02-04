#include "TankEnemyPatrolState.h"
#include <Vector3.h>
#include <random>

//アプリケーション
#include <application/object/character/enemy/tank/base/IBaseTankEnemy.h>
#include <application/object/character/player/Player.h>

using namespace Norm;

void TankEnemyPatrolState::Enter(IBaseTankEnemy* enemy) {
	//メンバ変数のリセット
	isRotation_ = false;
	isMoving_ = false;
}

void TankEnemyPatrolState::Update(IBaseTankEnemy* enemy) {
	//死亡状態に切り替え
	TransitionDeadState(enemy);

	//プレイヤーが近づいたら接近状態に切り替え
	float searchPlayerDistanceApproach = enemy->GetParam()["searchPlayerDistanceApproach"];
	if (Vector3(enemy->GetPlayer()->GetWorldPosition() - enemy->GetWorldPosition()).Length() < searchPlayerDistanceApproach) {
		enemy->ChangeState("Approach");
	}

	//パトロールの更新処理
	UpdatePatrol(enemy);
}

void TankEnemyPatrolState::Exit(IBaseTankEnemy* enemy) {
}

void TankEnemyPatrolState::UpdatePatrol(IBaseTankEnemy* enemy) {
	//回転も移動もしていなければ目標ポイントを決める
	if (!isRotation_ && !isMoving_) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(-300.0f, 300.0f);

		targetPosition_ = { dist(gen),0.0f,dist(gen) };
		//現在の速度を0にする
		enemy->SetVelocity({ 0.0f,0.0f,0.0f });

		isRotation_ = true;
	}

	//回転処理
	if (isRotation_) {
		//現在の向きを求める
		Vector3 currentDir = {
			std::sinf(enemy->GetWorldTransform().GetRotate().y),
			0.0f,
			std::cosf(enemy->GetWorldTransform().GetRotate().y)
		};
		currentDir.Normalize();
		//目標ポイントへの方向を求める
		Vector3 targetDir = targetPosition_ - enemy->GetWorldTransform().GetTranslate();
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
			//回転処理を終えて移動処理に移行
			isRotation_ = false;
			isMoving_ = true;
		}
		else {
			//回転スピードを使う場合、符号を揃える
			usingRotateSpeed = (angle > 0) ? rotateSpeed * kDeltaTime : -rotateSpeed * kDeltaTime;
		}
		//現在の回転取得
		Vector3 currentRotate = enemy->GetWorldTransform().GetRotate();
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
	if (isMoving_) {
		//スピードを求める
		float speed = enemy->GetParam()["speed"];
		speed *= 1.5f;
		//目標までのベクトルを求める
		Vector3 toTargetVec = targetPosition_ - enemy->GetWorldTransform().GetTranslate();
		if (toTargetVec.Length() < 10.0f) {
			//移動処理を終えて新たな目標地点を見つける
			isMoving_ = false;
		}
		//速度を求める
		Vector3 currentVelocity = {};
		currentVelocity = toTargetVec.Normalized() * speed;

		//速度を加算
		Vector3 currentTranslate = enemy->GetWorldTransform().GetTranslate();
		currentTranslate.x += currentVelocity.x * kDeltaTime;
		currentTranslate.z += currentVelocity.z * kDeltaTime;

		//速度と平行移動成分をセット
		enemy->SetVelocity(currentVelocity);
		enemy->SetTranslate(currentTranslate);
	}
}
