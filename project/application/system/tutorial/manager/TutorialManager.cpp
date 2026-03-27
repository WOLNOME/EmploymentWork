#include "TutorialManager.h"
#include <fstream>

//アプリケーション
#include <application/system/tutorial/command/executor/CommandExecutor.h>
#include <application/system/CameraManager.h>
#include <application/object/character/player/Player.h>
#include <application/ui/player/PlayerUI.h>
#include <application/object/character/item/manager/ItemManager.h>

using namespace Norm;

void TutorialManager::Initialize() {
	//コマンドエグゼキューターの初期化
	commandExecutor_ = std::make_unique<CommandExecutor>();
	commandExecutor_->Initialize();

	sequences_.clear();
	currentSequence_ = nullptr;
	currentStepIndex_ = 0;
	isWaiting_ = false;
	currentWaitCondition_.clear();
}

void TutorialManager::Update() {

	if (!currentSequence_) {
		return;
	}

	//コマンドエグゼキューターの更新
	commandExecutor_->Update();

	// 待機中
	if (isWaiting_) {
		//終了条件のチェック
		if (CheckWaitCondition(currentWaitCondition_)) {
			isWaiting_ = false;
			currentStepIndex_++;
		}
		return;
	}

	// シーケンス終了
	if (currentStepIndex_ >= currentSequence_->steps.size()) {
		// 次のシーケンスを開始
		if (currentSequenceName_ == "tutorial_intro") {
			Start("tutorial_move");
		}
		else {
			currentSequence_ = nullptr;
		}

		return;
	}

	// ステップ実行
	const Step& step = currentSequence_->steps[currentStepIndex_];
	ExecuteStep(step);

	currentWaitCondition_ = step.waitCondition;
	isWaiting_ = true;
}

void TutorialManager::LoadFromFile(const std::string& path) {
	// JsonUtilで読み込み
	std::optional<json> jsonOpt = JsonUtil::GetJsonData(path);

	if (!jsonOpt.has_value()) {
		// 読み込み失敗
		return;
	}

	const json& root = jsonOpt.value();

	// シーケンスごとに処理
	for (auto& [sequenceName, stepArray] : root.items()) {

		Sequence sequence;

		// Step配列
		for (auto& stepJson : stepArray) {

			Step step;

			// cmds
			if (stepJson.contains("cmds")) {

				for (auto& cmdJson : stepJson["cmds"]) {

					CommandData cmd;

					// cmd名
					if (cmdJson.contains("cmd")) {
						cmd.name = cmdJson["cmd"].get<std::string>();
					}

					// params
					for (auto& [key, value] : cmdJson.items()) {
						if (key != "cmd") {
							cmd.params[key] = value;
						}
					}

					step.commands.push_back(cmd);
				}
			}

			// wait
			if (stepJson.contains("wait")) {
				step.waitCondition = stepJson["wait"].get<std::string>();
			}

			sequence.steps.push_back(step);
		}

		sequences_[sequenceName] = sequence;
	}
}

void TutorialManager::Start(const std::string& sequenceName) {

	auto it = sequences_.find(sequenceName);
	if (it == sequences_.end()) {
		return;
	}

	currentSequence_ = &it->second;
	currentSequenceName_ = sequenceName;
	currentStepIndex_ = 0;
	isWaiting_ = false;
	currentWaitCondition_.clear();
}

void TutorialManager::SetCameraManager(CameraManager* cameraManager) {
	commandExecutor_->SetCameraManager(cameraManager);
}

void TutorialManager::SetPlayer(Player* player) {
	commandExecutor_->SetPlayer(player);
}

void TutorialManager::SetPlayerUI(PlayerUI* playerUI) {
	commandExecutor_->SetPlayerUI(playerUI);
}

void TutorialManager::SetItemManager(ItemManager* itemManager) {
	commandExecutor_->SetItemManager(itemManager);
}

void TutorialManager::ExecuteStep(const Step& step) {
	//現在ステップのコマンドを走査
	for (const CommandData& cmd : step.commands) {
		// params を json にまとめる
		json paramJson = json::object();
		for (const auto& [key, value] : cmd.params) {
			paramJson[key] = value;
		}

		// コマンド実行
		commandExecutor_->ExecuteCommand(
			cmd.name,
			paramJson,
			step.waitCondition
		);
	}
}

bool TutorialManager::CheckWaitCondition(const std::string& condition) const {

	if (condition == "MessageWindowOpened") {
		//メッセージウィンドウが開かれていたらtrue
		return commandExecutor_->GetMessageSystem()->GetIsWindowOpened();
	}
	if (condition == "MessageWindowClosed") {
		//メッセージウィンドウが開かれていなかったらtrue
		return commandExecutor_->GetMessageSystem()->GetIsWindowClosed();
	}
	if (condition == "MessageTextFinished") {
		bool flag = commandExecutor_->GetMessageSystem()->GetIsNextAdvance();
		commandExecutor_->GetMessageSystem()->SetIsNextAdvance(false);
		return flag;
	}
	if (condition == "CameraFinished") {
		return !commandExecutor_->GetCameraSystem()->GetIsDirection();
	}
	if (condition == "SpriteFinished") {
		bool flag = commandExecutor_->GetUISystem()->GetIsTutorialSpriteFinished();
		commandExecutor_->GetUISystem()->SetIsTutorialSpriteFinished(false);
		return flag;
	}
	if (condition == "ObjectiveComplete") {
		bool flag = commandExecutor_->GetObjectiveSystem()->CheckAllObjectiveComplete();
		if (flag) {
			commandExecutor_->GetObjectiveSystem()->ClearObjective();
		}
		return flag;
	}
	if (condition == "CommandFinished") {
		return true;
	}

	return false;
}
