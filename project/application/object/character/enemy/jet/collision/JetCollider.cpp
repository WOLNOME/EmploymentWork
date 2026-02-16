#include "JetCollider.h"

//アプリケーション
#include <application/object/character/enemy/jet/base/IBaseJetEnemy.h>

using namespace Norm;

JetCollider::JetCollider(IBaseJetEnemy* _holder) : OBBColliderBase(), holder_(_holder) {
}

void JetCollider::Debug() {
#ifdef _DEBUG
	//基底クラスのデバッグ処理
	OBBColliderBase::Debug();

	//debugLineColorを元に戻す
	debugLineColor_ = { 1,1,1,1 };

#endif // _DEBUG

}

void JetCollider::OnCollision(ICollider* _other, CollisionAttribute _attribute) {
	//変更する変数の定義
	int hp = holder_->GetHP();

	//当たり判定時の処理
	switch (_attribute) {
		//プレイヤーキャノンに当たった場合
	case CollisionAttribute::PlayerCannon:
	{
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//HPを減らす
		hp -= holder_->GetParam()["cannonDamage"];
		//0~MaxHPの範囲に収める
		hp = std::clamp(hp, 0, holder_->GetMaxHP());
		break;
	}
	//プレイヤー弾に当たった場合
	case CollisionAttribute::PlayerBullet:
	{
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//HPを減らす
		hp -= holder_->GetParam()["bulletDamage"];
		//0~MaxHPの範囲に収める
		hp = std::clamp(hp, 0, holder_->GetMaxHP());
		break;
	}
	//必殺弾に当たった場合
	case CollisionAttribute::PlayerSpecial:
	{
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//HPを減らす
		hp -= holder_->GetParam()["specialDamage"];
		//0~MaxHPの範囲に収める
		hp = std::clamp(hp, 0, holder_->GetMaxHP());

		break;
	}
	default:
		break;
	}

	//変更した変数のセット
	holder_->SetHP(hp);
}
