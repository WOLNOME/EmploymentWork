#include "Skydome.h"
#include "TextureManager.h"

void Skydome::Initialize() {
	//変数の初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("sky.png");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{}, "skydome");

}

void Skydome::Update() {
	//オブジェクトの更新
	object3d_->Update();
}

void Skydome::Draw(BaseCamera& camera_) {
	//オブジェクトの描画
	object3d_->Draw(&camera_, textureHandle_);
}
