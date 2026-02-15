#include "Jet.h"
#include <ImGuiManager.h>
#include <TextureManager.h>

//アプリケーション
#include <application/object/character/player/Player.h>
#include <application/object/character/enemy/jet/collision/JetCollider.h>

using namespace Norm;

void Jet::Initialize() {
	//ベースエネミーの初期化
	IBaseJetEnemy::Initialize();

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
