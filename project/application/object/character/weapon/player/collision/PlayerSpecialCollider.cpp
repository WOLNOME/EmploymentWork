#include "PlayerSpecialCollider.h"

//アプリケーション
#include <application/object/character/weapon/player/PlayerSpecial.h>

using namespace Norm;

PlayerSpecialCollider::PlayerSpecialCollider(PlayerSpecial* _holder) : SphereColliderBase(), holder_(_holder) {
}

void PlayerSpecialCollider::Debug() {
#ifdef _DEBUG
	//基底クラスのデバッグ処理
	SphereColliderBase::Debug();

	//debugLineColorを元に戻す
	debugLineColor_ = { 1,1,1,1 };

#endif // _DEBUG
}

void PlayerSpecialCollider::OnCollision(ICollider* _other, CollisionAttribute _attribute) {
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
		//敵に当たった場合
	case CollisionAttribute::Enemy:
		//共通処理
		commonCollisionProcess();

		break;
		//バリアに当たった場合
	case CollisionAttribute::Barrier:
		//共通処理
		commonCollisionProcess();

		break;
	default:
		break;
	}
}
