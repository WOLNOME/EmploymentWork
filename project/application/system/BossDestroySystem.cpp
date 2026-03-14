#include "BossDestroySystem.h"
#include <Input.h>
#include <SceneManager.h>
#include <TextureManager.h>
#include <SpriteManager.h>
#include <Object3dManager.h>
#include <CombinedParticleManager.h>
#include <cassert>
#include <ImGuiManager.h>

//アプリケーション
#include "application/system/CameraManager.h"

using namespace Norm;

void BossDestroySystem::Initialize() {
	//パラメーターの取得
	param_ = JsonUtil::GetJsonData("Resources/parameters/bossAppearSystem");

	//ブラックボードの生成・初期化
	blackBoard_ = std::make_unique<BlackBoard>();

	//映像演出の生成・初期化
	cinematic_ = std::make_unique<Cinematic>();
	cinematic_->Initialize("Resources/cinematic/boss_destroy", blackBoard_.get());

	//レベルローダーの生成と初期化
	levelLoader_ = std::make_unique<LevelLoader>();
	levelLoader_->Initialize();
	//封印オブジェクトを消す
	levelLoader_->GetSealedBoxData()->GetObject3d()->SetIsDisplay(false);
	for (auto& collider : levelLoader_->GetSealedBoxData()->GetColliders()) {
		collider.second->SetCollisionAttribute(CollisionAttribute::Nothingness);
	}

	//SEの初期化
	{
		decideSE_ = std::make_unique<Audio>();
		decideSE_->Initialize("se/decide.mp3");
		groundShakeSE_ = std::make_unique<Audio>();
		groundShakeSE_->Initialize("se/ground_shake.mp3");
		explosionSmallSE_ = std::make_unique<Audio>();
		explosionSmallSE_->Initialize("se/explosion_small.mp3");
		explosionLargeSE_ = std::make_unique<Audio>();
		explosionLargeSE_->Initialize("se/explosion_large.mp3");

		//ブラックボードに登録
		blackBoard_->SetValue<Audio*>("Ground_Shake_SE", groundShakeSE_.get());
		blackBoard_->SetValue<Audio*>("Explosion_Small_SE", explosionSmallSE_.get());
		blackBoard_->SetValue<Audio*>("Explosion_Large_SE", explosionLargeSE_.get());
	}

	//レターボックス
	{
		//テクスチャ
		thLetterBox_ = TextureManager::GetInstance()->LoadTexture("letterBox.png");
		//スプライト
		letterBox_ = std::make_unique<Sprite>();
		letterBox_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("letterBox"), Order::Front4, thLetterBox_);
		letterBox_->SetPosition({ 0.0f,0.0f });
		letterBox_->SetIsDisplay(true);
	}
	//スキップUI
	{
		//テクスチャ
		thSkipUI_ = TextureManager::GetInstance()->LoadTexture("skipUI.png");
		//スプライト
		skipUI_ = std::make_unique<Sprite>();
		skipUI_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("skipUI"), Order::Front5, thSkipUI_);
		skipUI_->SetPosition({ 0.0f,0.0f });
		skipUI_->SetIsDisplay(true);
	}
	//ボスの生成・初期化
	{
		boss_ = std::make_unique<Object3d>();
		boss_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Boss"), "boss");
		boss_->SetIsDisplay(true);
		boss_->SetIsLightProcess(true);
		//ワールドトランスフォームの初期化
		wtBoss_ = std::make_unique<WorldTransform>();
		wtBoss_->Initialize();
		wtBoss_->SetTranslate({ 0.0f,16.0f,6.0f });
		wtBoss_->SetRotate({ 0.0f,pi,0.0f });
		//オブジェクトにセット
		boss_->RegistWorldTransform(wtBoss_.get());
		//ブラックボードに登録
		blackBoard_->SetValue<Object3d*>("Boss_Object", boss_.get());
	}
	//黒煙パーティクルの生成・初期化
	{
		std::vector<CombinedParticle*> copy{ kBSNum };
		for (int i = 0; i < kBSNum; i++) {
			blackSmokes_[i] = std::make_unique<CombinedParticle>();
			blackSmokes_[i]->Initialize(CombinedParticleManager::GetInstance()->GenerateName("blackSmoke"), "BlackSmoke");
			blackSmokes_[i]->SetIsRepeat(true);
			copy[i] = blackSmokes_[i].get();
		}
		//ブラックボードに登録
		blackBoard_->SetValue<std::vector<CombinedParticle*>>("BlackSmoke_Particle", copy);
	}
	// 中規模爆発パーティクルの生成・初期化
	{
		std::vector<CombinedParticle*> copy{ kMENum };
		for (int i = 0; i < kMENum; i++) {
			mediumExplosions_[i] = std::make_unique<CombinedParticle>();
			mediumExplosions_[i]->Initialize(
				CombinedParticleManager::GetInstance()->GenerateName("MediumExplosion"),
				"Cannon_Hit"
			);
			copy[i] = mediumExplosions_[i].get();
		}
		blackBoard_->SetValue<std::vector<CombinedParticle*>>("MediumExplosion_Particle", copy);
	}
	// 巨大爆発パーティクルの生成・初期化
	{
		std::vector<CombinedParticle*> copy{ kHENum };
		for (int i = 0; i < kHENum; i++) {
			hugeExplosions_[i] = std::make_unique<CombinedParticle>();
			hugeExplosions_[i]->Initialize(
				CombinedParticleManager::GetInstance()->GenerateName("HugeExplosion"),
				"HugeExplosion"
			);
			copy[i] = hugeExplosions_[i].get();
		}
		blackBoard_->SetValue<std::vector<CombinedParticle*>>("HugeExplosion_Particle", copy);
	}
}

void BossDestroySystem::Update() {
	//カメラマネージャーの確認
	assert(cameraManager_ && "カメラマネージャーがセットされていません");

	//レベルローダーの更新
	levelLoader_->Update();

	//映像演出の更新
	cinematic_->Update();

	//映像演出が終了もしくはスキップボタンが押されたならクリアシーンへ遷移
	{
		Input* input = Input::GetInstance();
		uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("white.png");
		if (!cinematic_->GetIsAllShotsFinished()) {
			if (input->TriggerKey(DIK_ESCAPE) || input->TriggerPadButton(GamePadButton::START)) {

				if (SceneManager::GetInstance()->SetNextScene("GameClear",
					SceneTransitionAnimation::Type::FADE,
					SceneTransitionAnimation::Type::FADE,
					SceneTransitionAnimation::Option::NONE,
					2.0f, textureHandle, TransitionMode::Normal
				)) {
					//決定音を出す
					decideSE_->Play(false, 1.0f);
				}
			}
		}
		else {
			SceneManager::GetInstance()->SetNextScene("GameClear",
				SceneTransitionAnimation::Type::FADE,
				SceneTransitionAnimation::Type::FADE,
				SceneTransitionAnimation::Option::NONE,
				2.0f, textureHandle, TransitionMode::Normal
			);
		}
	}

}

void BossDestroySystem::DebugWithImGui() {
#ifdef _DEBUG

#endif // _DEBUG

}

void BossDestroySystem::SetCameraManager(CameraManager* _cameraManager) {
	cameraManager_ = _cameraManager;
	//ブラックボードに登録
	blackBoard_->SetValue<BaseCamera*>("Camera", cameraManager_->GetActiveCamera());
}
