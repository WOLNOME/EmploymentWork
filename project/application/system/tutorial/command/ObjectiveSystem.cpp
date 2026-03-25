#include "ObjectiveSystem.h"
#include <TextureManager.h>
#include <TextTextureManager.h>
#include <SpriteManager.h>
#include <StringUtility.h>
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>

using namespace Norm;

void ObjectiveSystem::Initialize() {

	const Vector2 basePos = { 50.0f, 100.0f };
	const float offsetY = 40.0f;

	for (size_t i = 0; i < objectives_.size(); ++i) {

		Objective& obj = objectives_[i];

		obj.type = ObjectiveType::None;
		obj.current = 0;
		obj.target = 0;
		obj.completed = false;

		Vector2 pos = {
			basePos.x,
			basePos.y + offsetY * static_cast<float>(i)
		};

		// チェックマーク生成
		{
			uint32_t tex =
				TextureManager::GetInstance()->LoadTexture("checkMark.png");

			obj.chcekMark = std::make_unique<Sprite>();

			obj.chcekMark->Initialize(
				UVScrollTag{},
				SpriteManager::GetInstance()->GenerateName("objectiveCheck"),
				Order::Front5,
				10,
				0.1f,
				false,
				tex
			);

			obj.chcekMark->SetUVScrollSheetNum(0);
			obj.chcekMark->SetAnchorPoint({ 0.5f, 0.5f });
			obj.chcekMark->SetPosition({ pos.x - 30.0f, pos.y });
			obj.chcekMark->SetIsDisplay(false);
		}

		// テキスト生成
		{
			TextParam param;
			param.text = L"";
			param.font = Font::UDDegitalNP_B;
			param.fontStyle = FontStyle::Normal;
			param.size = 24.0f;
			param.color = { 1,1,1,1 };
			EdgeParam edge;
			edge.isEdgeDisplay = true;
			edge.width = 1.0f;
			edge.slideRate = { 0,0 };
			edge.color = { 0,0,0,1 };

			obj.textHandle =
				TextTextureManager::GetInstance()->LoadTextTexture(param);
			TextTextureManager::GetInstance()->EditEdgeParam(obj.textHandle, edge);

			obj.text = std::make_unique<Sprite>();
			obj.text->Initialize(
				TextTag{},
				SpriteManager::GetInstance()->GenerateName("objectiveText"),
				Order::Front5
			);

			obj.text->SetTexture(obj.textHandle);
			obj.text->SetAnchorPoint({ 0.0f, 0.5f });
			obj.text->SetPosition(pos);
			obj.text->SetIsDisplay(false);
		}
	}
}

void ObjectiveSystem::Update() {
	//プレイヤーチェック
	assert(player_ && "プレイヤーがセットされていません");


	for (auto& obj : objectives_) {

		switch (obj.type) {
		case ObjectiveType::None:
		{
			// 何も表示しない
			break;
		}
		case ObjectiveType::CollectItem:
		{
			//プレイヤーの獲得済みの収集物を取得
			obj.current = player_->GetTutorialItemNum();
			//テキストを更新
			TextTextureManager::GetInstance()->EditTextString(obj.textHandle, L"アイテムを集めよう！（{}/{}）", obj.current, obj.target);
			//目標数に達したら
			if (obj.current >= obj.target) {
				//アニメーションが終了していたら稼働しない
				if (!obj.chcekMark->GetIsFinishedUVScroll()) {
					//チェックのスプライトアニメーションを稼働
					obj.chcekMark->SetIsPlayUVScroll(true);
				}
				else {
					//目標の達成フラグを立てる
					obj.completed = true;
				}
			}

			break;
		}
		default:
			break;
		}
	}
}

bool ObjectiveSystem::CheckAllObjectiveComplete() {
	bool hasActiveObjective = false;

	for (const auto& obj : objectives_) {

		if (obj.type == ObjectiveType::None)
			continue;

		hasActiveObjective = true;

		if (!obj.completed) {
			return false; //1つでも未達成があればfalse
		}
	}

	//有効な目標が存在し、かつ全て完了しているならtrue
	return hasActiveObjective;
}

void ObjectiveSystem::ClearObjective() {

	for (auto& obj : objectives_) {

		obj.type = ObjectiveType::None;
		obj.current = 0;
		obj.target = 0;
		obj.completed = false;

		obj.text->SetIsDisplay(false);
		obj.chcekMark->SetIsDisplay(false);
		obj.chcekMark->SetIsFinishedUVScroll(false);

	}
}

void ObjectiveSystem::AddCollectObjective(int target) {

	for (auto& obj : objectives_) {

		if (obj.type != ObjectiveType::None)
			continue;

		obj.type = ObjectiveType::CollectItem;
		obj.target = target;
		obj.current = 0;
		obj.completed = false;
		
		obj.text->SetIsDisplay(true);
		obj.chcekMark->SetIsDisplay(true);

		return;
	}
}
