#include "BossAppearSystem.h"
#include <Object3dManager.h>
#include <cassert>
#include <ImGuiManager.h>

//アプリケーション
#include "application/system/CameraManager.h"

using namespace Norm;

void BossAppearSystem::Initialize() {
	//パラメーターの取得
	param_ = JsonUtil::GetJsonData("Resources/parameters/bossAppearSystem");

	//ブラックボードの生成・初期化
	blackBoard_ = std::make_unique<BlackBoard>();
	//ブラックボードに必要な情報を登録
	InitBlackBoard();

	//映像演出の生成・初期化
	cinematic_ = std::make_unique<Cinematic>();
	cinematic_->Initialize("Resources/cinematic/boss_appear", blackBoard_.get());

	//レベルローダーの生成と初期化
	levelLoader_ = std::make_unique<LevelLoader>();
	levelLoader_->Initialize();
	//ブラックボードに登録
	blackBoard_->SetValue<Object3d*>("SealedBox_Object", levelLoader_->GetSealedBoxData()->GetObject3d());

	//鍵1の生成・初期化
	{
		key1_ = std::make_unique<Object3d>();
		key1_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Key1"), "key");
		key1_->SetIsDisplay(true);
		key1_->SetIsLightProcess(true);
		//ワールドトランスフォームの初期化
		wtKey1_.Initialize();
		//オブジェクトにセット
		key1_->RegistWorldTransform(&wtKey1_);
		//ブラックボードに登録
		blackBoard_->SetValue<Object3d*>("Key1_Object", key1_.get());
		blackBoard_->SetValue<WorldTransform>("Key1_WorldTransform",wtKey1_);

	}
	//鍵2の生成・初期化
	{
		key2_ = std::make_unique<Object3d>();
		key2_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Key2"), "key");
		key2_->SetIsDisplay(true);
		key2_->SetIsLightProcess(true);
		//ワールドトランスフォームの初期化
		wtKey2_.Initialize();
		//オブジェクトにセット
		key2_->RegistWorldTransform(&wtKey2_);
		//ブラックボードに登録
		blackBoard_->SetValue<Object3d*>("Key2_Object", key2_.get());
		blackBoard_->SetValue<WorldTransform>("Key2_WorldTransform", wtKey2_);
	}


}

void BossAppearSystem::Update() {
	//カメラマネージャーの確認
	assert(cameraManager_ && "カメラマネージャーがセットされていません");

	//レベルローダーの更新
	levelLoader_->Update();

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