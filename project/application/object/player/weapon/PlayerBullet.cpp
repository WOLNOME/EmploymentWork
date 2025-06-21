#include "PlayerBullet.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "ParticleManager.h"

void PlayerBullet::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//インスタンスの生成と初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("black.png");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ShapeTag{}, Shape::kSphere);
	object3d_->worldTransform.scale = { 0.01f,0.01f,0.01f };
	//パーティクルの生成と初期化
	particle_ = std::make_unique<Particle>();
	particle_->Initialize(ParticleManager::GetInstance()->GenerateName("playerBulletHit"), "hit");
	particle_->emitter_.isPlay = false;
	particle_->emitter_.transform.scale = { 0.1f,0.1f,0.1f };
	particle_->emitter_.generateMethod = Particle::GenerateMethod::Clump;
	particle_->emitter_.clumpNum = 10;
	particle_->emitter_.effectStyle = Particle::EffectStyle::OneShot;

	//当たり判定の半径を設定
	radius_ = 0.01f;

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerBullet");
	lifeTime_ = param_["lifeTime"];
	lifeTimer_ = 0.0f;

	//初期化時点では死亡状態
	isDead_ = true;

}

void PlayerBullet::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();
	//弾が死亡していたら更新しない
	if (GetDeadTimer() > 0.0f || GetIsDead()) return;

	//移動処理
	Move();
}

void PlayerBullet::Draw() {
	//弾が死亡していたら描画しない
	if (GetDeadTimer() > 0.0f || GetIsDead()) return;

	//オブジェクトの描画
	object3d_->Draw(camera_, textureHandle_);
}

void PlayerBullet::DrawLine() {
	//ベースキャラクターのライン描画
	BaseCharacter::DrawLine();
}

void PlayerBullet::DebugWithImGui() {
#ifdef _DEBUG

	ImGui::Begin("プレイヤー銃弾");
	ImGui::DragFloat3("座標", &object3d_->worldTransform.translate.x, 0.01f);
	ImGui::End();

	//デバッグ用ラインのカラー
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };

#endif // _DEBUG

}

void PlayerBullet::OnCollision(CollisionAttribute attribute) {
	//当たり判定時の処理
	switch (attribute) {
	case CollisionAttribute::Enemy:
		//敵に当たった場合
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//パーティクルの発生
		particle_->emitter_.transform.translate = object3d_->worldTransform.worldTranslate;
		particle_->emitter_.isPlay = true;
		//死亡予約処理
		SetDeadTimer(particle_->GetParam()["LifeTime"]["Max"]);
		//当たり判定属性をなしに
		SetCollisionAttribute(CollisionAttribute::Nothingness);

		break;
	case CollisionAttribute::EnemyBullet:
		//敵弾に当たった場合
		break;
	default:
		break;
	}
}

void PlayerBullet::Move() {
	//移動量の大きさを制限
	float maxSpeed = param_["maxSpeed"];
	if (velocity_.Length() > maxSpeed) {
		velocity_.Normalize();
		velocity_ *= maxSpeed;
	}
	object3d_->worldTransform.translate += velocity_ * kDeltaTime;

	//弾が地面に当たったら死亡
	if (GetDeadTimer() == 0.0f && !isDead_) {
		if (object3d_->worldTransform.translate.y < 0.0f) {
			object3d_->worldTransform.translate.y = 0.0f;
			//死亡予約処理
			SetDeadTimer(0.1f);
			//当たり判定属性をなしに
			SetCollisionAttribute(CollisionAttribute::Nothingness);
		}
	}

	//弾が寿命を迎えたら死亡
	lifeTimer_ += kDeltaTime;
	if (lifeTimer_ >= lifeTime_) {
		if (GetDeadTimer() == 0.0f && !isDead_) {
			//死亡予約処理
			SetDeadTimer(0.1f);
			//当たり判定属性をなしに
			SetCollisionAttribute(CollisionAttribute::Nothingness);

			lifeTimer_ = 0.0f;	//寿命タイマーをリセット
		}
	}
	

}
