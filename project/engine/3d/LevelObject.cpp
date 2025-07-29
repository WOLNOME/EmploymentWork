#include "LevelObject.h"
#include <Object3dManager.h>
#include <CollisionManager.h>

void LevelObject::Initialize(const std::string& _name, const std::string& _filePath, const TransformEuler& _transform) {
	//名前のセット
	name_ = _name;
	//オブジェクトの生成と初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName(_name), _filePath);
	//トランスフォームパラメータのセット
	object3d_->worldTransform.translate = _transform.translate;
	object3d_->worldTransform.rotate = _transform.rotate;
	object3d_->worldTransform.scale = _transform.scale;
}

void LevelObject::Update() {
	//コリジョンが有効でなければreturn
	if (!isCollisionEnabled_) {
		return;
	}

	//前フレーム座標の更新
	prePosition_ = object3d_->worldTransform.translate;
	//当たり判定を登録
	CollisionManager::GetInstance()->SetColliders(this);
}

void LevelObject::DebugWithImGui() {
#ifdef _DEBUG
	//コライダーのデバッグ処理
	Collider::DebugWithImGui();
	//当たり判定可視化用ラインの色を変更
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };

#endif // _DEBUG
}

void LevelObject::OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) {
	//当たり判定の属性によって分岐
	switch (attribute) {
	case CollisionAttribute::Player:
		//ツリーオブジェクトに当たった場合
		if (name_ == "TreeObject") {
			//非表示にする
			object3d_->SetIsDisplay(false);
			//当たり判定を無効化
			isCollisionEnabled_ = false;
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
}

void LevelObject::SetCollisionInfo(const Vector3& _center, const Vector3& _size) {
	//コリジョンの有効化
	isCollisionEnabled_ = true;
	collisionShapeKind_ = CollisionShapeKind::OBB;
	collisionCenterOffsetOBB_ = _center / 2.0f;
	collisionSizeOBB_ = _size / 2.0f;
	//当たり判定の属性を設定
	SetCollisionAttribute(CollisionAttribute::Wall);
}
