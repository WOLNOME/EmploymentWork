#include "EnemyBomb.h"
#include <TextureManager.h>
#include <ImGuiManager.h>
#include <ParticleManager.h>
#include <Object3dManager.h>

//アプリケーション
#include <application/ui/player/PlayerUI.h>

void EnemyBomb::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//インスタンスの生成と初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("red.png");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Enemy_Bomb"), Shape::kSphere);
	object3d_->worldTransform.translate = { 0.0f,10000.0f,0.0f };
	object3d_->SetTexture(textureHandle_);

	uint32_t thWarning = TextureManager::GetInstance()->LoadTexture("red.png");
	warning_ = std::make_unique<Object3d>();
	warning_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("warning"), "circleShadow");
	warning_->SetIsDisplay(false);
	warning_->SetTexture(thWarning);
	warning_->SetIsLightProcess(false);
	warning_->worldTransform.scale = { 40.0f,1.0f,40.0f };

	//パーティクルの生成と初期化
	{
		particle_ = std::make_unique<Particle>();
		particle_->Initialize(ParticleManager::GetInstance()->GenerateName("EnemyBombHit"), "hit");
		particle_->emitter_.isPlay = false;
		particle_->emitter_.transform.scale = { 0.1f,0.1f,0.1f };
	}
	{
		explosion_ = std::make_unique<Particle>();
		explosion_->Initialize(ParticleManager::GetInstance()->GenerateName("Explosion"), "enemy_explosion");
		explosion_->emitter_.isPlay = false;
		explosion_->emitter_.transform.scale = { 40,20,40 };
	}
	//当たり判定の形状を設定
	collisionShapeKind_ = CollisionShapeKind::Sphere;
	//当たり判定の半径を設定
	collisionRadius_ = 40.0f;
	//パラメータの読み込み
	//param_ = JsonUtil::GetJsonData("Resources/parameters/EnemyBomb");
	//初期化時点では死亡状態
	isDead_ = true;

	//影の大きさを調整
	circleShadow_->worldTransform.scale = { 1.0f,1.0f,1.0f };

}

void EnemyBomb::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();
	//弾が死亡していたら更新しない
	if (GetDeadTimer() > 0.0f || GetIsDead()) return;
	//移動処理
	Move();
	//爆風処理
	Blast();
}

void EnemyBomb::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

	ImGui::Begin("敵ボム");
	ImGui::DragFloat3("座標", &object3d_->worldTransform.translate.x, 0.01f);
	ImGui::End();

	//デバッグ用ラインのカラー
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };

#endif // _DEBUG
}

void EnemyBomb::OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) {
	switch (attribute) {
	case CollisionAttribute::Player:
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//当たり判定を無しにする
		SetCollisionAttribute(CollisionAttribute::Nothingness);

		//被弾インジケーターをつける
		playerUI_->GetHitIndicator()->RegistIndicator(generatedPosition_);

		break;
	default:
		break;
	}
}

void EnemyBomb::SetInitParam(const Vector3& _initPos, const Vector3& _targetPos) {
	_targetPos;
	//初期位置を保存
	object3d_->worldTransform.translate = _initPos;
	generatedPosition_ = _initPos;
	warning_->worldTransform.translate = _initPos;
	warning_->worldTransform.translate.y = 0.005f;
	//表示する
	object3d_->SetIsDisplay(true);
	circleShadow_->SetIsDisplay(true);
	warning_->SetIsDisplay(true);
	//速度は0(自由落下)
	velocity_ = { 0.0f,0.0f,0.0f };

	//当たり判定属性をセット
	SetCollisionAttribute(CollisionAttribute::Nothingness);
	//死亡状態を解除
	isDead_ = false;
	prePosition_ = { FLT_MAX,FLT_MAX ,FLT_MAX };

	//爆風
	isBlast_ = false;
	durationTimer_ = 0.0f;
}

void EnemyBomb::Move() {
	//弾が爆風状態なら移動しない
	if (isBlast_) {
		return;
	}

	//重力の影響を加算
	velocity_.y -= gravity_ * kDeltaTime;
	//速度を加算
	object3d_->worldTransform.translate.y += velocity_.y * kDeltaTime;

	//弾が地面に達したら
	if (object3d_->worldTransform.translate.y <= 0.0f) {
		object3d_->worldTransform.translate.y = 0.0f;
		//パーティクルの発生
		particle_->emitter_.transform.translate = object3d_->worldTransform.worldTranslate;
		particle_->emitter_.isPlay = true;
		explosion_->emitter_.transform.translate = object3d_->worldTransform.worldTranslate;
		explosion_->emitter_.transform.translate.y = 10.0f;
		explosion_->emitter_.isPlay = true;
		//モデルを非表示に
		object3d_->SetIsDisplay(false);
		warning_->SetIsDisplay(false);
		circleShadow_->SetIsDisplay(false);

		//当たり判定属性を爆風に
		SetCollisionAttribute(CollisionAttribute::EnemyBlast);
		//爆風の処理を開始
		isBlast_ = true;
	}
}

void EnemyBomb::Blast() {
	//爆風の処理
	if (!isBlast_) {
		return;
	}

	//爆風時間の加算
	durationTimer_ += kDeltaTime;
	if (durationTimer_ >= kDurationTime_) {
		//爆風時間を超えたら終了
		isBlast_ = false;
		durationTimer_ = 0.0f;
		//パーティクルの発生を止める
		particle_->emitter_.isPlay = false;
		explosion_->emitter_.isPlay = false;

		//死亡予約処理
		SetDeadTimer(particle_->GetParam()["LifeTime"]["Max"]);
	}
}
