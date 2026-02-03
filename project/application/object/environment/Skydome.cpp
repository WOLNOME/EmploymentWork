#include "Skydome.h"
#include "TextureManager.h"
#include "Object3dManager.h"

void Skydome::Initialize() {
	//変数の初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("skybox.dds");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ShapeTag{},Object3dManager::GetInstance()->GenerateName("SkyBox"),Shape::ShapeKind::kSkyBox);
	Vector3 pos = object3d_->worldTransform.GetTranslate();
	pos.y += 120.0f;
	object3d_->worldTransform.SetTranslate(pos);
	object3d_->worldTransform.SetScale({ 1500.0f,1500.0f,1500.0f });
	object3d_->SetTexture(textureHandle_);
	object3d_->SetIsLightProcess(false);

}
