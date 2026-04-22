#include "EmphasisUI.h"
#include <WinApp.h>
#include <SpriteManager.h>
#include <TextureManager.h>
#include <ImGuiManager.h>
#include <BaseCamera.h>
#include <cassert>

//アプリケーション
#include <application/system/CameraManager.h>
#include <application/object/character/enemy/manager/EnemyManager.h>
#include <application/object/character/item/manager/ItemManager.h>

using namespace Norm;


void EmphasisUI::Initialize() {
	//jsonファイルの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerUI");

	//強調アイコン
	{
		//テクスチャハンドル
		thIcon_ = TextureManager::GetInstance()->LoadTexture("emphasisIcon.png");

		for (int i = 0; i < kIconNum_; i++) {
			icons_[i] = std::make_unique<Sprite>();
			icons_[i]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("emphasisIcon"), Order::Back1, thIcon_);
			icons_[i]->SetAnchorPoint({ 0.5f,0.5f });
		}
	}
}

void EmphasisUI::Update() {
	//カメラマネージャーがセットされていなければ警告
	assert(cameraManager_ != nullptr && "RadarにCameraManagerインスタンスを渡してください");
	//エネミーマネージャーがセットされていなければ警告
	assert(enemyManager_ != nullptr && "RadarにEnemyManagerインスタンスを渡してください");
	//アイテムマネージャーがセットされていなければ警告
	assert(itemManager_ != nullptr && "RadarにItemManagerインスタンスを渡してください");

	//強調アイコンの更新処理
	UpdateIcon();


}

void EmphasisUI::UpdateIcon() {
	const float drawDistanceMax =
		param_["emphasis"]["drawDistanceMax"].get<float>();
	//強調アイコン付与ラムダ
	auto processAttachEmphasisIcon = [&](const Vector3 subjectPos, const std::string& type, int& spriteIndex) {
		//タイプによって高さを変える
		float height3d = 0.0f;
		if (type == "keyCanota") {
			height3d = param_["emphasis"]["iconHeight3D"]["keyCanota"].get<float>();
		}
		else if (type == "boss") {
			height3d = param_["emphasis"]["iconHeight3D"]["boss"].get<float>();
		}
		else if (type == "key") {
			height3d = param_["emphasis"]["iconHeight3D"]["key"].get<float>();
		}
		else if (type == "tutorialItem") {
			height3d = param_["emphasis"]["iconHeight3D"]["tutorialItem"].get<float>();
		}
		//ワールド座標を作る（高さを加算）
		Vector3 worldPos = subjectPos;
		worldPos.y += height3d;

		//座標変換
		Vector3 clipPos =
			MyMath::Transform(worldPos, cameraManager_->GetActiveCamera()->GetViewProjectionMatrix());

		if (clipPos.z < 0.0f || clipPos.z > 1.0f)
			return;

		Vector2 screenPos = {
			(clipPos.x * 0.5f + 0.5f) * WinApp::kClientWidth,
			(clipPos.y * -0.5f + 0.5f) * WinApp::kClientHeight
		};

		if (screenPos.x < 0.0f || screenPos.x > WinApp::kClientWidth ||
			screenPos.y < 0.0f || screenPos.y > WinApp::kClientHeight)
			return;

		float distance =
			(cameraManager_->GetActiveCamera()->worldTransform.GetWorldTranslate() - worldPos).Length();

		if (distance > drawDistanceMax) return;

		//パラメーター反映
		icons_[spriteIndex]->SetIsDisplay(true);
		icons_[spriteIndex]->SetPosition(screenPos);

		spriteIndex++;
		};

	//全スプライトを非表示に
	for (int i = 0; i < kIconNum_; i++) {
		icons_[i]->SetIsDisplay(false);
	}

	//スプライトインデックス
	int spriteIndex = 0;

	//キーキャノ太
	for (const auto& keyCanota : enemyManager_->GetKeyCanotas()) {
		//生存状態でないなら次へ
		if (keyCanota->GetState() != BaseCharacter::State::kActive) {
			continue;
		}

		processAttachEmphasisIcon(keyCanota->GetWorldTransform().GetTranslate(), "keyCanota", spriteIndex);
	}
	//ボス
	for (const auto& boss : enemyManager_->GetBosses()) {
		//生存状態でないなら次へ
		if (boss->GetState() != BaseCharacter::State::kActive) {
			continue;
		}

		processAttachEmphasisIcon(boss->GetWorldTransform().GetTranslate(), "boss", spriteIndex);
	}
	//鍵
	for (const auto& key : itemManager_->GetKeyItems()) {
		//生存状態でないなら次へ
		if (key->GetState() != BaseCharacter::State::kActive) {
			continue;
		}

		processAttachEmphasisIcon(key->GetWorldTransform().GetTranslate(), "key", spriteIndex);
	}
	//チュートリアルアイテム
	for (const auto& tutorialItem : itemManager_->GetTutorialCollectibleItmes()) {
		//生存状態でないなら次へ
		if (tutorialItem->GetState() != BaseCharacter::State::kActive) {
			continue;
		}

		processAttachEmphasisIcon(tutorialItem->GetWorldTransform().GetTranslate(), "tutorialItem", spriteIndex);
	}
}

void EmphasisUI::SetIsDisplay(bool _isDisplay) {
	for (auto& icon : icons_) {
		if (icon) {
			icon->SetIsDisplay(_isDisplay);
		}
	}
}