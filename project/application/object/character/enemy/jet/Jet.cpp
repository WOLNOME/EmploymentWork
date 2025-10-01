#include "Jet.h"
#include <ImGuiManager.h>
#include <TextureManager.h>
#include <Object3dManager.h>

//アプリケーション
#include <application/object/character/player/Player.h>

void Jet::Initialize() {
	//ベースキャラクターの初期化
	IBaseJetEnemy::Initialize();
	//テクスチャ
	//textureHandle_ = TextureManager::GetInstance()->LoadTexture("jet.png");
	//インスタンスの生成と初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Jet"), "bunny");
	object3d_->worldTransform.scale = { 1.5f,1.5f,1.5f };
	//object3d_->SetTexture(textureHandle_);
	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/jet");
	//当たり判定のパラメーター入力
	collisionCenterOffsetOBB_ = { param_["collisionCenterOffsetOBB"]["x"],param_["collisionCenterOffsetOBB"]["y"] ,param_["collisionCenterOffsetOBB"]["z"] };
	collisionSizeOBB_ = { param_["collisionSizeOBB"]["x"],param_["collisionSizeOBB"]["y"] ,param_["collisionSizeOBB"]["z"] };
	//パラメータの反映
	maxHP_ = param_["maxHP"];
	hp_ = maxHP_;
}

void Jet::Update() {
	//ベースエネミーの更新
	IBaseJetEnemy::Update();
}

void Jet::DebugWithImGui() {
#ifdef _DEBUG
	//ベースエネミーのデバッグ処理
	IBaseJetEnemy::DebugWithImGui();
#endif // _DEBUG
}

void Jet::OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) {
	//ベースエネミーの当たり判定処理
	IBaseJetEnemy::OnCollision(attribute, subjectPos);
}
