#include "Boss.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "Object3dManager.h"

//アプリケーション
#include "application/object/character/player/Player.h"

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

	//当たり判定の半径を設定
	collisionLocalAABB_ = {
		.min = { -6.0f, -4.5f, -6.0f },	//最小座標
		.max = { 6.0f, 2.5f, 6.0f }		//最大座標
	};

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/boss");
	//パラメータの反映
	maxHP_ = param_["maxHP"];
	hp_ = maxHP_;
	cannonCoolTime_ = param_["cannonCoolTime"];
	cannonCoolTimer_ = 0.0f;

}

void Boss::Update() {
	//ベースエネミーの更新
	IBaseEnemy::Update();

	//攻撃
	Attack();
}

void Boss::DebugWithImGui() {
#ifdef _DEBUG
	//ベースエネミーのデバッグ処理
	IBaseEnemy::DebugWithImGui();
#endif // _DEBUG
}

void Boss::OnCollision(CollisionAttribute attribute) {
	//ベースエネミーの当たり判定処理
	IBaseEnemy::OnCollision(attribute);
}

void Boss::Attack() {
	//死亡していたら処理を行わない
	if (GetDeadTimer() > 0.0f || isDead_) return;

	//クールタイム処理
	if (cannonCoolTimer_ > 0.0f) {
		cannonCoolTimer_ -= kDeltaTime;
		//クールタイムがマイナスになったら0にする
		if (cannonCoolTimer_ < 0.0f) {
			cannonCoolTimer_ = 0.0f;
		}
		//砲弾を発射したフラグをオフ
		isCannonFire_ = false;
		//計算後はこの関数を抜ける
		return;
	}

	//もしプレイヤーが索敵範囲内にいなければ処理を行わない。
	float searchPlayerDistanceAttack = param_["searchPlayerDistanceAttack"];
	if (player_->GetWorldTransform().translate.Distance(object3d_->worldTransform.translate) > searchPlayerDistanceAttack) {
		return;
	}
	//未攻撃状態なら攻撃処理
	if (!isCannonFire_) {
		//砲弾を発射したフラグをオン
		isCannonFire_ = true;
		//クールタイムをセット
		cannonCoolTimer_ = cannonCoolTime_;
	}
}
