#include "Barrier.h"
#include <TextureManager.h>
#include <CombinedParticleManager.h>
#include <Object3dManager.h>
#include <cassert>

//アプリケーション
#include <application/object/character/enemy/boss/collision/BarrierCollider.h>
#include <application/object/character/enemy/boss/Boss.h>
#include <application/system/CameraManager.h>

using namespace Norm;

Barrier::~Barrier() {
}

void Barrier::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//SEの初期化
	shaveSE_ = std::make_unique<Audio>();
	shaveSE_->Initialize("se/barrier_shave.mp3");
	breakSE_ = std::make_unique<Audio>();
	breakSE_->Initialize("se/barrier_break.mp3");

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/barrier");
	audioParam_ = JsonUtil::GetJsonData("Resources/parameters/audio");

	//モデルの初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("sky.png");
	object3d_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Barrier"), Shape::ShapeKind::kSphere);
	object3d_->SetTexture(textureHandle_);
	object3d_->SetIsDisplay(false);
	object3d_->SetIsLightProcess(false);
	object3d_->SetColor({ 1,1,1,0.6f });
	//モデルのワールドトランスフォームの初期化
	worldTransform_.SetScale({
		param_["collisionRadiusSphere"],
		param_["collisionRadiusSphere"],
		param_["collisionRadiusSphere"]
		});

	//バリア削れエフェクトの生成・初期化
	{
		for (int i = 0; i < kScrapeNum; i++) {
			scrapes_[i] = std::make_unique<CombinedParticle>();
			scrapes_[i]->Initialize(CombinedParticleManager::GetInstance()->GenerateName("BarrierScrape"), "Barrier_Scrape");
		}
	}
	//バリア割れるエフェクトの生成・初期化
	{
		destroy_ = std::make_unique<CombinedParticle>();
		destroy_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("BarrierDestroy"), "Barrier_Destroy");
	}

	//当たり判定の生成・初期化
	collider_ = std::make_unique<BarrierCollider>(this);
	auto* barrierCollider = dynamic_cast<BarrierCollider*>(collider_.get());
	collider_->SetWorldTransform(&worldTransform_);
	collider_->SetCollisionAttribute(CollisionAttribute::Nothingness);
	barrierCollider->SetRadius(param_["collisionRadiusSphere"]);

	//メンバ変数の初期化
	maxHP_ = param_["maxHP"];
	hp_ = maxHP_;

}

void Barrier::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//移動
	Move();
	//死亡プロセス
	DeadProcess();

}

void Barrier::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

#endif // _DEBUG
}

void Barrier::Spawn(const Vector3& _position) {
	//ステートがアイドルでなければ失敗
	if (state_ != State::kIdle) {
		return;
	}

	//HPをセット
	hp_ = maxHP_;
	//当たり判定を有効化
	collider_->SetCollisionAttribute(CollisionAttribute::Barrier);
	//モデルを表示
	object3d_->SetIsDisplay(true);
	//指定座標に出現
	Vector3 position = _position;
	worldTransform_.SetTranslate(position);
	//ステートをアクティブに変更する
	SetState(State::kActive);
}

void Barrier::Move() {
	//ステートがアクティブでなければ失敗
	if (state_ != State::kActive) {
		return;
	}

	//座標を常にボスの座標に合わせる
	worldTransform_.SetTranslate(boss_->GetWorldTransform().GetWorldTranslate());
}

void Barrier::DamageProcess(int _effectGenerateNum,const Vector3& _subjectPos) {
	//ステートがアクティブでなければ失敗
	if (state_ != State::kActive) {
		return;
	}

	//HPが0より大きいなら
	if (hp_ > 0) {
		//破片エフェクト発生処理
		if (_effectGenerateNum > 0) {

			size_t size = scrapes_.size();
			int count = 0;

			for (int i = 0; i < size; i++) {

				if (!scrapes_[i]->GetIsPlay()) {

					TransformEuler transform =
						scrapes_[i]->GetBaseTransform();
					transform.translate = _subjectPos;
					scrapes_[i]->SetBaseTransform(transform);
					scrapes_[i]->SetIsPlay(true);

					count++;
				}

				if (count >= _effectGenerateNum) {
					break;
				}
			}
		}
		//最大距離
		float maxDistance = audioParam_["distance"].get<float>();
		//カメラまでの距離
		float distance = Vector3(
			worldTransform_.GetTranslate() - cameraManager_->GetActiveCamera()->worldTransform.GetWorldTranslate()
		).Length();
		//音量
		float volume = 0.0f;
		if (distance < maxDistance) {
			volume = MyMath::Lerp(1.0f, 0.0f, distance / maxDistance);
		}
		//削りSE
		shaveSE_->Play(false, volume);
	}
}

void Barrier::DeadProcess() {
	//ステートがアクティブでなければ失敗
	if (state_ != State::kActive) {
		return;
	}

	//HPが0になったら
	if (hp_ <= 0) {
		//バリア破壊エフェクト発生
		TransformEuler transform = destroy_->GetBaseTransform();
		transform.translate = boss_->GetWorldTransform().GetTranslate();
		transform.translate.y = 0.0f;
		destroy_->SetBaseTransform(transform);
		destroy_->SetIsPlay(true);
		//最大距離
		float maxDistance = audioParam_["distance"].get<float>();
		//カメラまでの距離
		float distance = Vector3(
			worldTransform_.GetTranslate() - cameraManager_->GetActiveCamera()->worldTransform.GetWorldTranslate()
		).Length();
		//音量
		float volume = 0.0f;
		if (distance < maxDistance) {
			volume = MyMath::Lerp(1.0f, 0.0f, distance / maxDistance);
		}
		//破壊SE
		breakSE_->Play(false, volume);
		//アイドル状態にする
		SetState(BaseCharacter::State::kIdle);
	}

}