#include "BossAppearSystem.h"
#include <cassert>

//アプリケーション
#include "application/system/CameraManager.h"

using namespace Norm;

void BossAppearSystem::Initialize() {
	//レベルローダーの生成と初期化
	levelLoader_ = std::make_unique<LevelLoader>();
	levelLoader_->Initialize();
}

void BossAppearSystem::Update() {
	//カメラマネージャーの確認
	assert(cameraManager_ && "カメラマネージャーがセットされていません");

	//レベルローダーの更新
	levelLoader_->Update();

	//演出
	Direction();

}

void BossAppearSystem::DebugWithImGui() {
}

void BossAppearSystem::Direction() {


}
