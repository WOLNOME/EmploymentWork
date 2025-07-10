#include "Skydome.h"
#include "TextureManager.h"
#include "Object3dManager.h"

void Skydome::Initialize() {
	//変数の初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("rostock_laage_airport_4k.dds");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ShapeTag{},Object3dManager::GetInstance()->GenerateName("SkyBox"),Shape::ShapeKind::kSkyBox);
	object3d_->worldTransform.scale = { 900.0f,900.0f,900.0f };
	object3d_->SetTexture(textureHandle_);
	object3d_->SetIsLightProcess(false);

}
