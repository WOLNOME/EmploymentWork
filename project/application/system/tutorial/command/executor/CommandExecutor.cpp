#include "CommandExecutor.h"
#include <cassert>
#include <Vector3.h>
#include <TextureManager.h>

//アプリケーション
#include <application/system/tutorial/command/InputSystem.h>
#include <application/system/tutorial/command/CameraSystem.h>
#include <application/system/tutorial/command/MessageSystem.h>
#include <application/system/tutorial/command/ObjectiveSystem.h>
#include <application/system/tutorial/command/UISystem.h>
#include <application/system/tutorial/command/ObjectSystem.h>

using namespace Norm;

void CommandExecutor::Initialize() {
    // ===== System生成 =====
    inputSystem_ = std::make_unique<InputSystem>();
    messageSystem_ = std::make_unique<MessageSystem>();
    cameraSystem_ = std::make_unique<CameraSystem>();
    uiSystem_ = std::make_unique<UISystem>();
    objectiveSystem_ = std::make_unique<ObjectiveSystem>();
    objectSystem_ = std::make_unique<ObjectSystem>();

    // ===== System初期化 =====
    inputSystem_->Initialize();
    messageSystem_->Initialize();
    cameraSystem_->Initialize();
    uiSystem_->Initialize();
    objectiveSystem_->Initialize();
    objectSystem_->Initialize();
}

void CommandExecutor::ExecuteCommand(const std::string& _name, const json& _param) {
    //チェック
    assert(inputSystem_ && "InputSystem が初期化されていません");
    assert(messageSystem_ && "MessageSystem が初期化されていません");
    assert(cameraSystem_ && "CameraSystem が初期化されていません");
    assert(uiSystem_ && "UISystem が初期化されていません");
    assert(objectiveSystem_ && "ObjectiveSystem が初期化されていません");
    assert(objectSystem_ && "ObjectSystem が初期化されていません");
    
    //処理
    if (_name == "LockInput") {
        inputSystem_->SetIsLocked(true);
    }
    else if (_name == "UnlockInput") {
        inputSystem_->SetIsLocked(false);
    }
    else if (_name == "ShowMessage") {
        if (_param.contains("text")) {
            messageSystem_->ShowMessage(_param["text"].get<std::string>());
        }
    }
    else if (_name == "OpenMessageWindow") {
        messageSystem_->Open();
    }
    else if (_name == "CloseMessageWindow") {
        messageSystem_->Close();
    }
    else if (_name == "MoveCameraToTarget") {
        cameraSystem_->MoveToTarget();
    }
    else if (_name == "FixPlayer") {

        Vector3 pos = {
            _param["pos"][0].get<float>(),
            _param["pos"][1].get<float>(),
            _param["pos"][2].get<float>()
        };

        Vector3 rot = {
            _param["rot"][0].get<float>(),
            _param["rot"][1].get<float>(),
            _param["rot"][2].get<float>()
        };

        objectSystem_->FixPlayer(pos, rot);
    }
    else if (_name == "ShowMoveTutorialSprite") {
        //テクスチャハンドル
        uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("uvChecker.png");
        uiSystem_->ShowSprite(textureHandle);
    }
    else if (_name == "SetObjective") {

        std::string text;
        int count = 0;

        if (_param.contains("text")) {
            text = _param["text"].get<std::string>();
        }

        if (_param.contains("count")) {
            count = _param["count"].get<int>();
        }

        objectiveSystem_->Set(text, count);
    }
    else if (_name == "SpawnItem") {

        const auto& transforms = _param["transforms"];

        for (const auto& t : transforms) {

            Vector3 pos = {
                t["pos"][0].get<float>(),
                t["pos"][1].get<float>(),
                t["pos"][2].get<float>()
            };

            Vector3 rot = {
                t["rot"][0].get<float>(),
                t["rot"][1].get<float>(),
                t["rot"][2].get<float>()
            };

            objectSystem_->SpawnItem(pos, rot);
        }
    }
}
