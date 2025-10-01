#include "EnemyBomb.h"
#include <TextureManager.h>
#include <ImGuiManager.h>
#include <ParticleManager.h>
#include <Object3dManager.h>

//アプリケーション
#include <application/ui/player/PlayerUI.h>

void EnemyBomb::Initialize() {
	//インスタンスの生成と初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("red.png");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Enemy_Bomb"), Shape::kSphere);
	object3d_->worldTransform.translate = { 0.0f,-10000.0f,0.0f };
	object3d_->SetTexture(textureHandle_);
	//パーティクルの生成と初期化
	particle_ = std::make_unique<Particle>();
	particle_->Initialize(ParticleManager::GetInstance()->GenerateName("EnemyBombHit"), "hit");
	particle_->emitter_.isPlay = false;
	particle_->emitter_.transform.scale = { 0.1f,0.1f,0.1f };
	particle_->emitter_.generateMethod = Particle::GenerateMethod::Clump;
	particle_->emitter_.clumpNum = 10;
	particle_->emitter_.effectStyle = Particle::EffectStyle::OneShot;
	//当たり判定の形状を設定
	collisionShapeKind_ = CollisionShapeKind::Sphere;
	//当たり判定の半径を設定
	collisionRadius_ = 1.0f;
	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/EnemyBomb");
	//初期化時点では死亡状態
	isDead_ = true;
}

void EnemyBomb::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();
	//弾が死亡していたら更新しない
	if (GetDeadTimer() > 0.0f || GetIsDead()) return;
	//移動処理
	Move();
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
		//パーティクルの発生
		particle_->emitter_.transform.translate = object3d_->worldTransform.worldTranslate;
		particle_->emitter_.isPlay = true;
		//死亡予約処理
		SetDeadTimer(particle_->GetParam()["LifeTime"]["Max"]);
		//当たり判定属性をなしに
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
	//表示する
	object3d_->SetIsDisplay(true);
	//速度は0(自由落下)
	velocity_ = { 0.0f,0.0f,0.0f };

	//当たり判定属性をセット
	SetCollisionAttribute(CollisionAttribute::EnemyCannon);
	//死亡状態を解除
	isDead_ = false;
	prePosition_ = { FLT_MAX,FLT_MAX ,FLT_MAX };
}

void EnemyBomb::Move() {
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
		//死亡予約処理
		SetDeadTimer(particle_->GetParam()["LifeTime"]["Max"]);
		//当たり判定属性をなしに
		SetCollisionAttribute(CollisionAttribute::Nothingness);
	}
}
