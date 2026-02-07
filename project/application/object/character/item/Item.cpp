#include "Item.h"
#include <TextureManager.h>
#include <Object3dManager.h>
#include <CombinedParticleManager.h>
#include <random>

//アプリケーション
#include <application/object/character/item/collision/ItemCollider.h>

using namespace Norm;

void Item::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//パラメーターの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/item");

	//オブジェクトを生成・初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Item"), Shape::ShapeKind::kCube);
	object3d_->worldTransform.SetTranslate({ FLT_MAX,FLT_MAX ,FLT_MAX });
	object3d_->SetIsDisplay(false);
	
	//アイドル状態のパーティクルを生成
	idleParticle_ = std::make_unique<CombinedParticle>();
	idleParticle_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("item_idle"), "Item_Idle");
	idleParticle_->SetIsRepeat(true);
	//ゲット時のパーティクルを生成
	getParticle_ = std::make_unique<CombinedParticle>();
	getParticle_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("item_get"), "Item_Get");

	//当たり判定の生成・初期化
	collider_ = std::make_unique<ItemCollider>(this);
	auto* itemCollider = dynamic_cast<ItemCollider*>(collider_.get());
	collider_->SetCollisionAttribute(CollisionAttribute::Nothingness);
	collider_->SetWorldTransform(&object3d_->worldTransform);

	//影の大きさを調整
	circleShadow_->worldTransform.SetScale({ 1.0f,1.0f,1.0f });
}

void Item::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//死ぬまでの処理
	UntilDeathProcess();

	//パーティクルの更新
	UpdateParticle();

}

void Item::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();
#endif // _DEBUG
}

void Item::Spawn(const Vector3& _initPos) {
	//初期座標を保存
	Vector3 initPos = _initPos;
	initPos.y = param_["initHeight"];
	object3d_->worldTransform.SetTranslate(initPos);
	//表示する
	object3d_->SetIsDisplay(true);
	circleShadow_->SetIsDisplay(true);
	//静止パーティクルの設定
	TransformEuler transform = idleParticle_->GetBaseTransform();
	transform.translate = _initPos;
	idleParticle_->SetBaseTransform(transform);
	idleParticle_->SetIsPlay(true);
	// 確率でアイテムの種類を決定
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, 4);
	int itemType = dist(mt);
	// アイテムの種類を設定
	uint32_t textureHandle = 0u;
	switch (itemType) {
	case 0:
	case 1: // 40%の確率（回復）
		collider_->SetCollisionAttribute(CollisionAttribute::Item_Heal);
		textureHandle = TextureManager::GetInstance()->LoadTexture("green.png");
		object3d_->SetTexture(textureHandle);
		break;

	case 2: // 20%：リロード速度アップ
		collider_->SetCollisionAttribute(CollisionAttribute::Item_ReloadSpeedUp);
		textureHandle = TextureManager::GetInstance()->LoadTexture("red.png");
		object3d_->SetTexture(textureHandle);
		break;

	case 3: // 20%：移動速度アップ
		collider_->SetCollisionAttribute(CollisionAttribute::Item_MoveSpeedUp);
		textureHandle = TextureManager::GetInstance()->LoadTexture("blue.png");
		object3d_->SetTexture(textureHandle);
		break;

	case 4: // 20%：回転速度アップ
		collider_->SetCollisionAttribute(CollisionAttribute::Item_TurnSpeedUp);
		textureHandle = TextureManager::GetInstance()->LoadTexture("yellow.png");
		object3d_->SetTexture(textureHandle);
		break;

	default:
		break;
	}
	//アクティブ状態にする
	SetState(State::kActive);


}

void Item::UntilDeathProcess() {
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
		Vector3 pos = object3d_->worldTransform.GetTranslate();
		pos.y= MyMath::Lerp(from, to, MyMath::EaseInOutSine(t));
		object3d_->worldTransform.SetTranslate(pos);

		// 状態遷移
		if (swingTimer_ >= swingTime) {
			swingTimer_ = 0.0f;
			isUp_ = !isUp_; // 上昇/下降切り替え
		}
	}

	//新トランスフォーム
	Vector3 newRotate = object3d_->worldTransform.GetRotate();
	Vector3 newScale = object3d_->worldTransform.GetScale();

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
		float scale = MyMath::Lerp(1.0f, 0.0f, getParticle_->GetElapsedTime() / getParticle_->GetDuration());
		newScale = { scale, scale, scale };
	}
	else {
		//回転させる
		newRotate.y += 0.03f;
	}

	//新トランスフォームのセット
	object3d_->worldTransform.SetRotate(newRotate);
	object3d_->worldTransform.SetScale(newScale);
}

void Item::UpdateParticle() {
	TransformEuler transform = idleParticle_->GetBaseTransform();
	transform.translate = object3d_->worldTransform.GetTranslate();
	idleParticle_->SetBaseTransform(transform);
}
