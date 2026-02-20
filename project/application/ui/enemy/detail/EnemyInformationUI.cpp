#include "EnemyInformationUI.h"
#include <WinApp.h>
#include <TextTextureManager.h>
#include <TextureManager.h>
#include <SpriteManager.h>
#include <GameCamera.h>
#include <cassert>

//アプリケーション
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
	//カメラチェック
	assert(camera_ != nullptr && "カメラをセットしてください");
	//エネミーマネージャーチェック
	assert(enemyManager_ != nullptr && "エネミーマネージャーをセットしてください");

	//フレームの処理
	FrameProcess();
	//詳細の処理
	DetailProcess();

}

void EnemyInformationUI::FrameProcess() {
	int frameIndex = 0;
	for (int i = 0; i < kNumEnemyFrame; i++) {
		//あらかじめ全て非表示状態にしておく
		spriteEnemyFrame_[i]->SetIsDisplay(false);
	}

	//共通描画用ラムダ式
	auto drawEnemyFrame = [&](const Vector3& worldPos) {
		//フレームの番号が最大値を超えていたら描画しない
		if (frameIndex >= kNumEnemyFrame) return;

		Vector3 clipPos = MyMath::Transform(worldPos, camera_->GetViewProjectionMatrix());
		//クリップ座標のZ値が0未満または1より大きいなら描画しない
		if (clipPos.z < 0.0f || clipPos.z > 1.0f) return;

		Vector2 screenPos = {
			(clipPos.x * 0.5f + 0.5f) * WinApp::kClientWidth,
			(clipPos.y * -0.5f + 0.5f) * WinApp::kClientHeight
		};

		//スクリーン座標が画面範囲から外れていたら描画しない
		if (screenPos.x < 0.0f || screenPos.x > WinApp::kClientWidth ||
			screenPos.y < 0.0f || screenPos.y > WinApp::kClientHeight) return;

		//非表示状態を解除
		spriteEnemyFrame_[frameIndex]->SetIsDisplay(true);
		spriteEnemyFrame_[frameIndex]->SetIsDisplay(true);
		//パラメーターを代入
		spriteEnemyFrame_[frameIndex]->SetPosition(screenPos);
		spriteEnemyFrame_[frameIndex]->SetPosition(screenPos);

		frameIndex++;	//←先着方式
		};

	//キャノ太
	for (const auto& canota : enemyManager_->GetCanotas()) {
		//アクティブでないなら描画しない
		if (canota->GetState() != BaseCharacter::State::kActive) continue;

		Vector3 pos = canota->GetWorldTransform().GetWorldTranslate();
		drawEnemyFrame(pos);
	}

	//キーキャノ太
	for (const auto& keyCanota : enemyManager_->GetKeyCanotas()) {
		//死亡していたら描画しない
		if (keyCanota->GetState() != BaseCharacter::State::kActive) continue;

		Vector3 pos = keyCanota->GetWorldTransform().GetWorldTranslate();
		drawEnemyFrame(pos);
	}

	//ジェット
	for (const auto& jet : enemyManager_->GetJets()) {
		//死亡していたら描画しない
		if (jet->GetState() != BaseCharacter::State::kActive)
			continue;

		Vector3 pos = jet->GetWorldTransform().GetWorldTranslate();
		drawEnemyFrame(pos);
	}

	//ボス
	for (const auto& boss : enemyManager_->GetBosses()) {
		//死亡していたら描画しない
		if (boss->GetState() != BaseCharacter::State::kActive)
			continue;

		Vector3 pos = boss->GetWorldTransform().GetWorldTranslate();
		drawEnemyFrame(pos);
	}
}

void EnemyInformationUI::DetailProcess() {
	Vector2 baseOffset = { 150.0f, -90.0f };
	Vector2 levelOffset = { 317,-165 };
	Vector2 rangeOffset = { 317,-105 };
	Vector2 itemOffset = { 317,-45 };
	float distance = FLT_MAX;
	float drawDistanceMax = 100.0f;	//描画する最大の距離

	//あらかじめ全て非表示状態にしておく
	spriteInfoBase_->SetIsDisplay(false);
	spriteLevel_->SetIsDisplay(false);
	spriteRange_->SetIsDisplay(false);
	for (int i = 0; i < kNumItem; i++) {
		spriteItem_[i]->SetIsDisplay(false);
	}

	//共通描画用ラムダ式
	auto drawEnemyDetail = [&](const Vector3& worldPos, const std::string& type) {
		Vector3 clipPos = MyMath::Transform(worldPos, camera_->GetViewProjectionMatrix());
		//クリップ座標のZ値が0未満または1より大きいなら描画しない
		if (clipPos.z < 0.0f || clipPos.z > 1.0f) return;

		//スクリーン座標を求める
		Vector2 screenPos = {
			(clipPos.x * 0.5f + 0.5f) * WinApp::kClientWidth,
			(clipPos.y * -0.5f + 0.5f) * WinApp::kClientHeight
		};

		//スクリーン座標が画面範囲から外れていたら描画しない
		if (screenPos.x < 0.0f || screenPos.x > WinApp::kClientWidth ||
			screenPos.y < 0.0f || screenPos.y > WinApp::kClientHeight) return;

		//カメラと対象の座標の距離を計測
		float cameraToSubjectLength = Vector3(camera_->worldTransform.GetWorldTranslate() - worldPos).Length();
		//最大描画距離内になければ描画しない
		if (cameraToSubjectLength > drawDistanceMax) return;
		//すでに描画した対象との距離より離れていたら描画しない
		if (cameraToSubjectLength > distance) return;


		//非表示状態を解除
		spriteInfoBase_->SetIsDisplay(true);
		spriteLevel_->SetIsDisplay(true);
		spriteRange_->SetIsDisplay(true);
		//アイテムのみ非表示
		for (int i = 0; i < kNumItem; i++) {
			spriteItem_[i]->SetIsDisplay(false);
		}

		//基盤の設定
		{
			spriteInfoBase_->SetPosition(screenPos + baseOffset);
		}
		//レベルの設定
		{
			if (type == "canota") {
				spriteLevel_->SetUVScrollSheetNum(0);
			}
			else if (type == "keyCanota") {
				spriteLevel_->SetUVScrollSheetNum(1);
			}
			else if (type == "jet") {
				spriteLevel_->SetUVScrollSheetNum(2);
			}
			else if (type == "boss") {
				spriteLevel_->SetUVScrollSheetNum(3);
			}
			spriteLevel_->SetPosition(screenPos + levelOffset);
		}
		//距離の設定
		{
			TextTextureManager::GetInstance()->EditTextString(thRange_, L"{:03.0f}m", cameraToSubjectLength);
			spriteRange_->SetPosition(screenPos + rangeOffset);
		}
		//アイテムの設定
		{
			if (type == "canota") {
				//テクスチャを変える
				spriteItem_[0]->SetTexture(TextureManager::GetInstance()->LoadTexture("healIcon.png"));
				spriteItem_[1]->SetTexture(TextureManager::GetInstance()->LoadTexture("specialIcon.png"));
				//座標をセット
				spriteItem_[0]->SetPosition({ screenPos.x + itemOffset.x,screenPos.y + itemOffset.y });
				spriteItem_[1]->SetPosition({ screenPos.x + itemOffset.x + 60.0f,screenPos.y + itemOffset.y });
				//表示
				spriteItem_[0]->SetIsDisplay(true);
				spriteItem_[1]->SetIsDisplay(true);
			}
			else if (type == "keyCanota") {

			}
			else if (type == "jet") {

			}
			else if (type == "boss") {

			}
		}


		//反映した対象の距離を更新
		distance = cameraToSubjectLength;

		};

	//キャノ太
	for (const auto& canota : enemyManager_->GetCanotas()) {
		//アクティブでないなら描画しない
		if (canota->GetState() != BaseCharacter::State::kActive) continue;

		Vector3 pos = canota->GetWorldTransform().GetWorldTranslate();
		drawEnemyFrame(pos);
	}

	//キーキャノ太
	for (const auto& keyCanota : enemyManager_->GetKeyCanotas()) {
		//死亡していたら描画しない
		if (keyCanota->GetState() != BaseCharacter::State::kActive) continue;

		Vector3 pos = keyCanota->GetWorldTransform().GetWorldTranslate();
		drawEnemyFrame(pos);
	}

	//ジェット
	for (const auto& jet : enemyManager_->GetJets()) {
		//死亡していたら描画しない
		if (jet->GetState() != BaseCharacter::State::kActive)
			continue;

		Vector3 pos = jet->GetWorldTransform().GetWorldTranslate();
		drawEnemyFrame(pos);
	}

	//ボス
	for (const auto& boss : enemyManager_->GetBosses()) {
		//死亡していたら描画しない
		if (boss->GetState() != BaseCharacter::State::kActive)
			continue;

		Vector3 pos = boss->GetWorldTransform().GetWorldTranslate();
		drawEnemyFrame(pos);
	}
}
