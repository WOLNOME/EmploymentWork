#include "EnemyBulletCollider.h"

//アプリケーション
#include <application/object/character/weapon/enemy/EnemyBullet.h>
#include <application/ui/player/PlayerUI.h>

using namespace Norm;

EnemyBulletCollider::EnemyBulletCollider(EnemyBullet* _holder) : SphereColliderBase(), holder_(_holder) {
}

void EnemyBulletCollider::Debug() {
#ifdef _DEBUG
	//基底クラスのデバッグ処理
	SphereColliderBase::Debug();

	//debugLineColorを元に戻す
	debugLineColor_ = { 1,1,1,1 };

#endif // _DEBUG

}

void EnemyBulletCollider::OnCollision(ICollider* _other, CollisionAttribute _attribute) {
	//当たり判定時の処理
	switch (_attribute) {
		//プレイヤーに当たった場合
	case CollisionAttribute::Player:
	{
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//死亡処理
		holder_->DeadProcess();

		//被弾インジケーターをつける
		holder_->GetPlayerUI()->GetHitIndicator()->RegistIndicator(holder_->GetGeneratedPosition());

		break;
	}
	//プレイヤー砲弾に当たった場合
	case CollisionAttribute::PlayerCannon:
	{
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//死亡処理
		holder_->DeadProcess();

		break;
	}
	//プレイヤー必殺弾に当たった場合
	case CollisionAttribute::PlayerSpecial:
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
