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
	Collider::DebugWithImGui();
#endif // _DEBUG
}

void LevelObject::OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) {
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
