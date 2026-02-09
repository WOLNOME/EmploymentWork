#include "Jet.h"
#include <ImGuiManager.h>
#include <TextureManager.h>
#include <Object3dManager.h>

//アプリケーション
#include <application/object/character/player/Player.h>
#include <application/object/character/enemy/jet/collision/JetCollider.h>

using namespace Norm;

void Jet::Initialize() {
	//ベースエネミーの初期化
	IBaseJetEnemy::Initialize();

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/jet");

	//テクスチャ
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("jet.png");
	//オブジェクトの生成と初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Jet"), "jet");
	object3d_->worldTransform.SetTranslate({ FLT_MAX,FLT_MAX ,FLT_MAX });
	object3d_->worldTransform.SetScale({ 1.5f,1.5f,1.5f });
	object3d_->SetIsDisplay(false);
	object3d_->SetTexture(textureHandle_);

	//当たり判定の初期化
	auto* jetCollider = dynamic_cast<JetCollider*>(collider_.get());
	collider_->SetWorldTransform(&object3d_->worldTransform);
	collider_->SetOffset({
		param_["collisionCenterOffsetOBB"]["x"],
		param_["collisionCenterOffsetOBB"]["y"],
		param_["collisionCenterOffsetOBB"]["z"]
		});
	jetCollider->SetOBBSize({
		param_["collisionSizeOBB"]["x"],
		param_["collisionSizeOBB"]["y"],
		param_["collisionSizeOBB"]["z"]
		});

	//パラメータの反映
	maxHP_ = param_["maxHP"];
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

void Jet::Spawn(const Vector3& _initPos, const Vector3& _initRotate) {
	//ステートがアイドルでなければ失敗
	if (state_ != State::kIdle) {
		return;
	}

	//初期位置を保存（高さはそろえる）
	Vector3 initPos = _initPos;
	initPos.y = 40.0f;
	object3d_->worldTransform.SetTranslate(initPos);
	//初期回転を保存
	object3d_->worldTransform.SetRotate(_initRotate);
	//表示する
	object3d_->SetIsDisplay(true);
	circleShadow_->SetIsDisplay(true);
	//HPの設定
	hp_ = maxHP_;
	//当たり判定の属性を設定
	collider_->SetCollisionAttribute(CollisionAttribute::Enemy);
	//アクティブ状態にする
	SetState(State::kActive);
	//ステートを決定
	ChangeState("Patrol");
}
