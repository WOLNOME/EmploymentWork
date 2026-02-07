#include "EnemyBombCollider.h"

//アプリケーション
#include <application/object/character/weapon/enemy/EnemyBomb.h>
#include <application/ui/player/PlayerUI.h>

using namespace Norm;

EnemyBombCollider::EnemyBombCollider(EnemyBomb* _holder) : SphereColliderBase(), holder_(_holder) {
}

void EnemyBombCollider::Debug() {
#ifdef _DEBUG
	//基底クラスのデバッグ処理
	SphereColliderBase::Debug();

	//debugLineColorを元に戻す
	debugLineColor_ = { 1,1,1,1 };

#endif // _DEBUG

}

void EnemyBombCollider::OnCollision(ICollider* _other, CollisionAttribute _attribute) {

	//当たり判定の処理
	switch (_attribute) {
	case CollisionAttribute::Player:
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//当たり判定を無しにする
		SetCollisionAttribute(CollisionAttribute::Nothingness);

		//被弾インジケーターをつける
		holder_->GetPlayerUI()->GetHitIndicator()->RegistIndicator(holder_->GetGeneratedPosition());

		break;
	default:
		break;
	}
}
