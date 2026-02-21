#include "EnemyInformationUI.h"
#include <WinApp.h>
#include <TextTextureManager.h>
#include <TextureManager.h>
#include <SpriteManager.h>
#include <cassert>

//アプリケーション
#include <application/system/CameraManager.h>
#include <application/object/character/enemy/manager/EnemyManager.h>

using namespace Norm;

void EnemyInformationUI::Initialize() {
	//パラメータ読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/enemyUI");

	//敵フレームの初期化
	{
		thEnemyFrame_ = TextureManager::GetInstance()->LoadTexture("enemyFrameUI.png");
		for (int i = 0; i < kNumEnemyFrame; i++) {
			spriteEnemyFrame_[i] = std::make_unique<Sprite>();
			spriteEnemyFrame_[i]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("EnemyFrame"), Order::Back1, thEnemyFrame_);
			spriteEnemyFrame_[i]->SetAnchorPoint({ 0.5f,0.5f });
			spriteEnemyFrame_[i]->SetIsDisplay(false);
		}
	}
	//詳細情報基盤の初期化
	{
		thInfoBase_ = TextureManager::GetInstance()->LoadTexture("infoBase.png");
		spriteInfoBase_ = std::make_unique<Sprite>();
		spriteInfoBase_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("InfoBase"), Order::Back0, thInfoBase_);
		spriteInfoBase_->SetAnchorPoint({ 0.5f,0.5f });
		spriteInfoBase_->SetIsDisplay(false);
	}
	//レベル情報の初期化
	{
		thInfoBase_ = TextureManager::GetInstance()->LoadTexture("level.png");
		spriteLevel_ = std::make_unique<Sprite>();
		spriteLevel_->Initialize(UVScrollTag{}, SpriteManager::GetInstance()->GenerateName("level"), Order::Back0, 4, 0.01f, false, thInfoBase_);
		spriteLevel_->SetAnchorPoint({ 0.0f,0.0f });
		spriteLevel_->SetIsDisplay(false);
	}
	//距離情報の初期化
	{
		//テキストパラメーターを設定
		TextParam textParam = {
			.text = L"",
			.font = Font::UDDegitalN_B,
			.fontStyle = FontStyle::Normal,
			.size = 32.0f,
			.color = Vector4(0.224f,0.769f,0.584f,1.0f),
		};
		thRange_ = TextTextureManager::GetInstance()->LoadTextTexture(textParam);
		spriteRange_ = std::make_unique<Sprite>();
		spriteRange_->Initialize(TextTag{}, SpriteManager::GetInstance()->GenerateName("rangeText"), Order::Back0);
		spriteRange_->SetAnchorPoint({ 0.5f,0.5f });
		spriteRange_->SetTexture(thRange_);
		spriteRange_->SetIsDisplay(false);
	}
	//アイテム情報の初期化
	{
		thItem_ = TextureManager::GetInstance()->LoadTexture("cross.png");
		for (int i = 0; i < kNumItem; i++) {
			spriteItem_[i] = std::make_unique<Sprite>();
			spriteItem_[i]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("ItemIcon"), Order::Back0, thItem_);
			spriteItem_[i]->SetAnchorPoint({ 0.5f,0.5f });
			spriteItem_[i]->SetIsDisplay(false);
		}
	}

}

void EnemyInformationUI::Update() {
	//カメラマネージャーチェック
	assert(cameraManager_ && "カメラマネージャーをセットしてください");
	//エネミーマネージャーチェック
	assert(enemyManager_ != nullptr && "エネミーマネージャーをセットしてください");

	//情報処理
	InformationProcess();

}

void EnemyInformationUI::InformationProcess() {
	const float frameDrawDistanceMax =
		param_["information"]["frameDrawDistanceMax"].get<float>();
	const float detailDrawDistanceMax =
		param_["information"]["detailDrawDistanceMax"].get<float>();
	const float bulletDistance =
		param_["information"]["bulletDistance"].get<float>();
	Vector2 baseOffset = {
		param_["information"]["baseOffset"]["x"].get<float>(),
		param_["information"]["baseOffset"]["y"].get<float>()
	};
	Vector2 levelOffset = {
		param_["information"]["levelOffset"]["x"].get<float>(),
		param_["information"]["levelOffset"]["y"].get<float>()
	};
	Vector2 rangeOffset = {
		param_["information"]["rangeOffset"]["x"].get<float>(),
		param_["information"]["rangeOffset"]["y"].get<float>()
	};
	Vector2 itemOffset = {
		param_["information"]["itemOffset"]["x"].get<float>(),
		param_["information"]["itemOffset"]["y"].get<float>()
	};

	// ---- 初期化 ----

	for (int i = 0; i < kNumEnemyFrame; i++) {
		spriteEnemyFrame_[i]->SetIsDisplay(false);
	}

	spriteInfoBase_->SetIsDisplay(false);
	spriteLevel_->SetIsDisplay(false);
	spriteRange_->SetIsDisplay(false);

	for (int i = 0; i < kNumItem; i++) {
		spriteItem_[i]->SetIsDisplay(false);
	}

	int frameIndex = 0;
	float nearestDistance = FLT_MAX;

	// 共通ラムダ
	auto processEnemy = [&](auto& container, const std::string& type) {

		for (const auto& enemy : container) {

			if (enemy->GetState() != BaseCharacter::State::kActive)
				continue;

			Vector3 worldPos = enemy->GetWorldTransform().GetWorldTranslate();

			// ---- 座標変換 ----

			Vector3 clipPos =
				MyMath::Transform(worldPos, cameraManager_->GetActiveCamera()->GetViewProjectionMatrix());

			if (clipPos.z < 0.0f || clipPos.z > 1.0f)
				continue;

			Vector2 screenPos = {
				(clipPos.x * 0.5f + 0.5f) * WinApp::kClientWidth,
				(clipPos.y * -0.5f + 0.5f) * WinApp::kClientHeight
			};

			if (screenPos.x < 0.0f || screenPos.x > WinApp::kClientWidth ||
				screenPos.y < 0.0f || screenPos.y > WinApp::kClientHeight)
				continue;

			float distance =
				(cameraManager_->GetActiveCamera()->worldTransform.GetWorldTranslate() - worldPos).Length();

			// ===================================================
			// フレームUI処理
			// ===================================================

			if (distance <= frameDrawDistanceMax &&
				frameIndex < kNumEnemyFrame) {
				spriteEnemyFrame_[frameIndex]->SetIsDisplay(true);
				spriteEnemyFrame_[frameIndex]->SetPosition(screenPos);

				Vector4 color =
					(distance <= bulletDistance)
					? Vector4(1, 0, 0, 1)
					: Vector4(0.224f, 0.769f, 0.584f, 1.0f);

				spriteEnemyFrame_[frameIndex]->SetColor(color);

				frameIndex++;
			}

			// ===================================================
			// 詳細UI処理（最も近い敵のみ）
			// ===================================================

			if (distance > detailDrawDistanceMax)
				continue;

			if (distance >= nearestDistance)
				continue;

			nearestDistance = distance;

			spriteInfoBase_->SetIsDisplay(true);
			spriteLevel_->SetIsDisplay(true);
			spriteRange_->SetIsDisplay(true);

			spriteInfoBase_->SetPosition(screenPos + baseOffset);
			spriteLevel_->SetPosition(screenPos + levelOffset);
			spriteRange_->SetPosition(screenPos + rangeOffset);

			// ---- レベル表示 ----

			if (type == "canota") {
				spriteLevel_->SetUVScrollSheetNum(0);
			}
			else if (type == "jet") {
				spriteLevel_->SetUVScrollSheetNum(1);
			}
			else if (type == "keyCanota") {
				spriteLevel_->SetUVScrollSheetNum(2);
			}
			else if (type == "boss") {
				spriteLevel_->SetUVScrollSheetNum(3);
			}

			TextTextureManager::GetInstance()
				->EditTextString(thRange_, L"{:03.0f}m", distance);

			// ---- アイテム表示 ----

			for (int i = 0; i < kNumItem; i++) {
				spriteItem_[i]->SetIsDisplay(false);
			}

			if (type == "canota") {
				spriteItem_[0]->SetTexture(TextureManager::GetInstance()->LoadTexture("healIcon.png"));
				spriteItem_[1]->SetTexture(TextureManager::GetInstance()->LoadTexture("specialIcon.png"));

				spriteItem_[0]->SetPosition({ screenPos.x + itemOffset.x, screenPos.y + itemOffset.y });
				spriteItem_[1]->SetPosition({ screenPos.x + itemOffset.x + 60.0f, screenPos.y + itemOffset.y });

				spriteItem_[0]->SetIsDisplay(true);
				spriteItem_[1]->SetIsDisplay(true);
			}
			else if (type == "keyCanota") {
				spriteItem_[0]->SetTexture(TextureManager::GetInstance()->LoadTexture("keyIcon.png"));
				spriteItem_[0]->SetPosition({ screenPos.x + itemOffset.x, screenPos.y + itemOffset.y });
				spriteItem_[0]->SetIsDisplay(true);
			}
			else {
				spriteItem_[0]->SetTexture(TextureManager::GetInstance()->LoadTexture("cross.png"));
				spriteItem_[0]->SetPosition({ screenPos.x + itemOffset.x, screenPos.y + itemOffset.y });
				spriteItem_[0]->SetIsDisplay(true);
			}
		}
		};

	// ---- 敵処理 ----

	processEnemy(enemyManager_->GetCanotas(), "canota");
	processEnemy(enemyManager_->GetKeyCanotas(), "keyCanota");
	processEnemy(enemyManager_->GetJets(), "jet");
	processEnemy(enemyManager_->GetBosses(), "boss");
}
