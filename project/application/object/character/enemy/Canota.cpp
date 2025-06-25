#include "Canota.h"
#include "ImGuiManager.h"

//アプリケーション
#include "application/object/character/player/Player.h"

void Canota::Initialize() {
	//ベースエネミーの初期化
	IBaseEnemy::Initialize();
	//インスタンスの生成と初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{}, "enemy");
	if (light_) {
		object3d_->SetSceneLight(light_);
	}

	//当たり判定のサイズを設定
	collisionLocalAABB_ = {
		.min = { -4.0f, -2.3f, -4.0f },	//最小座標
		.max = { 4.0f, 1.7f, 4.0f }		//最大座標
	};

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/canota");
	//パラメータの反映
	maxHP_ = param_["maxHP"];
	hp_ = maxHP_;
	cannonCoolTime_ = param_["cannonCoolTime"];
	cannonCoolTimer_ = 0.0f;
}

void Canota::Update() {
	//ベースエネミーの更新
	IBaseEnemy::Update();

	//攻撃
	Attack();
}

void Canota::Draw() {
	//ベースエネミーの描画
	IBaseEnemy::Draw();
}

void Canota::DebugWithImGui() {
#ifdef _DEBUG
	//ベースエネミーのデバッグ処理
	IBaseEnemy::DebugWithImGui();
#endif // _DEBUG
}

void Canota::OnCollision(CollisionAttribute attribute) {
	//ベースエネミーの当たり判定処理
	IBaseEnemy::OnCollision(attribute);
}

void Canota::Attack() {
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
