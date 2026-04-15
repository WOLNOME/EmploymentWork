#include "PlayerSpecialCollider.h"

//アプリケーション
#include <application/object/character/weapon/player/PlayerSpecial.h>
#include <application/object/character/weapon/player/manager/PlayerWeaponManager.h>

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

		//死亡処理
		holder_->DeadProcess(PlayerSpecial::DeadType::Collide);

		};

	//当たり判定時の処理
	switch (_attribute) {
		//敵に当たった場合
	case CollisionAttribute::Enemy:
	{
		//共通処理
		commonCollisionProcess();

		//ヒットカウントの加算
		int currentHitNum = holder_->GetPlayerWeaponManager()->GetSpecialHitNum();
		holder_->GetPlayerWeaponManager()->SetSpecialHitNum(currentHitNum + 1);

		break;
	}
	//バリアに当たった場合
	case CollisionAttribute::Barrier:
		//共通処理
		commonCollisionProcess();

		break;
	default:
		break;
	}
}
