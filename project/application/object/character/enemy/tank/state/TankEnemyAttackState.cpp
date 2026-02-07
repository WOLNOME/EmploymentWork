#include "TankEnemyAttackState.h"
#include <cassert>

//アプリケーション
#include <application/object/character/enemy/tank/base/IBaseTankEnemy.h>
#include <application/object/character/player/Player.h>
#include <application/object/character/weapon/enemy/manager/EnemyWeaponManager.h>

using namespace Norm;

TankEnemyAttackState::TankEnemyAttackState() {
}

void TankEnemyAttackState::Enter(IBaseTankEnemy* enemy) {
}

void TankEnemyAttackState::Update(IBaseTankEnemy* enemy) {
	//死亡状態に切り替え
	TransitionDeadState(enemy);

	//攻撃範囲から外れたら接近状態に切り替え
	float searchPlayerDistanceAttack = enemy->GetParam()["searchPlayerDistanceAttack"];
	if (Vector3(enemy->GetPlayer()->GetWorldTransform().GetPreWorldTranslate() - enemy->GetWorldTransform().GetPreWorldTranslate()).Length() > searchPlayerDistanceAttack) {
		enemy->ChangeState("Approach");
	}

	//回転の更新処理
	UpdateRotate(enemy);
	//攻撃の更新処理
	UpdateAttack(enemy);

}

void TankEnemyAttackState::Exit(IBaseTankEnemy* enemy) {
}

void TankEnemyAttackState::UpdateAttack(IBaseTankEnemy* enemy) {
	//クールタイム処理
	if (coolTimer_ > 0.0f) {
		coolTimer_ -= kDeltaTime;
		//クールタイムがマイナスになったら0にする
		if (coolTimer_ < 0.0f) {
			coolTimer_ = 0.0f;
		}
		//クールタイム処理を終えたら関数を抜ける
		return;
	}

	//角度が一定以下出なければ弾を発射しない
	{
		//現在の向きを求める
		Vector3 currentDir = {
			std::sinf(enemy->GetWorldTransform().GetRotate().y),
			0.0f,
			std::cosf(enemy->GetWorldTransform().GetRotate().y)
		};
		currentDir.Normalize();
		//目標ポイントへの方向を求める
		Vector3 targetDir = enemy->GetPlayer()->GetWorldTransform().GetTranslate() - enemy->GetWorldTransform().GetTranslate();
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
		//10度より大きかったら発射しない
		if (std::abs(angle) > (1.0f / 18.0f) * pi) {
			return;
		}
	}

	//攻撃処理
	if (coolTimer_ == 0.0f) {
		//クールタイムをセット
		float cannonCoolTime = enemy->GetParam()["cannonCoolTime"];
		coolTimer_ = cannonCoolTime;
		//初期位置を計算
		Vector3 initPos = enemy->GetWorldTransform().GetTranslate();
		std::string tag = enemy->GetParam()["tag"];
		if (tag == "canota") {
			initPos.y += 1.5f;
			initPos.x += std::sinf(enemy->GetWorldTransform().GetRotate().y) * 7.0f;
			initPos.z += std::cosf(enemy->GetWorldTransform().GetRotate().y) * 7.0f;
		}
		else if (tag == "keyCanota") {
			initPos.y += 2.0f;	//←高さ
			initPos.x += std::sinf(enemy->GetWorldTransform().GetRotate().y) * 10.0f;
			initPos.z += std::cosf(enemy->GetWorldTransform().GetRotate().y) * 10.0f;
		}
		else {
			//そのタグは存在しない
			assert(false && "そのようなタグは存在しません");
		}
		//スポーン
		enemy->GetEnemyWeaponManager()->SpawnCannon(initPos, enemy->GetPlayer()->GetWorldTransform().GetTranslate());
	}

}
