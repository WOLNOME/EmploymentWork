#include "CommandExecutor.h"

//アプリケーション
#include <application/system/tutorial/command/InputSystem.h>
#include <application/system/tutorial/command/CameraSystem.h>
#include <application/system/tutorial/command/MessageSystem.h>
#include <application/system/tutorial/command/ObjectiveSystem.h>
#include <application/system/tutorial/command/UISystem.h>

using namespace Norm;

void CommandExecutor::Initialize() {
}

void CommandExecutor::ExecuteCommand(const std::string& _name, const json& _param) {
    if (_name == "LockInput") {
        if (inputSystem_) {
            inputSystem_->SetIsLocked(true);
        }
    }
    else if (_name == "UnlockInput") {
        if (inputSystem_) {
            inputSystem_->SetIsLocked(false);
        }
    }
    else if (_name == "ShowMessage") {
        if (messageSystem_ && _param.contains("text")) {
            messageSystem_->Show(_param["text"].get<std::string>());
        }
    }
    else if (_name == "CloseMessageWindow") {
        if (messageSystem_) {
            messageSystem_->Close();
        }
    }
    else if (_name == "MoveCameraToTarget") {
        if (cameraSystem_) {
            cameraSystem_->MoveToTarget();
        }
    }
    else if (_name == "FixPlayer") {
        if (cameraSystem_ && _param.contains("pos") && _param.contains("rot")) {

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

            cameraSystem_->FixToPlayer(pos, rot);
        }
    }
    else if (_name == "ShowMoveTutorialSprite") {
        if (uiSystem_) {
            uiSystem_->ShowMoveTutorial();
        }
    }
    else if (_name == "SetObjective") {

        if (!objectiveSystem_) return;

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

        if (!objectSystem_ || !_param.contains("transforms")) return;

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
