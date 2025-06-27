#include "Skydome.h"
#include "TextureManager.h"
#include "Object3dManager.h"

void Skydome::Initialize() {
	//変数の初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("sky.png");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{},Object3dManager::GetInstance()->GenerateName("Skydome"), "skydome");
	object3d_->SetTexture(textureHandle_);
	object3d_->SetIsLightProcess(false);

}

void Skydome::Update() {
}
