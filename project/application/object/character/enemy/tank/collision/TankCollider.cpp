#include "TankCollider.h"

//アプリケーション
#include <application/object/character/enemy/tank/base/IBaseTankEnemy.h>

using namespace Norm;

TankCollider::TankCollider(IBaseTankEnemy* _holder) : OBBColliderBase(), holder_(_holder) {
}

void TankCollider::Debug() {
#ifdef _DEBUG
	//基底クラスのデバッグ処理
	OBBColliderBase::Debug();

	//debugLineColorを元に戻す
	debugLineColor_ = { 1,1,1,1 };

#endif // _DEBUG
}

void TankCollider::OnCollision(ICollider* _other, CollisionAttribute _attribute) {
	//変更する変数の定義
	int hp = holder_->GetHP();
	Vector3 velocity = holder_->GetVelocity();

	//当たり判定時の処理
	switch (_attribute) {
		//プレイヤーに当たった場合
	case CollisionAttribute::Player:
	{
		//HPを減らす
		hp -= 1;
		//0~MaxHPの範囲に収める
		hp = std::clamp(hp, 0, holder_->GetMaxHP());

		//相手の座標の方向と反対方向のベクトルを速度に加算
		Vector3 reflectVec = -(_other->GetWorldTransform().GetTranslate() - GetWorldTransform().GetTranslate()).Normalized();
		velocity.x += reflectVec.x * 50.0f;
		velocity.z += reflectVec.z * 50.0f;

		break;
	}
	//エネミーに当たった場合
	case CollisionAttribute::Enemy:
	{
		//相手の座標と反対方向のベクトルを速度に加算
		Vector3 reflectVec = -(_other->GetWorldTransform().GetTranslate() - GetWorldTransform().GetTranslate()).Normalized();
		velocity.x += reflectVec.x * 30.0f;
		velocity.z += reflectVec.z * 30.0f;

		break;
	}
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
	//バリアに当たった場合
	case CollisionAttribute::Barrier:
	{
		//相手の座標と反対方向のベクトルを速度に加算
		Vector3 reflectVec = -(_other->GetWorldTransform().GetTranslate() - GetWorldTransform().GetTranslate()).Normalized();
		velocity.x += reflectVec.x * 30.0f;
		velocity.z += reflectVec.z * 30.0f;

		break;
	}
	default:
		break;
	}

	//変更した変数のセット
	holder_->SetHP(hp);
	holder_->SetVelocity(velocity);
}
