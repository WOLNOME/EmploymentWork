#include "PlayerBulletCollider.h"

//アプリケーション
#include <application/object/character/weapon/player/PlayerBullet.h>

using namespace Norm;

PlayerBulletCollider::PlayerBulletCollider(PlayerBullet* _holder) : SphereColliderBase(), holder_(_holder) {
}

void PlayerBulletCollider::Debug() {
#ifdef _DEBUG
	//基底クラスのデバッグ処理
	SphereColliderBase::Debug();

	//debugLineColorを元に戻す
	debugLineColor_ = { 1,1,1,1 };

#endif // _DEBUG
}

void PlayerBulletCollider::OnCollision(ICollider* _other, CollisionAttribute _attribute) {
	//当たり判定時の処理
	switch (_attribute) {
		//敵に当たった場合
	case CollisionAttribute::Enemy:
	{
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//死亡処理
		holder_->DeadProcess();

		break;
	}
	//敵キャノンに当たった場合
	case CollisionAttribute::EnemyCannon:
	{
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//死亡処理
		holder_->DeadProcess();

		break;
	}
	//バリアに当たった場合
	case CollisionAttribute::Barrier:
	{
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//死亡処理
		holder_->DeadProcess();

		break;
	}
	//壁に当たった場合
	case CollisionAttribute::Wall:
	{
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//死亡処理
		holder_->DeadProcess();

		break;
	}
	default:
		break;
	}
}
