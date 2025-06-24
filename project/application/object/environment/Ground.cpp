#include "Ground.h"
#include "TextureManager.h"

void Ground::Initialize() {
	//変数の初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("ground.png");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{}, "ground");

}

void Ground::Update() {
	//オブジェクトの更新
	object3d_->Update();
}

void Ground::Draw(BaseCamera& camera_) {
	//オブジェクトの描画
	object3d_->Draw(&camera_, textureHandle_);
}
