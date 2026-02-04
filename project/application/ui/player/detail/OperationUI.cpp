#include "OperationUI.h"
#include <TextureManager.h>
#include <SpriteManager.h>
#include <ImGuiManager.h>

using namespace Norm;

void OperationUI::Initialize() {
	//砲弾攻撃UIスプライトの初期化
	textureHandles_[(int)Type::kCannon] = TextureManager::GetInstance()->LoadTexture("space.png");
	sprites_[(int)Type::kCannon] = std::make_unique<Sprite>();
	sprites_[(int)Type::kCannon]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("operationUI"), Order::Front2, textureHandles_[(int)Type::kCannon]);
	sprites_[(int)Type::kCannon]->SetAnchorPoint({ 0.5f,0.5f });
	sprites_[(int)Type::kCannon]->SetPosition({ 290.0f,640.0f });
	sprites_[(int)Type::kCannon]->SetSize(sprites_[(int)Type::kCannon]->GetSize() * 0.95f);

	//機関銃攻撃UIスプライトの初期化
	textureHandles_[(int)Type::kBullet] = TextureManager::GetInstance()->LoadTexture("leftClick.png");
	sprites_[(int)Type::kBullet] = std::make_unique<Sprite>();
	sprites_[(int)Type::kBullet]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("operationUI"), Order::Front2, textureHandles_[(int)Type::kBullet]);
	sprites_[(int)Type::kBullet]->SetAnchorPoint({ 0.5f,0.5f });
	sprites_[(int)Type::kBullet]->SetPosition({ 790.0f,640.0f });
	sprites_[(int)Type::kBullet]->SetSize(sprites_[(int)Type::kBullet]->GetSize() * 0.95f);
	//視点移動UIスプライトの初期化
	textureHandles_[(int)Type::kPerspective] = TextureManager::GetInstance()->LoadTexture("mouseSlide.png");
	sprites_[(int)Type::kPerspective] = std::make_unique<Sprite>();
	sprites_[(int)Type::kPerspective]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("operationUI"), Order::Front2, textureHandles_[(int)Type::kPerspective]);
	sprites_[(int)Type::kPerspective]->SetAnchorPoint({ 0.5f,0.5f });
	sprites_[(int)Type::kPerspective]->SetPosition({ 490.0f,640.0f });
	sprites_[(int)Type::kPerspective]->SetSize(sprites_[(int)Type::kPerspective]->GetSize() * 0.95f);
	//移動UIスプライトの初期化
	textureHandles_[(int)Type::kMove] = TextureManager::GetInstance()->LoadTexture("ws.png");
	sprites_[(int)Type::kMove] = std::make_unique<Sprite>();
	sprites_[(int)Type::kMove]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("operationUI"), Order::Front2, textureHandles_[(int)Type::kMove]);
	sprites_[(int)Type::kMove]->SetAnchorPoint({ 0.5f,0.5f });
	sprites_[(int)Type::kMove]->SetPosition({ 1020.0f,640.0f });
	sprites_[(int)Type::kMove]->SetSize(sprites_[(int)Type::kMove]->GetSize() * 0.95f);
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