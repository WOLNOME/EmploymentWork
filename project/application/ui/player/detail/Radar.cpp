#include "Radar.h"
#include <SpriteManager.h>
#include <TextureManager.h>
#include <ImGuiManager.h>
#include <BaseCamera.h>
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>
#include <application/object/character/enemy/manager/EnemyManager.h>
#include <application/object/character/item/manager/ItemManager.h>
#include <application/object/character/enemy/tank/base/IBaseTankEnemy.h>

using namespace Norm;

void Radar::Initialize() {
	//スプライトの生成・初期化
	{
		//プレイヤー
		thPlayerMark_ = TextureManager::GetInstance()->LoadTexture("whiteHeart.png");
		playerMark_ = std::make_unique<Sprite>();
		playerMark_->Initialize(SpriteTag{},SpriteManager::GetInstance()->GenerateName("playerMark"), Order::Front2, thPlayerMark_);
		playerMark_->SetAnchorPoint({ 0.5f,0.5f });
		playerMark_->SetSize(playerMark_->GetSize() * 0.2f);
		playerMark_->SetPosition(kCenterPosition_);
		playerMark_->SetColor({ 1,0,0,1 });
	}
	{
		//エネミー
		thEnemyMark_ = TextureManager::GetInstance()->LoadTexture("whiteHeart.png");
		for (int i = 0; i < kEnemyUINum_; i++) {
			enemyMarks_[i] = std::make_unique<Sprite>();
			enemyMarks_[i]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("enemyMark"), Order::Front2, thEnemyMark_);
			enemyMarks_[i]->SetAnchorPoint({ 0.5f,0.5f });
			enemyMarks_[i]->SetSize(enemyMarks_[i]->GetSize() * 0.1f);

		}
	}
	{
		//アイテム
		thItemMark_ = TextureManager::GetInstance()->LoadTexture("whiteHeart.png");
		for (int i = 0; i < kItemUINum_; i++) {
			itemMarks_[i] = std::make_unique<Sprite>();
			itemMarks_[i]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("itemMark"), Order::Front1, thItemMark_);
			itemMarks_[i]->SetAnchorPoint({ 0.5f,0.5f });
			itemMarks_[i]->SetSize(itemMarks_[i]->GetSize() * 0.1f);
		}
	}
	{
		//コンパス
		thCompass_ = TextureManager::GetInstance()->LoadTexture("compass.png");
		compass_ = std::make_unique<Sprite>();
		compass_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("compass"), Order::Front2, thCompass_);
		compass_->SetAnchorPoint({ 0.5f,0.5f });
		compass_->SetPosition(kCenterPosition_);
	}
}

void Radar::Update() {
	//カメラがセットされていなければ警告
	assert(camera_ != nullptr && "RadarにBaseCameraインスタンスを渡してください");
	//プレイヤーがセットされていなければ警告
	assert(player_ != nullptr && "RadarにPlayerインスタンスを渡してください");
	//エネミーマネージャーがセットされていなければ警告
	assert(enemyManager_ != nullptr && "RadarにEnemyManagerインスタンスを渡してください");
	//アイテムマネージャーがセットされていなければ警告
	assert(itemManager_ != nullptr && "RadarにItemManagerインスタンスを渡してください");

	//エネミーマークの更新
	UpdateEnemyMark();
	//アイテムマークの更新
	UpdateItemMark();
	//コンパスの更新
	UpdateCompass();

}

void Radar::AttachShake(const Vector2& _shakeOffset) {
	//プレイヤーマークを揺らす
	playerMark_->SetShakeOffset(_shakeOffset);
	//エネミーマークを揺らす
	for (int i = 0; i < kEnemyUINum_; i++) {
		enemyMarks_[i]->SetShakeOffset(_shakeOffset);
	}
	//アイテムマークを揺らす
	for (int i = 0; i < kItemUINum_; i++) {
		itemMarks_[i]->SetShakeOffset(_shakeOffset);
	}
	//コンパスを揺らす
	compass_->SetShakeOffset(_shakeOffset);
}

void Radar::AttachBlinking(const Vector4& _color) {
	//コンパスは通常通り色を適用する
	compass_->SetColor(_color);

	//プレイヤーマークは引数と元の色の各成分を掛けた値を適用する
	Vector4 playerMarkColor = { 1,0,0,1 };
	playerMark_->SetColor({ playerMarkColor.x * _color.x,playerMarkColor.y * _color.y ,playerMarkColor.z * _color.z ,playerMarkColor.w * _color.w });

	//エネミーマークは現在のカラーと引数を掛けた値を適用する
	for (int i = 0; i < kEnemyUINum_; i++) {
		enemyMarks_[i]->SetColor({ enemyMarks_[i]->GetColor().x * _color.x,enemyMarks_[i]->GetColor().y * _color.y ,enemyMarks_[i]->GetColor().z * _color.z ,enemyMarks_[i]->GetColor().w * _color.w });
	}

	//アイテムマークは現在のカラーと引数を掛けた値を適用する
	for (int i = 0; i < kItemUINum_; i++) {
		itemMarks_[i]->SetColor({ itemMarks_[i]->GetColor().x * _color.x,itemMarks_[i]->GetColor().y * _color.y ,itemMarks_[i]->GetColor().z * _color.z ,itemMarks_[i]->GetColor().w * _color.w });
	}
}

void Radar::DebugWithImGui() {
#ifdef _DEBUG

#endif //_DEBUG
}

void Radar::UpdateEnemyMark() {
	//回転適用ラムダ
	auto rotateAttach = [](const Vector3& vec, float angleRad) -> Vector3 {
		float cosTheta = std::cos(angleRad);
		float sinTheta = std::sin(angleRad);
		return {
			vec.x * cosTheta - vec.z * sinTheta,
			vec.y,
			vec.x * sinTheta + vec.z * cosTheta
		};
		};

	//タンクエネミー処理ラムダ
	auto processEnemy = [&](IBaseTankEnemy* enemy, const Vector4& color, int& spriteIndex) {
		if (!enemy || enemy->GetHP() == 0) return;
		//プレイヤー→敵のベクトルを作る
		Vector3 playerToEnemy = enemy->GetWorldPosition() - player_->GetWorldPosition();
		if (playerToEnemy.Length() > kSearchLength_) return;
		//カメラの回転を適用
		Vector3 rotated = rotateAttach(playerToEnemy, camera_->worldTransform.GetRotate().y);

		enemyMarks_[spriteIndex]->SetPosition({
			kCenterPosition_.x + (rotated.x * kUnitLength_),
			kCenterPosition_.y - (rotated.z * kUnitLength_)
			});

		enemyMarks_[spriteIndex]->SetIsDisplay(true);
		enemyMarks_[spriteIndex]->SetColor(color);
		spriteIndex++;
		};

	//ジェットエネミー処理ラムダ
	auto processJetEnemy = [&](IBaseJetEnemy* enemy, const Vector4& color, int& spriteIndex) {
		if (!enemy || enemy->GetHP() == 0) return;
		//プレイヤー→敵のベクトルを作る
		Vector3 playerToEnemy = enemy->GetWorldPosition() - player_->GetWorldPosition();
		if (playerToEnemy.Length() > kSearchLength_) return;
		//カメラの回転を適用
		Vector3 rotated = rotateAttach(playerToEnemy, camera_->worldTransform.GetRotate().y);
		enemyMarks_[spriteIndex]->SetPosition({
			kCenterPosition_.x + (rotated.x * kUnitLength_),
			kCenterPosition_.y - (rotated.z * kUnitLength_)
			});
		enemyMarks_[spriteIndex]->SetIsDisplay(true);
		enemyMarks_[spriteIndex]->SetColor(color);
		spriteIndex++;
		};

	//全スプライトを非表示に
	for (int i = 0; i < kEnemyUINum_; i++) {
		enemyMarks_[i]->SetIsDisplay(false);
	}

	//レーダー中心とスプライトインデックス
	int spriteIndex = 0;

	//キャノ太処理（青）
	for (const auto& canota : enemyManager_->GetCanotas()) {
		processEnemy(canota.get(), { 0, 0, 1, 1 }, spriteIndex);
	}

	//ボス処理（紫）
	for (const auto& boss : enemyManager_->GetKeyCanotas()) {
		processEnemy(boss.get(), { 1, 0, 1, 1 }, spriteIndex);
	}

	//ジェット処理（緑）
	for (const auto& jet : enemyManager_->GetJets()) {
		processJetEnemy(jet.get(), { 0, 1, 0, 1 }, spriteIndex);
	}
}

void Radar::UpdateItemMark() {
	//回転適用ラムダ
	auto rotateAttach = [](const Vector3& vec, float angleRad) -> Vector3 {
		float cosTheta = std::cos(angleRad);
		float sinTheta = std::sin(angleRad);
		return {
			vec.x * cosTheta - vec.z * sinTheta,
			vec.y,
			vec.x * sinTheta + vec.z * cosTheta
		};
	};
	//アイテム処理ラムダ
	auto processItem = [&](const Vector3& itemPos, const Vector4& color, int& spriteIndex) {
		if (itemPos.Length() > kSearchLength_) return;
		//プレイヤー→アイテムのベクトルを作る
		Vector3 playerToItem = itemPos - player_->GetWorldPosition();
		Vector3 rotated = rotateAttach(playerToItem, camera_->worldTransform.GetRotate().y);
		itemMarks_[spriteIndex]->SetPosition({
			kCenterPosition_.x + (rotated.x * kUnitLength_),
			kCenterPosition_.y - (rotated.z * kUnitLength_)
			});
		itemMarks_[spriteIndex]->SetIsDisplay(true);
		itemMarks_[spriteIndex]->SetColor(color);
		spriteIndex++;
	};
	//全スプライトを非表示に
	for (int i = 0; i < kItemUINum_; i++) {
		itemMarks_[i]->SetIsDisplay(false);
	}
	//レーダー中心とスプライトインデックス
	int spriteIndex = 0;
	//アイテムマークの更新（白）
	for (const auto& item : itemManager_->GetItems()) {
		//アイテムがアイドル状態なら次へ
		if (item->GetState() == BaseCharacter::State::kIdle) {
			continue;
		}

		processItem(item->GetWorldPosition(), { 1, 1, 1, 1 }, spriteIndex);
	}
}

void Radar::UpdateCompass() {
	//カメラの回転をコンパスにそのまま適用
	compass_->SetRotation(-camera_->worldTransform.GetRotate().y);
}
