#include "LevelObjectCollider.h"

//アプリケーション
#include "application/object/level/base/IBaseLevelObject.h"
#include "application/object/level/tree/LevelTree.h"
#include "application/object/level/bigTree/LevelBigTree.h"
#include "application/object/level/rock/LevelRock.h"
#include "application/object/level/bigRock/LevelBigRock.h"
#include "application/object/level/sealedBox/LevelSealedBox.h"

using namespace Norm;

LevelObjectCollider::LevelObjectCollider(IBaseLevelObject* _holder) : OBBColliderBase(), holder_(_holder) {
}

void LevelObjectCollider::Debug() {
#ifdef _DEBUG
	//基底クラスのデバッグ処理
	OBBColliderBase::Debug();

	//debugLineColorを元に戻す
	debugLineColor_ = { 1,1,1,1 };

#endif // _DEBUG
}

void LevelObjectCollider::OnCollision(ICollider* _other, CollisionAttribute _attribute) {
	//当たり判定の属性によって分岐
	switch (_attribute) {
	case CollisionAttribute::Player:
	case CollisionAttribute::PlayerSpecial:
	case CollisionAttribute::PlayerCannon:
	case CollisionAttribute::EnemyBlast:
	case CollisionAttribute::EnemyCannon:
	{
		//ツリーオブジェクトに当たった場合
		if (holder_->GetName() == "LevelTree") {
			//ツリーとしてdynamic_cast
			auto* levelTree = dynamic_cast<LevelTree*>(holder_);

			//当たり判定可視化用ラインの色を変更
			debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
			//倒れる処理
			levelTree->SetIsAction(handle_, true);
			Vector3 invertDirection = (_other->GetWorldTransform().GetWorldTranslate() - GetWorldTransform().GetWorldTranslate());
			invertDirection.y = 0.0f;
			invertDirection.Normalize();
			levelTree->SetInvertDirection(handle_, invertDirection);
			//属性を変更
			SetCollisionAttribute(CollisionAttribute::Nothingness);

		}
		//巨大ツリーオブジェクトに当たった場合
		else if (holder_->GetName() == "LevelBigTree") {
			//巨大ツリーとしてdynamic_cast
			auto* levelBigTree = dynamic_cast<LevelBigTree*>(holder_);
			//当たり判定可視化用ラインの色を変更
			debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		}
		//岩オブジェクトに当たった場合
		else if (holder_->GetName() == "LevelRock") {
			//岩としてdynamic_cast
			auto* levelRock = dynamic_cast<LevelRock*>(holder_);
			//当たり判定可視化用ラインの色を変更
			debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		}
		//巨大岩オブジェクトに当たった場合
		else if (holder_->GetName() == "LevelBigRock") {
			//巨大岩としてdynamic_cast
			auto* levelRock = dynamic_cast<LevelBigRock*>(holder_);
			//当たり判定可視化用ラインの色を変更
			debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		}
		//封印ボックスオブジェクトに当たった場合
		else if (holder_->GetName() == "LevelSealedBox") {
			//封印ボックスとしてdynamic_cast
			auto* levelSealedBox = dynamic_cast<LevelSealedBox*>(holder_);
			//当たり判定可視化用ラインの色を変更
			debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		}

		break;
	}
	default:
		break;
	}

}
