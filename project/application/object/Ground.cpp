#include "Ground.h"
#include "TextureManager.h"

void Ground::Initialize() {
	//変数の初期化
	worldTransform_.Initialize();
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("ground.png");
	object3d_ = std::make_unique<Object3d>();
	object3d_->InitializeModel("ground");

}

void Ground::Update() {
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Ground::Draw(BaseCamera& camera_) {
	//オブジェクトの描画
	object3d_->Draw(worldTransform_, camera_, nullptr, textureHandle_);
}
