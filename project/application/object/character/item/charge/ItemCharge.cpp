#include "ItemCharge.h"
#include <TextureManager.h>
#include <Object3dManager.h>
#include <CombinedParticleManager.h>

//アプリケーション
#include <application/object/character/item/charge/collision/ItemChargeCollider.h>

using namespace Norm;

void ItemCharge::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//パラメーターの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/item");

	//オブジェクトを生成・初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("blue.png");
	object3d_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Item_Charge"), Shape::ShapeKind::kCube);
	object3d_->SetTexture(textureHandle_);
	object3d_->SetIsDisplay(false);
	//ワールドトランスフォームの初期化
	worldTransform_.SetTranslate({ FLT_MAX,FLT_MAX ,FLT_MAX });

	//アイドル状態のパーティクルを生成
	idleParticle_ = std::make_unique<CombinedParticle>();
	idleParticle_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("item_idle"), "Item_Idle");
	idleParticle_->SetIsRepeat(true);
	//ゲット時のパーティクルを生成
	getParticle_ = std::make_unique<CombinedParticle>();
	getParticle_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("item_get"), "Item_Get");

	//当たり判定の生成・初期化
	collider_ = std::make_unique<ItemChargeCollider>(this);
	auto* itemCollider = dynamic_cast<ItemChargeCollider*>(collider_.get());
	collider_->SetCollisionAttribute(CollisionAttribute::Nothingness);
	collider_->SetWorldTransform(&worldTransform_);
	itemCollider->SetOBBSize({
		param_["collisionSizeOBB"]["x"].get<float>(),
		param_["collisionSizeOBB"]["y"].get<float>(),
		param_["collisionSizeOBB"]["z"].get<float>()
		});

	//影の大きさを調整
	csWorldTransform_.SetScale({ 1.0f,1.0f,1.0f });
}

void ItemCharge::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//死ぬまでの処理
	UntilDeathProcess();

	//パーティクルの更新
	UpdateParticle();

}

void ItemCharge::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();
#endif // _DEBUG
}


void ItemCharge::Spawn(const Vector3& _initPos) {
	//初期座標を保存
	Vector3 initPos = _initPos;
	initPos.y = param_["initHeight"];
	worldTransform_.SetTranslate(initPos);
	//初期スケールを保存
	worldTransform_.SetScale({ 1,1,1 });
	//表示する
	object3d_->SetIsDisplay(true);
	circleShadow_->SetIsDisplay(true);
	//静止パーティクルの設定
	TransformEuler transform = idleParticle_->GetBaseTransform();
	transform.translate = _initPos;
	idleParticle_->SetBaseTransform(transform);
	idleParticle_->SetIsPlay(true);
	//変数のリセット
	swingTimer_ = 0.0f;
	isUp_ = true;
	//コライダーの属性を決める
	collider_->SetCollisionAttribute(CollisionAttribute::Item_Charge);
	//アクティブ状態にする
	SetState(State::kActive);

}

void ItemCharge::UntilDeathProcess() {
	//共通処理
	{
		// タイマー更新
		swingTimer_ += kDeltaTime;

		// 補間率計算（0～1）
		float swingTime = param_["swingTime"];
		float t = std::clamp(swingTimer_ / swingTime, 0.0f, 1.0f);

		// 上下移動
		float initHeight = param_["initHeight"];
		float swingWidth = param_["swingWidth"];
		float from = isUp_ ? initHeight : initHeight + swingWidth;
		float to = isUp_ ? initHeight + swingWidth : initHeight;
		Vector3 pos = worldTransform_.GetTranslate();
		pos.y = MyMath::Lerp(from, to, MyMath::EaseInOutSine(t));
		worldTransform_.SetTranslate(pos);

		// 状態遷移
		if (swingTimer_ >= swingTime) {
			swingTimer_ = 0.0f;
			isUp_ = !isUp_; // 上昇/下降切り替え
		}
	}

	//新トランスフォーム
	Vector3 newRotate = worldTransform_.GetRotate();
	Vector3 newScale = worldTransform_.GetScale();

	// アイテムが消えるまでの処理(仮死状態の時)
	if (state_ == State::kAsphyxia) {
		//演出が終了したらアイドル状態にする
		if (!getParticle_->GetIsPlay()) {
			SetState(State::kIdle);
		}

		//表示
		object3d_->SetIsDisplay(true);
		//回転させる(めちゃ速く)
		newRotate.y += 0.3f;
		//縮小
		float scale = MyMath::Lerp(1.0f, 0.0f, getParticle_->GetElapsedTimer() / getParticle_->GetDuration());
		newScale = { scale, scale, scale };
	}
	else {
		//回転させる
		newRotate.y += 0.03f;
	}

	//新トランスフォームのセット
	worldTransform_.SetRotate(newRotate);
	worldTransform_.SetScale(newScale);
}

void ItemCharge::UpdateParticle() {
	TransformEuler transform = idleParticle_->GetBaseTransform();
	transform.translate = worldTransform_.GetTranslate();
	idleParticle_->SetBaseTransform(transform);
}
