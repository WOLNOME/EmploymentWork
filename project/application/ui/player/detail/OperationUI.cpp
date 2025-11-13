#include "OperationUI.h"
#include <TextureManager.h>
#include <SpriteManager.h>
#include <ImGuiManager.h>

void OperationUI::Initialize() {
	//砲弾攻撃UIスプライトの初期化
	textureHandles_[(int)Type::Cannon] = TextureManager::GetInstance()->LoadTexture("space.png");
	sprites_[(int)Type::Cannon] = std::make_unique<Sprite>();
	sprites_[(int)Type::Cannon]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("operationUI"), Order::Front2, textureHandles_[(int)Type::Cannon]);
	sprites_[(int)Type::Cannon]->SetAnchorPoint({ 0.5f,0.5f });
	sprites_[(int)Type::Cannon]->SetPosition({ 290.0f,640.0f });
	sprites_[(int)Type::Cannon]->SetSize(sprites_[(int)Type::Cannon]->GetSize() * 0.95f);

	//機関銃攻撃UIスプライトの初期化
	textureHandles_[(int)Type::Bullet] = TextureManager::GetInstance()->LoadTexture("leftClick.png");
	sprites_[(int)Type::Bullet] = std::make_unique<Sprite>();
	sprites_[(int)Type::Bullet]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("operationUI"), Order::Front2, textureHandles_[(int)Type::Bullet]);
	sprites_[(int)Type::Bullet]->SetAnchorPoint({ 0.5f,0.5f });
	sprites_[(int)Type::Bullet]->SetPosition({ 790.0f,640.0f });
	sprites_[(int)Type::Bullet]->SetSize(sprites_[(int)Type::Bullet]->GetSize() * 0.95f);
	//視点移動UIスプライトの初期化
	textureHandles_[(int)Type::Perspective] = TextureManager::GetInstance()->LoadTexture("mouseSlide.png");
	sprites_[(int)Type::Perspective] = std::make_unique<Sprite>();
	sprites_[(int)Type::Perspective]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("operationUI"), Order::Front2, textureHandles_[(int)Type::Perspective]);
	sprites_[(int)Type::Perspective]->SetAnchorPoint({ 0.5f,0.5f });
	sprites_[(int)Type::Perspective]->SetPosition({ 490.0f,640.0f });
	sprites_[(int)Type::Perspective]->SetSize(sprites_[(int)Type::Perspective]->GetSize() * 0.95f);
	//移動UIスプライトの初期化
	textureHandles_[(int)Type::Move] = TextureManager::GetInstance()->LoadTexture("ws.png");
	sprites_[(int)Type::Move] = std::make_unique<Sprite>();
	sprites_[(int)Type::Move]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("operationUI"), Order::Front2, textureHandles_[(int)Type::Move]);
	sprites_[(int)Type::Move]->SetAnchorPoint({ 0.5f,0.5f });
	sprites_[(int)Type::Move]->SetPosition({ 1020.0f,640.0f });
	sprites_[(int)Type::Move]->SetSize(sprites_[(int)Type::Move]->GetSize() * 0.95f);
}

void OperationUI::Update() {

}

void OperationUI::AttachShake(const Vector2& _shakeOffset) {
	//スプライトにシェイクを適用する
	for (int i = 0; i < (int)Type::kMaxTypeNum; i++) {
		sprites_[i]->SetShakeOffset(_shakeOffset);
	}
}

void OperationUI::AttachBlinking(const Vector4& _color) {
	//スプライトに点滅を適用する
	for (int i = 0; i < (int)Type::kMaxTypeNum; i++) {
		sprites_[i]->SetColor(_color);
	}
}