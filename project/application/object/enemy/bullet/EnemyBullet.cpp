#include "EnemyBullet.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "ParticleManager.h"

void EnemyBullet::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//インスタンスの生成と初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("red.png");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ShapeTag{}, Shape::kSphere);
	//パーティクルの生成と初期化
	particle_ = std::make_unique<Particle>();
	particle_->Initialize(ParticleManager::GetInstance()->GenerateName("enemyBulletHit"), "hit");
	particle_->emitter_.isPlay = false;
	particle_->emitter_.transform.scale = { 0.1f,0.1f,0.1f };
	particle_->emitter_.generateMethod = Particle::GenerateMethod::Clump;
	particle_->emitter_.clumpNum = 10;
	particle_->emitter_.effectStyle = Particle::EffectStyle::OneShot;

	//当たり判定の半径を設定
	radius_ = 1.0f;
	//当たり判定の属性を設定
	SetCollisionAttribute(CollisionAttribute::EnemyBullet);
}

void EnemyBullet::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();
	//弾が死亡していたら更新しない
	if (GetDeadTimer() > 0.0f && GetIsDead()) {
		return;
	}

	//移動処理
	Move();
}

void EnemyBullet::Draw() {
	//弾が死亡していたら描画しない
	if (GetDeadTimer() > 0.0f && GetIsDead()) {
		return;
	}
	//オブジェクトの描画
	object3d_->Draw(camera_, textureHandle_);
}

void EnemyBullet::DrawLine() {
	//ベースキャラクターのライン描画
	BaseCharacter::DrawLine();
}

void EnemyBullet::DebugWithImGui() {
#ifdef _DEBUG

	ImGui::Begin("敵弾");
	ImGui::DragFloat3("座標", &object3d_->worldTransform.translate.x, 0.01f);
	ImGui::End();

	//デバッグ用ラインのカラー
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };

#endif // _DEBUG

}

void EnemyBullet::OnCollision(CollisionAttribute attribute) {
	//当たり判定時の処理
	switch (attribute) {
	case CollisionAttribute::Player:
		//プレイヤーに当たった場合
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//パーティクルの発生
		particle_->emitter_.transform.translate = object3d_->worldTransform.worldTranslate;
		particle_->emitter_.isPlay = true;
		//死亡予約処理
		SetDeadTimer(particle_->GetParam()["LifeTime"]["Max"]);
		//当たり判定属性をなしに
		SetCollisionAttribute(CollisionAttribute::Nothingness);

		break;
	case CollisionAttribute::PlayerBullet:
		//プレイヤー弾に当たった場合
		break;
	default:
		break;
	}
}

void EnemyBullet::SetInitParam(const Vector3& _initPos, const Vector3& _targetPos) {
	//初期位置を保存
	object3d_->worldTransform.translate = _initPos;

	//向きベクトルを算出
	Vector3 targetVec = _targetPos - _initPos;
	//XZ方向の速度を算出
	velocity_.x = targetVec.x / hitTime_;
	velocity_.z = targetVec.z / hitTime_;
	
	//最大高度から重力を求める
	gravity_ = 2.0f * (maxHeight_ - _targetPos.y) / std::powf((hitTime_ / 2.0f), 2);
	//y方向の上昇速度を算出
	velocity_.y = gravity_ * (hitTime_ / 2.0f);
}

void EnemyBullet::Move() {
	//重力をかける
	velocity_.y -= gravity_ * kDeltaTime;

	object3d_->worldTransform.translate += velocity_ * kDeltaTime;

	//弾が地面に当たったら死亡
	if (GetDeadTimer() == 0.0f) {
		if (object3d_->worldTransform.translate.y < 0.0f) {
			object3d_->worldTransform.translate.y = 0.0f;
			//死亡予約処理
			SetDeadTimer(0.1f);
			//当たり判定属性をなしに
			SetCollisionAttribute(CollisionAttribute::Nothingness);
		}
	}

}
