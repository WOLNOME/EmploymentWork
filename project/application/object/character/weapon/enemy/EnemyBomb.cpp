#include "EnemyBomb.h"
#include <TextureManager.h>
#include <ImGuiManager.h>
#include <CombinedParticleManager.h>
#include <Object3dManager.h>

//アプリケーション
#include <application/ui/player/PlayerUI.h>
#include <application/object/character/weapon/enemy/collision/EnemyBombCollider.h>

using namespace Norm;

void EnemyBomb::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//パラメーターの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/enemyBomb");

	//オブジェクトの生成・初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("red.png");
	object3d_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Enemy_Bomb"), Shape::kSphere);
	object3d_->SetTexture(textureHandle_);
	//ワールドトランスフォームの初期化
	worldTransform_.SetTranslate({ FLT_MAX,FLT_MAX,FLT_MAX });

	//危険地帯の生成・初期化
	uint32_t thWarning = TextureManager::GetInstance()->LoadTexture("red.png");
	warning_ = std::make_unique<Object3d>();
	warning_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Warning"), "circleShadow");
	warning_->SetIsDisplay(false);
	warning_->SetTexture(thWarning);
	warning_->SetIsLightProcess(false);
	warning_->SetColor({ 1.0f,1.0f,1.0f,0.8f });
	//危険地帯のワールドトランスフォームの初期化
	warningWorldTransform_.Initialize();
	warningWorldTransform_.SetScale({ 40.0f,1.0f,40.0f });
	//危険地帯オブジェクトにセット
	warning_->RegistWorldTransform(&warningWorldTransform_);

	//爆発パーティクルの生成と初期化
	{
		explosion_ = std::make_unique<CombinedParticle>();
		explosion_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("BombGround"), "Bomb_Ground");
	}

	//当たり判定の生成・初期化
	collider_ = std::make_unique<EnemyBombCollider>(this);
	auto* enemyBombCollider = dynamic_cast<EnemyBombCollider*>(collider_.get());
	collider_->SetCollisionAttribute(CollisionAttribute::Nothingness);
	collider_->SetWorldTransform(&worldTransform_);
	enemyBombCollider->SetRadius(param_["collisionRadiusSphere"]);

	//影の初期化
	csWorldTransform_.SetScale({ 1.0f,1.0f,1.0f });

}

void EnemyBomb::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//爆発演出の半刻が過ぎたら
	if (state_ == State::kAsphyxia && explosion_->GetElapsedTimer() > explosion_->GetDuration() * 0.5f) {
		//アイドル状態にする
		SetState(State::kIdle);
		//危険地帯オブジェクトを非表示にする
		warning_->SetIsDisplay(false);
	}

	//弾がアクティブでないなら更新しない
	if (GetState() != State::kActive)
		return;

	//移動処理
	Move();
}

void EnemyBomb::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

	ImGui::Begin("敵ボム");
	ImGui::End();

#endif // _DEBUG
}

void EnemyBomb::Spawn(const BombMethod& _method, const Vector3& _initPos, const Vector3& _targetPos, float _size) {
	//ステートがアイドルでなければ失敗
	if (state_ != State::kIdle) {
		return;
	}

	///共通処理

	//初期位置を保存
	worldTransform_.SetTranslate(_initPos);
	generatedPosition_ = _initPos;
	//サイズを保存
	worldTransform_.SetScale({ _size ,_size,_size });
	csWorldTransform_.SetScale({ _size ,_size,_size });
	//ワールドトランスフォームを更新（前データの上書き）
	worldTransform_.UpdateMatrix();
	//表示する
	object3d_->SetIsDisplay(true);
	circleShadow_->SetIsDisplay(true);
	warning_->SetIsDisplay(true);
	//当たり判定属性をセット
	collider_->SetCollisionAttribute(CollisionAttribute::Nothingness);
	//アクティブ状態に切り替え
	SetState(State::kActive);

	///別処理
	switch (_method) {
	case BombMethod::Launch:
	{
		//正確な落下地点
		Vector3 fallingPoint = _targetPos;
		fallingPoint.y = 0.0f;

		//初期位置を保存
		Vector3 warningPos = _targetPos;
		warningPos.y = 0.005f;
		warningWorldTransform_.SetTranslate(warningPos);

		//速度
		Vector3 targetVec = fallingPoint - _initPos;
		float hitTime = param_["hitTime"];
		float maxHeight = param_["maxHeight"];
		velocity_.x = targetVec.x / hitTime;
		velocity_.z = targetVec.z / hitTime;
		gravity_ = 2.0f * (maxHeight - fallingPoint.y) / std::powf((hitTime / 2.0f), 2);
		velocity_.y = 4.0f * (_initPos.y - fallingPoint.y) / hitTime;

		break;
	}
	case BombMethod::Fall:
	{
		//初期位置を保存
		Vector3 warningPos = _initPos;
		warningPos.y = 0.005f;
		warningWorldTransform_.SetTranslate(warningPos);

		//速度は0(自由落下)
		velocity_ = { 0.0f,0.0f,0.0f };

		break;
	}
	default:
		break;
	}
}

void EnemyBomb::Move() {
	//当たり判定が爆風になっていたら移動しない
	if (collider_->GetCollisionAttribute() == CollisionAttribute::EnemyBlast) {
		return;
	}

	//新座標を定義
	Vector3 newTranslate = worldTransform_.GetTranslate();

	//重力の影響を加算
	velocity_.y -= gravity_ * kDeltaTime;
	//速度を加算
	newTranslate += velocity_ * kDeltaTime;

	//弾が地面に達したら
	if (newTranslate.y <= 0.0f) {
		newTranslate.y = 0.0f;
		//パーティクルの発生
		TransformEuler transform = explosion_->GetBaseTransform();
		transform.translate = newTranslate;
		explosion_->SetBaseTransform(transform);
		explosion_->SetIsPlay(true);
		//モデルを非表示に
		object3d_->SetIsDisplay(false);
		circleShadow_->SetIsDisplay(false);
		//仮死状態にする
		SetState(State::kAsphyxia);

		//当たり判定属性を爆風に
		collider_->SetCollisionAttribute(CollisionAttribute::EnemyBlast);
	}

	//座標をセット
	worldTransform_.SetTranslate(newTranslate);
}