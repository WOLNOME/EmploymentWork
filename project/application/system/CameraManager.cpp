#include "CameraManager.h"
#include <cassert>
#include <Object3dManager.h>
#include <LineManager.h>
#include <ParticleManager.h>
#include <BulletTrailManager.h>

using namespace Norm;

void CameraManager::Initialize() {
}

void CameraManager::Update() {
    if (activeCamera_) {
        activeCamera_->Update();
    }
}

void CameraManager::DebugWithImGui() {
    if (activeCamera_) {
        activeCamera_->DebugWithImGui();
    }
}

void CameraManager::RegistCamera(const std::string& name, std::unique_ptr<Norm::BaseCamera> camera) {
    // nullチェック
    if (!camera) {
        assert(false && "カメラがnullです");
        return;
    }

    // 既に同名カメラが存在するかチェック
    if (cameras_.contains(name)) {
        assert(false && "カメラの名前が既に使われています");
        return;
    }

    // 登録
    cameras_.emplace(name, std::move(camera));

    // まだアクティブカメラが未設定ならこれを設定
    if (!activeCamera_) {
        activeCamera_ = cameras_.at(name).get();
    }

}

void CameraManager::SetActiveCamera(const std::string& name) {
    auto it = cameras_.find(name);

    // 存在チェック
    if (it == cameras_.end()) {
        assert(false && "Camera name not found.");
        return;
    }

    // アクティブカメラ変更
    activeCamera_ = it->second.get();

    // 描画系マネージャーへ反映
    Object3dManager::GetInstance()->SetCamera(activeCamera_);
    LineManager::GetInstance()->SetCamera(activeCamera_);
    ParticleManager::GetInstance()->SetCamera(activeCamera_);
    BulletTrailManager::GetInstance()->SetCamera(activeCamera_);
}