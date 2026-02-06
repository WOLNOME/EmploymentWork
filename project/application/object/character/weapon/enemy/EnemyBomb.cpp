#include "EnemyBomb.h"
#include <TextureManager.h>
#include <ImGuiManager.h>
#include <CombinedParticleManager.h>
#include <Object3dManager.h>

//アプリケーション
#include <application/ui/player/PlayerUI.h>

using namespace Norm;

void EnemyBomb::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//パラメーターの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/enemyBomb");

	//インスタンスの生成と初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("red.png");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Enemy_Bomb"), Shape::kSphere);
	object3d_->worldTransform.SetTranslate({ FLT_MAX,FLT_MAX,FLT_MAX });
	object3d_->SetTexture(textureHandle_);

	uint32_t thWarning = TextureManager::GetInstance()->LoadTexture("red.png");
	warning_ = std::make_unique<Object3d>();
	warning_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("warning"), "circleShadow");
	warning_->SetIsDisplay(false);
	warning_->SetTexture(thWarning);
	warning_->SetIsLightProcess(false);
	warning_->worldTransform.SetScale({40.0f,1.0f,40.0f});

	//パーティクルの生成と初期化
	{
		explosion_ = std::make_unique<CombinedParticle>();
		explosion_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("BombExplosion"), "Explosion");
	}
	//当たり判定の形状を設定
	collisionShapeKind_ = CollisionShapeKind::Sphere;
	//当たり判定の半径を設定
	collisionRadius_ = 40.0f;

	//影の初期化
	circleShadow_->worldTransform.SetScale({ 1.0f,1.0f,1.0f });

}

void EnemyBomb::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//爆発演出が終了したらアイドル状態にする
	if (state_ == State::kAsphyxia && !explosion_->GetIsPlay()) {
		SetState(State::kIdle);
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

	//デバッグ用ラインのカラー
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };

#endif // _DEBUG
}

void EnemyBomb::Spawn(const BombMethod& _method, const Vector3& _initPos, const Vector3& _targetPos) {
	///共通処理

	//初期位置を保存
	object3d_->worldTransform.SetTranslate(_initPos);
	generatedPosition_ = _initPos;
	//表示する
	object3d_->SetIsDisplay(true);
	circleShadow_->SetIsDisplay(true);
	warning_->SetIsDisplay(true);
	//当たり判定属性をセット
	SetCollisionAttribute(CollisionAttribute::Nothingness);
	//アクティブ状態に切り替え
	SetState(State::kActive);
	prePosition_ = { FLT_MAX,FLT_MAX ,FLT_MAX };

	///別処理
	switch (_method) {
	case BombMethod::Launch: {
		//初期位置を保存
		Vector3 warningPos = _targetPos;
		warningPos.y = 0.005f;
		warning_->worldTransform.SetTranslate(warningPos);

		//速度
		Vector3 targetVec = _targetPos - _initPos;
		float hitTime = param_["hitTime"];
		float maxHeight = param_["maxHeight"];
		velocity_.x = targetVec.x / hitTime;
		velocity_.z = targetVec.z / hitTime;
		gravity_ = 2.0f * (maxHeight - _targetPos.y) / std::powf((hitTime / 2.0f), 2);
		velocity_.y = 4.0f * (_initPos.y - _targetPos.y) / hitTime;

		break;
	}
	case BombMethod::Fall: {
		//初期位置を保存
		Vector3 warningPos = _initPos;
		warningPos.y = 0.005f;
		warning_->worldTransform.SetTranslate(warningPos);

		//速度は0(自由落下)
		velocity_ = { 0.0f,0.0f,0.0f };

		break;
	}
	default:
		break;
	}
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

void EnemyBomb::Move() {
	//当たり判定が爆風になっていたら移動しない
	if (GetCollisionAttribute() == CollisionAttribute::EnemyBlast) {
		return;
	}

	//新座標を定義
	Vector3 newTranslate = object3d_->worldTransform.GetTranslate();

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
		warning_->SetIsDisplay(false);
		circleShadow_->SetIsDisplay(false);
		//仮死状態にする
		SetState(State::kAsphyxia);

		//当たり判定属性を爆風に
		SetCollisionAttribute(CollisionAttribute::EnemyBlast);
	}

	//座標をセット
	object3d_->worldTransform.SetTranslate(newTranslate);
}