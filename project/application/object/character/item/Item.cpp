#include "Item.h"
#include <TextureManager.h>
#include <Object3dManager.h>
#include <ParticleManager.h>
#include <random>

void Item::Initialize(const Vector3& _initPos) {
	//パラメーターの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/item");

	//当たり判定の形状を設定
	collisionShapeKind_ = CollisionShapeKind::OBB;
	//オブジェクトを生成・初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Item"), Shape::ShapeKind::kCube);
	object3d_->worldTransform.translate = _initPos;
	object3d_->worldTransform.translate.y = param_["initHeight"];

	//アイドル状態のパーティクルを生成
	idleParticle_ = std::make_unique<Particle>();
	idleParticle_->Initialize(ParticleManager::GetInstance()->GenerateName("item_idle"), "item_idle");
	idleParticle_->emitter_.transform.translate = _initPos;
	idleParticle_->emitter_.transform.scale = { 1.0f, 1.0f, 1.0f };
	idleParticle_->emitter_.isGravity = true;
	idleParticle_->emitter_.gravity = 1.0f;
	//ゲット時のパーティクルを生成
	getParticle_ = std::make_unique<Particle>();
	getParticle_->Initialize(ParticleManager::GetInstance()->GenerateName("item_get"), "item_get");
	getParticle_->emitter_.transform.scale = { 0.1f,0.1f,0.1f };
	getParticle_->emitter_.generateMethod = Particle::GenerateMethod::Clump;
	getParticle_->emitter_.clumpNum = 15;
	getParticle_->emitter_.effectStyle = Particle::EffectStyle::OneShot;
	getParticle_->emitter_.isPlay = false;
	getParticle_->emitter_.isBillboard = false;

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
		SetCollisionAttribute(CollisionAttribute::Item_Heal);
		textureHandle = TextureManager::GetInstance()->LoadTexture("green.png");
		object3d_->SetTexture(textureHandle);
		break;

	case 2: // 20%：リロード速度アップ
		SetCollisionAttribute(CollisionAttribute::Item_ReloadSpeedUp);
		textureHandle = TextureManager::GetInstance()->LoadTexture("red.png");
		object3d_->SetTexture(textureHandle);
		break;

	case 3: // 20%：移動速度アップ
		SetCollisionAttribute(CollisionAttribute::Item_MoveSpeedUp);
		textureHandle = TextureManager::GetInstance()->LoadTexture("blue.png");
		object3d_->SetTexture(textureHandle);
		break;

	case 4: // 20%：回転速度アップ
		SetCollisionAttribute(CollisionAttribute::Item_TurnSpeedUp);
		textureHandle = TextureManager::GetInstance()->LoadTexture("yellow.png");
		object3d_->SetTexture(textureHandle);
		break;

	default:
		break;
	}
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

	//デバッグ用ラインのカラー
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };

#endif // _DEBUG
}

void Item::OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) {
	//当たり判定時の処理
	switch (attribute) {
		//プレイヤーに当たった場合
	case CollisionAttribute::Player: {
		//死ぬ
		float deadTime = param_["deadTime"];
		SetDeadTimer(deadTime);
		//当たり判定属性を消す
		SetCollisionAttribute(CollisionAttribute::Nothingness);

		//パーティクル
		idleParticle_->emitter_.isPlay = false; // パーティクルを非アクティブにする
		getParticle_->emitter_.isPlay = true; // パーティクルをアクティブにする
		getParticle_->emitter_.transform.translate = object3d_->worldTransform.translate; // パーティクルの位置をアイテムの位置に合わせる
		getParticle_->emitter_.transform.translate.y = 0.5f;

		break;
	}
	default:
		break;
	}
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
		object3d_->worldTransform.translate.y = MyMath::Lerp(from, to, MyMath::EaseInOutSine(t));

		// 状態遷移
		if (swingTimer_ >= swingTime) {
			swingTimer_ = 0.0f;
			isUp_ = !isUp_; // 上昇/下降切り替え
		}
	}

	// アイテムが消えるまでの処理
	if (GetDeadTimer() > 0.0f) {
		//表示
		object3d_->SetIsDisplay(true);
		//回転させる
		object3d_->worldTransform.rotate.y += 0.3f;
		//縮小
		float deadTime = param_["deadTime"];
		float scale = MyMath::Lerp(0.0f, 1.0f, GetDeadTimer() / deadTime);
		object3d_->worldTransform.scale = { scale, scale, scale };
	}
	else {
		//回転させる
		object3d_->worldTransform.rotate.y += 0.03f;
	}
}

void Item::UpdateParticle() {
	//パーティクルの位置をオブジェクトの位置に合わせる
	idleParticle_->emitter_.transform.translate = object3d_->worldTransform.translate;

}
