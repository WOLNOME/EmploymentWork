#include "Boss.h"
#include <ImGuiManager.h>
#include <TextureManager.h>
#include <Object3dManager.h>

//アプリケーション
#include <application/object/character/player/Player.h>

Boss::~Boss() {

}

void Boss::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/boss");

	//当たり判定の形状を設定
	collisionShapeKind_ = CollisionShapeKind::OBB;
	//当たり判定の属性を設定
	SetCollisionAttribute(CollisionAttribute::Enemy);
	//当たり判定のパラメーター入力
	collisionCenterOffsetOBB_ = { param_["collisionCenterOffsetOBB"]["x"],param_["collisionCenterOffsetOBB"]["y"] ,param_["collisionCenterOffsetOBB"]["z"] };
	collisionSizeOBB_ = { param_["collisionSizeOBB"]["x"],param_["collisionSizeOBB"]["y"] ,param_["collisionSizeOBB"]["z"] };

	//モデルの生成・初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Boss"), "boss");

	//影の大きさを調整
	circleShadow_->worldTransform.scale = { 16.0f,1.0f,16.0f };

	//ブラックボードの生成
	blackBoard_ = std::make_unique<BlackBoard>();
	//ブラックボードに必要な情報を登録
	{
		//定数情報
		blackBoard_->SetValue<float>("ChaseDistance", param_["chaseDistance

		//ボスのパラメーター
		blackBoard_->SetValue<Vector3>("BossPos", object3d_->worldTransform.translate);

	}

}

void Boss::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();
}

void Boss::DebugWithImGui() {
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();
}

