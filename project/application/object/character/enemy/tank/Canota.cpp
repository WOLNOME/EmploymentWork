#include "Canota.h"
#include <ImGuiManager.h>
#include <Object3dManager.h>

//アプリケーション
#include <application/object/character/player/Player.h>
#include <application/object/character/enemy/tank/collision/TankCollider.h>

using namespace Norm;

void Canota::Initialize() {
	//ベースエネミーの初期化
	IBaseTankEnemy::Initialize();

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/canota");

	//インスタンスの生成と初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Canota"), "enemy");
	object3d_->worldTransform.SetTranslate({ FLT_MAX,FLT_MAX ,FLT_MAX });
	object3d_->SetIsDisplay(false);

	//当たり判定の初期化
	auto* tankCollider = dynamic_cast<TankCollider*>(collider_.get());
	collider_->SetWorldTransform(&object3d_->worldTransform);
	collider_->SetOffset({
		param_["collisionCenterOffsetOBB"]["x"],
		param_["collisionCenterOffsetOBB"]["y"],
		param_["collisionCenterOffsetOBB"]["z"]
		});
	tankCollider->SetOBBSize({
		param_["collisionSizeOBB"]["x"],
		param_["collisionSizeOBB"]["y"],
		param_["collisionSizeOBB"]["z"]
		});

	//パラメータの反映
	maxHP_ = param_["maxHP"];

	//影の初期化
	circleShadow_->worldTransform.SetScale({ 5.3f,5.3f,5.3f });

}

void Canota::Update() {
	//ベースエネミーの更新
	IBaseTankEnemy::Update();
}

void Canota::DebugWithImGui() {
#ifdef _DEBUG
	//ベースエネミーのデバッグ処理
	IBaseTankEnemy::DebugWithImGui();
#endif // _DEBUG
}

void Canota::Spawn(const Vector3& _initPos, const Vector3& _initRotate) {
	//初期位置を保存（高さはそろえる）
	Vector3 initPos = _initPos;
	initPos.y = 3.0f;
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
	//前フレーム座標を初期化
	prePosition_ = { FLT_MAX,FLT_MAX ,FLT_MAX };

}