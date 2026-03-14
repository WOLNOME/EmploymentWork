#include "BossAppearSystem.h"
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

void BossAppearSystem::Initialize() {
	//SEの初期化
	decideSE_ = std::make_unique<Audio>();
	decideSE_->Initialize("se/decide.mp3");

	//パラメーターの取得
	param_ = JsonUtil::GetJsonData("Resources/parameters/bossAppearSystem");

	//ブラックボードの生成・初期化
	blackBoard_ = std::make_unique<BlackBoard>();

	//映像演出の生成・初期化
	cinematic_ = std::make_unique<Cinematic>();
	cinematic_->Initialize("Resources/cinematic/boss_appear", blackBoard_.get());

	//レベルローダーの生成と初期化
	levelLoader_ = std::make_unique<LevelLoader>();
	levelLoader_->Initialize();
	//ブラックボードに登録
	blackBoard_->SetValue<Object3d*>("SealedBox_Object", levelLoader_->GetSealedBoxData()->GetObject3d());

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
	//鍵1の生成・初期化
	{
		key1_ = std::make_unique<Object3d>();
		key1_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Key1"), "key");
		key1_->SetIsDisplay(true);
		key1_->SetIsLightProcess(true);
		//ワールドトランスフォームの初期化
		wtKey1_ = std::make_unique<WorldTransform>();
		wtKey1_->Initialize();
		//オブジェクトにセット
		key1_->RegistWorldTransform(wtKey1_.get());
		//ブラックボードに登録
		blackBoard_->SetValue<Object3d*>("Key1_Object", key1_.get());
		blackBoard_->SetValue<WorldTransform*>("Key1_WorldTransform",wtKey1_.get());

	}
	//鍵2の生成・初期化
	{
		key2_ = std::make_unique<Object3d>();
		key2_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Key2"), "key");
		key2_->SetIsDisplay(true);
		key2_->SetIsLightProcess(true);
		//ワールドトランスフォームの初期化
		wtKey2_ = std::make_unique<WorldTransform>();
		wtKey2_->Initialize();
		//オブジェクトにセット
		key2_->RegistWorldTransform(wtKey2_.get());
		//ブラックボードに登録
		blackBoard_->SetValue<Object3d*>("Key2_Object", key2_.get());
		blackBoard_->SetValue<WorldTransform*>("Key2_WorldTransform", wtKey2_.get());
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
		//オブジェクトにセット
		boss_->RegistWorldTransform(wtBoss_.get());
		//ブラックボードに登録
		blackBoard_->SetValue<Object3d*>("Boss_Object", boss_.get());
		blackBoard_->SetValue<WorldTransform*>("Boss_WorldTransform", wtBoss_.get());
	}
	//フラッシュパーティクル1の生成・初期化
	{
		flush1_ = std::make_unique<CombinedParticle>();
		flush1_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("flush1"), "Flush");
		//ブラックボードに登録
		blackBoard_->SetValue<CombinedParticle*>("Flush1_Particle", flush1_.get());
	}
	//フラッシュパーティクル2の生成・初期化
	{
		flush2_ = std::make_unique<CombinedParticle>();
		flush2_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("flush2"), "Flush");
		//ブラックボードに登録
		blackBoard_->SetValue<CombinedParticle*>("Flush2_Particle", flush2_.get());
	}
	//封印解除中パーティクルの生成・初期化
	{
		verge_ = std::make_unique<CombinedParticle>();
		verge_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("verge"), "Verge");
		verge_->SetIsRepeat(true);
		//ブラックボードに登録
		blackBoard_->SetValue<CombinedParticle*>("Verge_Particle", verge_.get());
	}
	//ボス出現時パーティクル
	{
		appear_ = std::make_unique<CombinedParticle>();
		appear_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("appear"), "Boss_Appear");
		//ブラックボードに登録
		blackBoard_->SetValue<CombinedParticle*>("Appear_Particle", appear_.get());
	}

}

void BossAppearSystem::Update() {
	//カメラマネージャーの確認
	assert(cameraManager_ && "カメラマネージャーがセットされていません");

	//レベルローダーの更新
	levelLoader_->Update();

	//映像演出の更新
	cinematic_->Update();

	//映像演出が終了もしくはスキップボタンが押されたならゲームプレイシーンへ戻る
	{
		Input* input = Input::GetInstance();
		uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("black.png");
		if (!cinematic_->GetIsAllShotsFinished()) {
			if (input->TriggerKey(DIK_ESCAPE) || input->TriggerPadButton(GamePadButton::START)) {
				
				if (SceneManager::GetInstance()->SetNextScene("GamePlay",
					SceneTransitionAnimation::Type::FADE,
					SceneTransitionAnimation::Type::FADE,
					SceneTransitionAnimation::Option::NONE,
					0.5f, textureHandle, TransitionMode::FromKeep
				)) {
					//決定音を出す
					decideSE_->Play(false, 1.0f);
				}
			}
		}
		else {
			SceneManager::GetInstance()->SetNextScene("GamePlay",
				SceneTransitionAnimation::Type::FADE,
				SceneTransitionAnimation::Type::FADE,
				SceneTransitionAnimation::Option::NONE,
				0.5f, textureHandle, TransitionMode::FromKeep
			);
		}
	}
}

void BossAppearSystem::DebugWithImGui() {
#ifdef _DEBUG
	
#endif // _DEBUG

}

void BossAppearSystem::SetCameraManager(CameraManager* _cameraManager) {
	cameraManager_ = _cameraManager;
	//ブラックボードに登録
	blackBoard_->SetValue<BaseCamera*>("Camera", cameraManager_->GetActiveCamera());
}
