#include "JetEnemyPatrolState.h"
#include <Vector3.h>
#include <random>

//アプリケーション
#include <application/object/character/enemy/jet/base/IBaseJetEnemy.h>
#include <application/object/character/player/Player.h>

void JetEnemyPatrolState::Enter(IBaseJetEnemy* enemy) {
	//目標ポイントをランダムに決定
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(-300.0f, 300.0f);
	float height = enemy->GetParam()["height"];
	targetPosition_ = { dist(gen),height,dist(gen) };
}

void JetEnemyPatrolState::Update(IBaseJetEnemy* enemy) {
	//死亡状態に切り替え
	TransitionDeadState(enemy);

	//プレイヤーが近づいたら接近状態に切り替え
	float searchPlayerDistanceApproach = enemy->GetParam()["searchPlayerDistanceApproach"];
	Vector3 playerPos = enemy->GetPlayer()->GetWorldPosition();
	playerPos.y = enemy->GetParam()["height"];
	if (Vector3(playerPos - enemy->GetWorldPosition()).Length() < searchPlayerDistanceApproach) {
		enemy->ChangeState("Approach");
		//接近の通知をする

	}

	//パトロールの更新処理
	UpdatePatrol(enemy);
}

void JetEnemyPatrolState::Exit(IBaseJetEnemy* enemy) {
}

void JetEnemyPatrolState::UpdatePatrol(IBaseJetEnemy* enemy) {
	//目標ポイントとジェットの距離が近づいたら目標ポイントを更新
	float updateTargetPointDistance = enemy->GetParam()["updateTargetPointDistance"];
	if (Vector3(enemy->GetPlayer()->GetWorldPosition() - enemy->GetWorldPosition()).Length() < updateTargetPointDistance) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(-300.0f, 300.0f);
		float height = enemy->GetParam()["height"];
		targetPosition_ = { dist(gen),height,dist(gen) };
	}

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
