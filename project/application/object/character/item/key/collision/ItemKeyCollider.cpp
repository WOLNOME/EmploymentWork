#include "ItemKeyCollider.h"

//アプリケーション
#include <application/object/character/item/key/ItemKey.h>

using namespace Norm;

ItemKeyCollider::ItemKeyCollider(ItemKey* _holder) : holder_(_holder) {
}

void ItemKeyCollider::Debug() {
#ifdef _DEBUG
	//基底クラスのデバッグ処理
	OBBColliderBase::Debug();

	//debugLineColorを元に戻す
	debugLineColor_ = { 1,1,1,1 };

#endif // _DEBUG
}

void ItemKeyCollider::OnCollision(Norm::ICollider* _other, Norm::CollisionAttribute _attribute) {
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

		break;
	}
	default:
		break;
	}
}
