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

	//モデルの初期化
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Canota"), "canota");
	object3d_->SetIsDisplay(false);
	//ワールドトランスフォームの初期化
	worldTransform_.SetTranslate({ FLT_MAX,FLT_MAX ,FLT_MAX });

	//当たり判定の初期化
	auto* tankCollider = dynamic_cast<TankCollider*>(collider_.get());
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
	csWorldTransform_.SetScale({ 5.3f,5.3f,5.3f });

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
	//ステートがアイドルでなければ失敗
	if (state_ != State::kIdle) {
		return;
	}

	//初期位置を保存（高さはそろえる）
	Vector3 initPos = _initPos;
	initPos.y = 0.0f;
	worldTransform_.SetTranslate(initPos);
	//初期回転を保存
	worldTransform_.SetRotate(_initRotate);
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