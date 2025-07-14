#include "EnemyApproachState.h"

//アプリケーション
#include <application/object/character/enemy/base/IBaseEnemy.h>
#include <application/object/character/player/Player.h>

void EnemyApproachState::Enter(IBaseEnemy* enemy) {
}

void EnemyApproachState::Update(IBaseEnemy* enemy) {
	//死亡状態に切り替え
	TransitionDeadState(enemy);

	//攻撃範囲に入ったら攻撃状態に切り替え
	float searchPlayerDistanceAttack = enemy->GetParam()["searchPlayerDistanceAttack"];
	if (Vector3(enemy->GetPlayer()->GetWorldPosition() - enemy->GetWorldPosition()).Length() < searchPlayerDistanceAttack) {
		enemy->ChangeState("Attack");
	}

	//接近範囲から外れたらパトロール状態に切り替え
	float searchPlayerDistanceApproach = enemy->GetParam()["searchPlayerDistanceApproach"];
	if (Vector3(enemy->GetPlayer()->GetWorldPosition() - enemy->GetWorldPosition()).Length() > searchPlayerDistanceApproach) {
		enemy->ChangeState("Patrol");
	}

	//接近の更新処理
	UpdateApproach(enemy);
	//回転の更新処理
	UpdateRotate(enemy);

}

void EnemyApproachState::Exit(IBaseEnemy* enemy) {
}

void EnemyApproachState::UpdateApproach(IBaseEnemy* enemy) {
	
	//プレイヤーへの方向を求める
	Vector3 dirToPlayer = enemy->GetPlayer()->GetWorldTransform().translate - enemy->GetWorldTransform().translate;
	//y座標は考慮しない
	dirToPlayer.y = 0.0f;
	//正規化
	dirToPlayer.Normalize();

	//移動量を求める
	float speed = enemy->GetParam()["speed"];
	Vector3 currentVelocity = enemy->GetVelocity();
	currentVelocity += dirToPlayer * speed;

	//摩擦力をかける
	Vector3 frictionDir = -currentVelocity.Normalized();
	Vector3 frictionAccel = frictionDir * enemy->GetFloorFriction();
	currentVelocity += frictionAccel * kDeltaTime;

	//移動量の大きさを制限
	float maxSpeed = enemy->GetParam()["maxSpeed"];
	if (currentVelocity.Length() > maxSpeed) {
		currentVelocity.Normalize();
		currentVelocity *= maxSpeed;
	}
	//移動量の小ささを制限
	if (Vector3(currentVelocity * kDeltaTime).Length() < 0.01f) {
		currentVelocity = { 0.0f,0.0f,0.0f };
	}

	//速度を加算
	Vector3 currentTranslate = enemy->GetWorldTransform().translate;
	currentTranslate += currentVelocity * kDeltaTime;

	//速度と平行移動成分をセット
	enemy->SetVelocity(currentVelocity);
	enemy->SetTranslate(currentTranslate);
}
