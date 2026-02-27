#include "BarrierCollider.h"

//アプリケーション
#include <application/object/character/enemy/boss/Barrier.h>

using namespace Norm;

BarrierCollider::BarrierCollider(Barrier* _holder) : SphereColliderBase(), holder_(_holder) {
}

void BarrierCollider::Debug() {
#ifdef _DEBUG
	//基底クラスのデバッグ処理
	SphereColliderBase::Debug();

	//debugLineColorを元に戻す
	debugLineColor_ = { 1,1,1,1 };

#endif // _DEBUG
}

void BarrierCollider::OnCollision(ICollider* _other, CollisionAttribute _attribute) {
	//変更する変数の定義
	int hp = holder_->GetHP();
	int effectGenerateCount = 0;

	//当たり判定時の処理
	switch (_attribute) {
	case CollisionAttribute::PlayerCannon:
	{
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };

		hp -= holder_->GetParam()["cannonDamage"];
		effectGenerateCount = 5;
		break;
	}
	case CollisionAttribute::PlayerBullet:
	{
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };

		hp -= holder_->GetParam()["bulletDamage"];
		effectGenerateCount = 1;
		break;
	}
	case CollisionAttribute::PlayerSpecial:
	{
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };

		hp -= holder_->GetParam()["specialDamage"];
		effectGenerateCount = 15;
		break;
	}
	default:
		break;
	}

	// HPを0~MaxHPに収める
	hp = std::clamp(hp, 0, holder_->GetMaxHP());

	// エフェクト生成処理
	if (effectGenerateCount > 0) {

		size_t size = holder_->GetScrapes().size();
		int count = 0;

		for (int i = 0; i < size; i++) {

			if (!holder_->GetScrapes().at(i)->GetIsPlay()) {

				TransformEuler transform =
					holder_->GetScrapes().at(i)->GetBaseTransform();

				transform.translate =
					_other->GetWorldTransform().GetTranslate();

				holder_->GetScrapes().at(i)->SetBaseTransform(transform);
				holder_->GetScrapes().at(i)->SetIsPlay(true);

				count++;
			}

			if (count >= effectGenerateCount) {
				break;
			}
		}
	}

	//変更した変数のセット
	holder_->SetHP(hp);

}
