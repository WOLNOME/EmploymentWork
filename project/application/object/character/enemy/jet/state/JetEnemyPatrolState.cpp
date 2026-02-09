#include "JetEnemyPatrolState.h"
#include <Vector3.h>
#include <random>
#include <cassert>

//アプリケーション
#include <application/object/character/enemy/jet/base/IBaseJetEnemy.h>
#include <application/object/character/player/Player.h>
#include <application/ui/message/MessageUI.h>
#include <application/ui/enemy/EnemyUI.h>

using namespace Norm;

void JetEnemyPatrolState::Enter(IBaseJetEnemy* enemy) {
	//目標ポイントをランダムに決定
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(500.0f, 500.0f);
	float height = enemy->GetParam()["height"];
	targetPosition_ = { dist(gen),height,dist(gen) };
	stateContinueTimer_ = 0.0f;
	isDecidedTargetPoint_ = true;
}

void JetEnemyPatrolState::Update(IBaseJetEnemy* enemy) {
	//メッセージUIの確認
	assert(messageUI_ && "メッセージUIがセットされていません");

	//状態継続タイマーを更新
	stateContinueTimer_ += kDeltaTime;

	//死亡状態に切り替え
	TransitionDeadState(enemy);

	//プレイヤーが近づいたら接近状態に切り替え(状態継続最低時間を超えているときのみ)
	if (stateContinueTimer_ > kStateContinueTime_) {
		float searchPlayerDistanceApproach = enemy->GetParam()["searchPlayerDistanceApproach"];
		Vector3 playerPos = enemy->GetPlayer()->GetWorldTransform().GetTranslate();
		playerPos.y = enemy->GetParam()["height"];
		Vector3 enemyPos = enemy->GetWorldTransform().GetTranslate();
		float length = Vector3(playerPos - enemyPos).Length();
		if (length < searchPlayerDistanceApproach) {
			//接近に切り替え
			enemy->ChangeState("Approach");
			//接近の通知をする
			messageUI_->AddMessage(L"<注意> ジェットに捕捉された！");
			//発見UIを出す
			float height = enemy->GetParam()["uiHeight"];
			float front = enemy->GetParam()["uiFront"];
			enemyUI_->GetEnemyReactionUI()->SensingSpawn(enemy->GetWorldTransform().GetWorldTranslate(), height, front);

		}
	}

	//パトロールの更新処理
	UpdatePatrol(enemy);
}

void JetEnemyPatrolState::Exit(IBaseJetEnemy* enemy) {
}

void JetEnemyPatrolState::UpdatePatrol(IBaseJetEnemy* enemy) {
	//もし目標ポイントが決まっていなかったらランダムに決定
	if (!isDecidedTargetPoint_) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(-500.0f, 500.0f);
		float height = enemy->GetParam()["height"];
		targetPosition_ = { dist(gen),height,dist(gen) };
		isDecidedTargetPoint_ = true;
	}

	//目標ポイントとジェットの距離が近づいたら目標ポイントを更新
	float updateTargetPointDistance = enemy->GetParam()["updateTargetPointDistance"];
	if (Vector3(targetPosition_ - enemy->GetWorldTransform().GetTranslate()).Length() < updateTargetPointDistance) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(-500.0f, 500.0f);
		float height = enemy->GetParam()["height"];
		targetPosition_ = { dist(gen),height,dist(gen) };
	}

	//回転処理
	{
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
