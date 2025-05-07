#include "Skydome.h"
#include "TextureManager.h"

void Skydome::Initialize() {
	//変数の初期化
	worldTransform_.Initialize();
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("sky.png");
	object3d_ = std::make_unique<Object3d>();
	object3d_->InitializeModel("skydome");

}

void Skydome::Update() {
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(BaseCamera& camera_) {
	//オブジェクトの描画
	object3d_->Draw(worldTransform_, camera_, nullptr, textureHandle_);
}
