#include "EnemyCannon.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "ParticleManager.h"

void EnemyCannon::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//インスタンスの生成と初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("red.png");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ShapeTag{}, Shape::kSphere);
	//パーティクルの生成と初期化
	particle_ = std::make_unique<Particle>();
	particle_->Initialize(ParticleManager::GetInstance()->GenerateName("EnemyCannonHit"), "hit");
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
	param_ = JsonUtil::GetJsonData("Resources/parameters/EnemyCannon");

	//初期化時点では死亡状態
	isDead_ = true;
}

void EnemyCannon::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();
	//弾が死亡していたら更新しない
	if (GetDeadTimer() > 0.0f || GetIsDead()) return;

	//移動処理
	Move();
}

void EnemyCannon::Draw() {
	//弾が死亡していたら描画しない
	if (GetDeadTimer() > 0.0f || GetIsDead()) {
		return;
	}
	//オブジェクトの描画
	object3d_->Draw(camera_, textureHandle_);
}

void EnemyCannon::DrawLine() {
	//ベースキャラクターのライン描画
	BaseCharacter::DrawLine();
}

void EnemyCannon::DebugWithImGui() {
#ifdef _DEBUG

	ImGui::Begin("敵キャノン");
	ImGui::DragFloat3("座標", &object3d_->worldTransform.translate.x, 0.01f);
	ImGui::End();

	//デバッグ用ラインのカラー
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };

#endif // _DEBUG

}

void EnemyCannon::OnCollision(CollisionAttribute attribute) {
	//当たり判定時の処理
	switch (attribute) {
		//プレイヤーに当たった場合
	case CollisionAttribute::Player:
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//パーティクルの発生
		particle_->emitter_.transform.translate = object3d_->worldTransform.worldTranslate;
		particle_->emitter_.isPlay = true;
		//死亡予約処理
		SetDeadTimer(particle_->GetParam()["LifeTime"]["Max"]);
		//当たり判定属性をなしに
		SetCollisionAttribute(CollisionAttribute::Nothingness);

		break;
		//プレイヤー弾に当たった場合
	case CollisionAttribute::PlayerBullet:
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//パーティクルの発生
		particle_->emitter_.transform.translate = object3d_->worldTransform.worldTranslate;
		particle_->emitter_.isPlay = true;
		//死亡予約処理
		SetDeadTimer(particle_->GetParam()["LifeTime"]["Max"]);
		//当たり判定属性をなしに
		SetCollisionAttribute(CollisionAttribute::Nothingness);

		break;
		//プレイヤーキャノンに当たった場合
	case CollisionAttribute::PlayerCannon:
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//パーティクルの発生
		particle_->emitter_.transform.translate = object3d_->worldTransform.worldTranslate;
		particle_->emitter_.isPlay = true;
		//死亡予約処理
		SetDeadTimer(particle_->GetParam()["LifeTime"]["Max"]);
		//当たり判定属性をなしに
		SetCollisionAttribute(CollisionAttribute::Nothingness);

		break;
	default:
		break;
	}
}

void EnemyCannon::SetInitParam(const Vector3& _initPos, const Vector3& _targetPos) {
	//初期位置を保存
	object3d_->worldTransform.translate = _initPos;

	//向きベクトルを算出
	Vector3 targetVec = _targetPos - _initPos;
	//XZ方向の速度を算出
	float hitTime = param_["hitTime"];
	velocity_.x = targetVec.x / hitTime;
	velocity_.z = targetVec.z / hitTime;

	//最大高度から重力を求める
	float maxHeight = param_["maxHeight"];
	//もしinitPosのy座標がmaxHeightより大きい場合
	if (_initPos.y > maxHeight) {
		//最大高度をinitPosのy座標にする
		maxHeight = _initPos.y;
		gravity_ = 2.0f * (_initPos.y - _targetPos.y) / std::powf(hitTime, 2);
		//y方向の上昇速度は0
		velocity_.y = 0.0f;
		//当たり判定属性をセット
		SetCollisionAttribute(CollisionAttribute::EnemyBullet);
		//死亡状態を解除
		isDead_ = false;
		prePosition_ = { FLT_MAX,FLT_MAX ,FLT_MAX };
		return;
	}

	gravity_ = 2.0f * (maxHeight - _targetPos.y) / std::powf((hitTime / 2.0f), 2);
	//y方向の上昇速度を算出
	velocity_.y = 4.0f * (_initPos.y - _targetPos.y) / hitTime;

	//当たり判定属性をセット
	SetCollisionAttribute(CollisionAttribute::EnemyBullet);
	//死亡状態を解除
	isDead_ = false;
	prePosition_ = { FLT_MAX,FLT_MAX ,FLT_MAX };
}

void EnemyCannon::Move() {
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
