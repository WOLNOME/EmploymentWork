#include "Canota.h"
#include "ImGuiManager.h"
#include "Object3dManager.h"

//アプリケーション
#include "application/object/character/player/Player.h"

void Canota::Initialize() {
	//ベースエネミーの初期化
	IBaseEnemy::Initialize();
	//インスタンスの生成と初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Canota"), "enemy");

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/canota");

	//当たり判定のパラメーター入力
	collisionCenterOffsetOBB_ = { param_["collisionCenterOffsetOBB"]["x"],param_["collisionCenterOffsetOBB"]["y"] ,param_["collisionCenterOffsetOBB"]["z"] };
	collisionSizeOBB_ = { param_["collisionSizeOBB"]["x"],param_["collisionSizeOBB"]["y"] ,param_["collisionSizeOBB"]["z"] };

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

void Canota::DebugWithImGui() {
#ifdef _DEBUG
	//ベースエネミーのデバッグ処理
	IBaseEnemy::DebugWithImGui();
#endif // _DEBUG
}

void Canota::OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) {
	//ベースエネミーの当たり判定処理
	IBaseEnemy::OnCollision(attribute,subjectPos);
}

void Canota::Attack() {
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
