#include "LevelObjectCollider.h"

//アプリケーション
#include "application/object/level/loader/LevelLoader.h"


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
	//変更する変数の取得
	bool isAction = holder_->GetIsAction();
	Vector3 invertDirection = holder_->GetInvertDirection();

	//当たり判定の属性によって分岐
	switch (_attribute) {
	case CollisionAttribute::Player:
		//ツリーオブジェクトに当たった場合
		if (holder_->GetName() == "TreeObject") {
			//当たり判定可視化用ラインの色を変更
			debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
			//倒れる処理
			isAction = true;
			invertDirection = (_other->GetWorldTransform().GetWorldTranslate() - GetWorldTransform().GetWorldTranslate());
			invertDirection.y = 0.0f;
			invertDirection.Normalize();

			//属性を変更
			SetCollisionAttribute(CollisionAttribute::Nothingness);

		}

		break;
	case CollisionAttribute::Enemy:
		break;
	case CollisionAttribute::PlayerBullet:
		break;
	case CollisionAttribute::EnemyBullet:
		break;
	default:
		break;
	}

	//変更する変数のセット
	holder_->SetIsAction(isAction);
	holder_->SetInvertDirection(invertDirection);

}
