#include "CommandExecutor.h"
#include <cassert>
#include <Vector3.h>
#include <TextureManager.h>
#include <StringUtility.h>

//アプリケーション
#include <application/system/CameraManager.h>
#include <application/object/character/player/Player.h>
#include <application/ui/player/PlayerUI.h>
#include <application/object/character/item/manager/ItemManager.h>

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

void CommandExecutor::Update() {
	//各システムの更新
	messageSystem_->Update();
	cameraSystem_->Update();
	uiSystem_->Update();
	objectiveSystem_->Update();
	objectSystem_->Update();
}

void CommandExecutor::ExecuteCommand(const std::string& _name, const json& _param, const std::string& _waitType) {
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
		bool isManualNext = (_waitType == "MessageTextFinished");
		std::wstring text = StringUtility::ConvertString(_param["text"].get<std::string>());
		messageSystem_->ShowText(text, isManualNext);
	}
	else if (_name == "OpenMessageWindow") {
		messageSystem_->OpenWindow();
	}
	else if (_name == "CloseMessageWindow") {
		messageSystem_->CloseWindow();
	}
	else if (_name == "MoveCameraToTarget") {

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

		cameraSystem_->SetTargetTransform(pos, rot);
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
		uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("support.png");
		uiSystem_->ShowSprite(textureHandle);
	}
	else if (_name == "SetObjective") {
		std::string type = _param["type"];

		//タイプごとの処理
		if (type == "collect") {
			objectiveSystem_->AddCollectObjective(_param["count"].get<int>());
		}
	}
	else if (_name == "SpawnItem") {

		const auto& transforms = _param["transforms"];

		for (const auto& t : transforms) {

			Vector3 pos = {
				t["pos"][0].get<float>(),
				t["pos"][1].get<float>(),
				t["pos"][2].get<float>()
			};

			objectSystem_->SpawnItem(pos);
		}
	}
}

void CommandExecutor::SetCameraManager(CameraManager* cameraManager) {
	cameraSystem_->SetCameraManager(cameraManager);
}

void CommandExecutor::SetPlayer(Player* player) {
	inputSystem_->SetPlayer(player);
	cameraSystem_->SetPlayer(player);
	objectiveSystem_->SetPlayer(player);
	objectSystem_->SetPlayer(player);
}

void CommandExecutor::SetPlayerUI(PlayerUI* playerUI) {
	cameraSystem_->SetPlayerUI(playerUI);
}

void CommandExecutor::SetItemManager(ItemManager* itemManager) {
	objectSystem_->SetItemManager(itemManager);
}
