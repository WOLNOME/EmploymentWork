#include "EnemyCannonCollider.h"

//アプリケーション
#include <application/object/character/weapon/enemy/EnemyCannon.h>
#include <application/ui/player/PlayerUI.h>

using namespace Norm;

EnemyCannonCollider::EnemyCannonCollider(EnemyCannon* _holder) : SphereColliderBase(), holder_(_holder) {
}

void EnemyCannonCollider::Debug() {
#ifdef _DEBUG
	//基底クラスのデバッグ処理
	SphereColliderBase::Debug();

	//debugLineColorを元に戻す
	debugLineColor_ = { 1,1,1,1 };

#endif // _DEBUG
}

void EnemyCannonCollider::OnCollision(ICollider* _other, CollisionAttribute _attribute) {
	//衝突時の共通処理ラムダ式
	auto commonCollisionProcess = [this]() {
		//デバッグ用ラインのカラーを赤にする
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//パーティクルの発生
		TransformEuler transform = holder_->GetExplosionParticle()->GetBaseTransform();
		transform.translate = GetWorldTransform().GetTranslate();
		holder_->GetExplosionParticle()->SetBaseTransform(transform);
		holder_->GetExplosionParticle()->SetIsPlay(true);
		//仮死状態にする
		holder_->SetState(BaseCharacter::State::kAsphyxia);
		};

	//当たり判定時の処理
	switch (_attribute) {
		//プレイヤーに当たった場合
	case CollisionAttribute::Player:
		//共通処理
		commonCollisionProcess();

		//被弾インジケーターをつける
		holder_->GetPlayerUI()->GetHitIndicator()->RegistIndicator(holder_->GetGeneratedPosition());

		break;
		//プレイヤー弾に当たった場合
	case CollisionAttribute::PlayerBullet:
		//共通処理
		commonCollisionProcess();

		//当たり判定属性をなしに
		SetCollisionAttribute(CollisionAttribute::Nothingness);

		break;
		//プレイヤーキャノンに当たった場合
	case CollisionAttribute::PlayerCannon:
		//共通処理
		commonCollisionProcess();

		//当たり判定属性をなしに
		SetCollisionAttribute(CollisionAttribute::Nothingness);

		break;
	default:
		break;
	}
}
