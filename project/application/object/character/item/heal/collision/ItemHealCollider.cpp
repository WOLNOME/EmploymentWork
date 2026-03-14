#include "ItemHealCollider.h"

//アプリケーション
#include <application/object/character/item/heal/ItemHeal.h>

using namespace Norm;

ItemHealCollider::ItemHealCollider(ItemHeal* _holder) : holder_(_holder) {
	//SEの初期化
	getSE_ = std::make_unique<Audio>();
	getSE_->Initialize("se/itemGet_heal.mp3");
}

void ItemHealCollider::Debug() {
#ifdef _DEBUG
	//基底クラスのデバッグ処理
	OBBColliderBase::Debug();

	//debugLineColorを元に戻す
	debugLineColor_ = { 1,1,1,1 };

#endif // _DEBUG

}

void ItemHealCollider::OnCollision(ICollider* _other, CollisionAttribute _attribute) {
	//当たり判定時の処理
	switch (_attribute) {
		//プレイヤーに当たった場合
	case CollisionAttribute::Player: {
		//仮死状態にする
		holder_->SetState(BaseCharacter::State::kAsphyxia);

		//パーティクル
		holder_->GetIdleParticle()->SetIsPlay(false); // パーティクルを非アクティブにする
		holder_->GetGetParticle()->SetIsPlay(true); // パーティクルをアクティブにする
		TransformEuler transform = holder_->GetGetParticle()->GetBaseTransform();
		transform.translate = holder_->GetWorldTransform().GetTranslate();
		holder_->GetGetParticle()->SetBaseTransform(transform);

		//取得SE
		getSE_->Play(false, 1.0f);

		break;
	}
	default:
		break;
	}
}
