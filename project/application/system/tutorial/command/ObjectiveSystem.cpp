#include "ObjectiveSystem.h"
#include <TextureManager.h>
#include <TextTextureManager.h>
#include <SpriteManager.h>
#include <StringUtility.h>
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>
#include <application/object/character/weapon/player/manager/PlayerWeaponManager.h>
#include <application/object/character/enemy/manager/EnemyManager.h>

using namespace Norm;

void ObjectiveSystem::Initialize() {

	const Vector2 basePos = { 920.0f, 200.0f };
	const float offsetY = 40.0f;

	for (size_t i = 0; i < objectives_.size(); ++i) {

		Objective& obj = objectives_[i];

		obj.type = ObjectiveType::None;
		obj.current = 0;
		obj.targetCount = 0;
		obj.completed = false;

		Vector2 pos = {
			basePos.x,
			basePos.y + offsetY * static_cast<float>(i)
		};

		// チェックマーク生成
		{
			uint32_t tex =
				TextureManager::GetInstance()->LoadTexture("checkMark.png");

			obj.checkMark = std::make_unique<Sprite>();

			obj.checkMark->Initialize(
				UVScrollTag{},
				SpriteManager::GetInstance()->GenerateName("objectiveCheck"),
				Order::Front5,
				10,
				0.05f,
				false,
				tex
			);

			obj.checkMark->SetUVScrollSheetNum(0);
			obj.checkMark->SetAnchorPoint({ 0.5f, 0.5f });
			obj.checkMark->SetPosition({ pos.x - 30.0f, pos.y });
			obj.checkMark->SetIsDisplay(false);
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
	assert(playerWeaponManager_ && "プレイヤー武器マネージャーがセットされていません");
	assert(enemyManager_ && "敵マネージャーがセットされていません");

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
			TextTextureManager::GetInstance()->EditTextString(obj.textHandle, L"アイテムを集めよう！（{}/{}）", obj.current, obj.targetCount);
			//目標数に達したら
			if (obj.current >= obj.targetCount) {
				//アニメーションが終了していたら稼働しない
				if (!obj.checkMark->GetIsFinishedUVScroll()) {
					//チェックのスプライトアニメーションを稼働
					obj.checkMark->SetIsPlayUVScroll(true);
				}
				else {
					//目標の達成フラグを立てる
					obj.completed = true;
				}
			}

			break;
		}
		case ObjectiveType::CollectSpecial:
		{
			//プレイヤーの獲得済みの収集物を取得
			obj.current = player_->GetSpecialNum();
			//テキストを更新
			TextTextureManager::GetInstance()->EditTextString(obj.textHandle, L"スペシャル弾を集めよう！（{}/{}）", obj.current, obj.targetCount);
			//目標数に達したら
			if (obj.current >= obj.targetCount) {
				//アニメーションが終了していたら稼働しない
				if (!obj.checkMark->GetIsFinishedUVScroll()) {
					//チェックのスプライトアニメーションを稼働
					obj.checkMark->SetIsPlayUVScroll(true);
				}
				else {
					//目標の達成フラグを立てる
					obj.completed = true;
				}
			}
			break;
		}
		case ObjectiveType::HitCannon:
		{
			//現在のヒット数を取得
			obj.current = playerWeaponManager_->GetCannonHitNum();

			//テキストを更新
			TextTextureManager::GetInstance()->EditTextString(obj.textHandle, L"砲弾を当てよう！（{}/{}）", obj.current, obj.targetCount);
			//目標数に達したら
			if (obj.current >= obj.targetCount) {
				//アニメーションが終了していたら稼働しない
				if (!obj.checkMark->GetIsFinishedUVScroll()) {
					//チェックのスプライトアニメーションを稼働
					obj.checkMark->SetIsPlayUVScroll(true);
				}
				else {
					//目標の達成フラグを立てる
					obj.completed = true;
				}
			}
			break;
		}
		case ObjectiveType::HitBullet:
		{
			//現在のヒット数を取得
			obj.current = playerWeaponManager_->GetBulletHitNum();

			//テキストを更新
			TextTextureManager::GetInstance()->EditTextString(obj.textHandle, L"銃弾を当てよう！（{}/{}）", obj.current, obj.targetCount);
			//目標数に達したら
			if (obj.current >= obj.targetCount) {
				//アニメーションが終了していたら稼働しない
				if (!obj.checkMark->GetIsFinishedUVScroll()) {
					//チェックのスプライトアニメーションを稼働
					obj.checkMark->SetIsPlayUVScroll(true);
				}
				else {
					//目標の達成フラグを立てる
					obj.completed = true;
				}
			}
			break;
		}
		case ObjectiveType::DefeatEnemy:
		{
			//敵マネージャーから生存中の敵の数を取得
			int aliveCount = 0;
			for (const auto& subject : enemyManager_->GetCanotas()) {
				//アイドル状態もしくは仮死状態なら抜ける
				if (subject->GetState() == BaseCharacter::State::kIdle ||
					subject->GetState() == BaseCharacter::State::kAsphyxia)
					continue;

				//生存中の敵の数をカウント
				aliveCount++;
			}
			for (const auto& subject : enemyManager_->GetKeyCanotas()) {
				//アイドル状態もしくは仮死状態なら抜ける
				if (subject->GetState() == BaseCharacter::State::kIdle ||
					subject->GetState() == BaseCharacter::State::kAsphyxia)
					continue;

				//生存中の敵の数をカウント
				aliveCount++;
			}
			for (const auto& subject : enemyManager_->GetJets()) {
				//アイドル状態もしくは仮死状態なら抜ける
				if (subject->GetState() == BaseCharacter::State::kIdle ||
					subject->GetState() == BaseCharacter::State::kAsphyxia)
					continue;

				//生存中の敵の数をカウント
				aliveCount++;
			}
			for (const auto& subject : enemyManager_->GetBosses()) {
				//アイドル状態もしくは仮死状態なら抜ける
				if (subject->GetState() == BaseCharacter::State::kIdle ||
					subject->GetState() == BaseCharacter::State::kAsphyxia)
					continue;

				//生存中の敵の数をカウント
				aliveCount++;
			}
			//テキストを更新
			TextTextureManager::GetInstance()->EditTextString(obj.textHandle, L"敵を倒せ！");
			//全ての敵が倒されていたら
			if (aliveCount <= 0) {
				//アニメーションが終了していたら稼働しない
				if (!obj.checkMark->GetIsFinishedUVScroll()) {
					//チェックのスプライトアニメーションを稼働
					obj.checkMark->SetIsPlayUVScroll(true);
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
		obj.targetCount = 0;
		obj.completed = false;

		obj.text->SetIsDisplay(false);
		obj.checkMark->SetIsDisplay(false);
		obj.checkMark->SetIsFinishedUVScroll(false);
		obj.checkMark->SetIsPlayUVScroll(false);
		obj.checkMark->SetUVScrollSheetNum(0);

		TextTextureManager::GetInstance()->EditTextString(obj.textHandle, L"");
	}
}

void ObjectiveSystem::AddCollectObjective(const std::string& type, int targetCount) {

	if (type == "Item") {
		//プレイヤーの獲得アイテム数を0にリセット
		player_->SetTutorialItemNum(0);

		for (auto& obj : objectives_) {

			if (obj.type != ObjectiveType::None)
				continue;

			obj.type = ObjectiveType::CollectItem;
			obj.targetCount = targetCount;
			obj.current = 0;
			obj.completed = false;

			obj.text->SetIsDisplay(true);
			obj.checkMark->SetIsDisplay(true);

			return;
		}
	}
	else if (type == "Special") {
		//プレイヤーの獲得アイテム数を0にリセット
		player_->SetSpecialNum(0);

		for (auto& obj : objectives_) {

			if (obj.type != ObjectiveType::None)
				continue;

			obj.type = ObjectiveType::CollectSpecial;
			obj.targetCount = targetCount;
			obj.current = 0;
			obj.completed = false;

			obj.text->SetIsDisplay(true);
			obj.checkMark->SetIsDisplay(true);

			return;
		}
	}


}

void ObjectiveSystem::AddHitObjective(const std::string& type, int targetCount) {

	if (type == "Cannon") {
		//プレイヤーの砲弾ヒット数を0にリセット
		playerWeaponManager_->SetCannonHitNum(0);

		for (auto& obj : objectives_) {
			if (obj.type != ObjectiveType::None)
				continue;
			obj.type = ObjectiveType::HitCannon;
			obj.targetCount = targetCount;
			obj.current = 0;
			obj.completed = false;
			obj.text->SetIsDisplay(true);
			obj.checkMark->SetIsDisplay(true);
			return;
		}
	}
	else if (type == "Bullet") {
		//プレイヤーの銃弾ヒット数を0にリセット
		playerWeaponManager_->SetBulletHitNum(0);

		for (auto& obj : objectives_) {
			if (obj.type != ObjectiveType::None)
				continue;
			obj.type = ObjectiveType::HitBullet;
			obj.targetCount = targetCount;
			obj.current = 0;
			obj.completed = false;
			obj.text->SetIsDisplay(true);
			obj.checkMark->SetIsDisplay(true);
			return;
		}
	}
}

void ObjectiveSystem::AddDefeatEnemyObjective() {
	for (auto& obj : objectives_) {
		if (obj.type != ObjectiveType::None)
			continue;
		obj.type = ObjectiveType::DefeatEnemy;
		obj.targetCount = 0;
		obj.current = 0;
		obj.completed = false;
		obj.text->SetIsDisplay(true);
		obj.checkMark->SetIsDisplay(true);
		return;
	}
}
