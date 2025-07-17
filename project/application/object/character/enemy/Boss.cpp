#include "Boss.h"
#include <ImGuiManager.h>
#include <TextureManager.h>
#include <Object3dManager.h>

//アプリケーション
#include <application/object/character/player/Player.h>

void Boss::Initialize() {
	//ベースキャラクターの初期化
	IBaseEnemy::Initialize();
	//テクスチャ
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("boss.png");

	//インスタンスの生成と初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{},Object3dManager::GetInstance()->GenerateName("Boss"), "enemy");
	object3d_->worldTransform.scale = { 1.5f,1.5f,1.5f };
	object3d_->SetTexture(textureHandle_);
	uint32_t environmentTextureHandle = TextureManager::GetInstance()->LoadTexture("skybox.dds");
	object3d_->SetEnvironmentLightTextureHandle(environmentTextureHandle);

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/boss");

	//当たり判定のパラメーター入力
	collisionCenterOffsetOBB_ = { param_["collisionCenterOffsetOBB"]["x"],param_["collisionCenterOffsetOBB"]["y"] ,param_["collisionCenterOffsetOBB"]["z"] };
	collisionSizeOBB_ = { param_["collisionSizeOBB"]["x"],param_["collisionSizeOBB"]["y"] ,param_["collisionSizeOBB"]["z"] };

	//パラメータの反映
	maxHP_ = param_["maxHP"];
	hp_ = maxHP_;
}

void Boss::Update() {
	//ベースエネミーの更新
	IBaseEnemy::Update();

}

void Boss::DebugWithImGui() {
#ifdef _DEBUG
	//ベースエネミーのデバッグ処理
	IBaseEnemy::DebugWithImGui();
#endif // _DEBUG
}

void Boss::OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) {
	//ベースエネミーの当たり判定処理
	IBaseEnemy::OnCollision(attribute,subjectPos);
}

