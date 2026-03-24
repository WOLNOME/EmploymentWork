#include "ObjectiveSystem.h"
#include <TextureManager.h>
#include <TextTextureManager.h>
#include <SpriteManager.h>
#include <StringUtility.h>

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
				TextureManager::GetInstance()->LoadTexture("check.png");

			obj.chcekMark = std::make_unique<Sprite>();

			obj.chcekMark->Initialize(
				SpriteTag{},
				SpriteManager::GetInstance()->GenerateName("objectiveCheck"),
				Order::Front5,
				tex
			);

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

			obj.textHandle =
				TextTextureManager::GetInstance()->LoadTextTexture(param);

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

	for (auto& obj : objectives_) {

		switch (obj.type) {

		case ObjectiveType::None:
			// 何も表示しない
			break;

		case ObjectiveType::CollectItem:
		{
			// 完了時チェック表示
			if (obj.completed) {
				obj.chcekMark->SetIsDisplay(true);
			}
			break;
		}

		default:
			break;
		}
	}
}

void ObjectiveSystem::ClearObjective() {

	for (auto& obj : objectives_) {

		obj.type = ObjectiveType::None;
		obj.current = 0;
		obj.target = 0;
		obj.completed = false;

		obj.text->SetIsDisplay(false);
		obj.chcekMark->SetIsDisplay(false);

		TextTextureManager::GetInstance()->EditTextString(
			obj.textHandle,
			L"");
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

		std::wstring text =
			L"アイテム収集 0/" + std::to_wstring(target);

		TextTextureManager::GetInstance()->EditTextString(
			obj.textHandle,
			text);

		obj.text->SetIsDisplay(true);
		obj.chcekMark->SetIsDisplay(false);

		return;
	}
}
