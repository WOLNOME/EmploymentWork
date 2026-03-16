#include "Radar.h"
#include <SpriteManager.h>
#include <TextureManager.h>
#include <ImGuiManager.h>
#include <BaseCamera.h>
#include <cassert>

//アプリケーション
#include <application/system/CameraManager.h>
#include <application/object/character/player/Player.h>
#include <application/object/character/enemy/manager/EnemyManager.h>
#include <application/object/character/item/manager/ItemManager.h>
#include <application/object/character/enemy/tank/base/IBaseTankEnemy.h>

using namespace Norm;

void Radar::Initialize() {
	//jsonファイルの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerUI");

	//変数の初期化
	centerPosition_ = { param_["radar"]["centerPos"]["x"],param_["radar"]["centerPos"]["y"] };

	//スプライトの生成・初期化

	//コンパス
	{
		thCompass_ = TextureManager::GetInstance()->LoadTexture("compass.png");
		compass_ = std::make_unique<Sprite>();
		compass_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("compass"), Order::Front4, thCompass_);
		compass_->SetAnchorPoint({ 0.5f,0.5f });
		compass_->SetPosition(centerPosition_);
	}
	//レーダー基盤
	{
		thRadarBase_ = TextureManager::GetInstance()->LoadTexture("radar.png");
		radarBase_ = std::make_unique<Sprite>();
		radarBase_->Initialize(UVScrollTag{}, SpriteManager::GetInstance()->GenerateName("radar"), Order::Front2, 18, 0.07f, true, thRadarBase_);
		radarBase_->SetAnchorPoint({ 0.5f,0.5f });
		radarBase_->SetPosition(centerPosition_);
		radarBase_->SetIsPlayUVScroll(true);
	}
	//テクスチャハンドル
	{
		thCharacterMark_ = TextureManager::GetInstance()->LoadTexture("whiteTriangle.png");
		thPlayerMark_ = TextureManager::GetInstance()->LoadTexture("playerIcon.png");
		thItemMark_ = TextureManager::GetInstance()->LoadTexture("whiteCircle.png");
	}

	//プレイヤー
	{
		playerMark_ = std::make_unique<Sprite>();
		playerMark_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("playerMark"), Order::Front3, thPlayerMark_);
		playerMark_->SetAnchorPoint({ 0.5f,0.5f });
		playerMark_->SetPosition(centerPosition_);
		playerMark_->SetColor({ 1,1,1,1 });
		playerMark_->SetSize(playerMark_->GetSize() * 2.5f);
	}
	//エネミー
	{
		for (int i = 0; i < kEnemyUINum_; i++) {
			enemyMarks_[i] = std::make_unique<Sprite>();
			enemyMarks_[i]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("enemyMark"), Order::Front3, thCharacterMark_);
			enemyMarks_[i]->SetAnchorPoint({ 0.5f,0.5f });

		}
	}
	//アイテム
	{
		for (int i = 0; i < kItemUINum_; i++) {
			itemMarks_[i] = std::make_unique<Sprite>();
			itemMarks_[i]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("itemMark"), Order::Front3, thItemMark_);
			itemMarks_[i]->SetAnchorPoint({ 0.5f,0.5f });
		}
	}
	//誘導用矢印
	{
		//テクスチャハンドル
		thInductionArrow_ = TextureManager::GetInstance()->LoadTexture("inductionArrow.png");

		for (int i = 0; i < kInductionArrowNum_; i++) {
			inductionArrows_[i] = std::make_unique<Sprite>();
			inductionArrows_[i]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("inductionArrow"), Order::Front3, thInductionArrow_);
			inductionArrows_[i]->SetAnchorPoint({ 0.5f,0.5f });
		}
	}
}

void Radar::Update() {
	//カメラマネージャーがセットされていなければ警告
	assert(cameraManager_ != nullptr && "RadarにCameraManagerインスタンスを渡してください");
	//プレイヤーがセットされていなければ警告
	assert(player_ != nullptr && "RadarにPlayerインスタンスを渡してください");
	//エネミーマネージャーがセットされていなければ警告
	assert(enemyManager_ != nullptr && "RadarにEnemyManagerインスタンスを渡してください");
	//アイテムマネージャーがセットされていなければ警告
	assert(itemManager_ != nullptr && "RadarにItemManagerインスタンスを渡してください");

	//プレイヤーマークの更新
	UpdatePlayerMark();
	//エネミーマークの更新
	UpdateEnemyMark();
	//アイテムマークの更新
	UpdateItemMark();
	//誘導用矢印の更新
	UpdateInductionArrow();
	//コンパスの更新
	UpdateCompass();

}

void Radar::AttachShake(const Vector2& _shakeOffset) {
	//コンパスを揺らす
	compass_->SetShakeOffset(_shakeOffset);
	//レーダー基盤を揺らす
	radarBase_->SetShakeOffset(_shakeOffset);

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
	//誘導用矢印を揺らす
	for (int i = 0; i < kInductionArrowNum_; i++) {
		inductionArrows_[i]->SetShakeOffset(_shakeOffset);
	}
}

void Radar::AttachBlinking(const Vector4& _color) {
	//コンパスは通常通り色を適用する
	compass_->SetColor(_color);
	//レーダー基盤は通常通り色を適用する
	radarBase_->SetColor(_color);

	//プレイヤーマークは引数と元の色の各成分を掛けた値を適用する
	Vector4 playerMarkColor = { 1,1,1,1 };
	playerMark_->SetColor({ playerMarkColor.x * _color.x,playerMarkColor.y * _color.y ,playerMarkColor.z * _color.z ,playerMarkColor.w * _color.w });

	//エネミーマークは現在のカラーと引数を掛けた値を適用する
	for (int i = 0; i < kEnemyUINum_; i++) {
		enemyMarks_[i]->SetColor({ enemyMarks_[i]->GetColor().x * _color.x,enemyMarks_[i]->GetColor().y * _color.y ,enemyMarks_[i]->GetColor().z * _color.z ,enemyMarks_[i]->GetColor().w * _color.w });
	}

	//アイテムマークは現在のカラーと引数を掛けた値を適用する
	for (int i = 0; i < kItemUINum_; i++) {
		itemMarks_[i]->SetColor({ itemMarks_[i]->GetColor().x * _color.x,itemMarks_[i]->GetColor().y * _color.y ,itemMarks_[i]->GetColor().z * _color.z ,itemMarks_[i]->GetColor().w * _color.w });
	}

	//誘導用矢印は現在のカラーと引数を掛けた値を適用する
	for (int i = 0; i < kInductionArrowNum_; i++) {
		inductionArrows_[i]->SetColor({ inductionArrows_[i]->GetColor().x * _color.x,inductionArrows_[i]->GetColor().y * _color.y ,inductionArrows_[i]->GetColor().z * _color.z ,inductionArrows_[i]->GetColor().w * _color.w });
	}

}

void Radar::DebugWithImGui() {
#ifdef _DEBUG

#endif //_DEBUG
}

void Radar::UpdatePlayerMark() {
	//本体の回転を適用
	float playerRotated = -cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y + player_->GetWorldTransform().GetRotate().y;
	playerMark_->SetRotation(playerRotated);
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
	auto processEnemy = [&](IBaseTankEnemy* enemy, const Vector4& color, int& spriteIndex, float sizeRate) {
		//敵がアクティブでないなら処理しない
		if (enemy->GetState() != BaseCharacter::State::kActive) return;
		//プレイヤー→敵のベクトルを作る
		Vector3 playerToEnemy = enemy->GetWorldTransform().GetWorldTranslate() - player_->GetWorldTransform().GetWorldTranslate();
		if (playerToEnemy.Length() > kSearchLength_) return;
		//カメラの回転を適用
		Vector3 cameraRotated = rotateAttach(playerToEnemy, cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y);
		enemyMarks_[spriteIndex]->SetPosition({
			centerPosition_.x + (cameraRotated.x * kUnitLength_),
			centerPosition_.y - (cameraRotated.z * kUnitLength_)
			});
		//本体の回転を適用（カメラ回転+本体の回転）
		float subjectRotated = -cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y + enemy->GetWorldTransform().GetRotate().y;
		enemyMarks_[spriteIndex]->SetRotation(subjectRotated);
		//サイズ設定
		float markSize = param_["radar"]["markSize"];
		markSize *= sizeRate;
		enemyMarks_[spriteIndex]->SetSize({ markSize,markSize });

		enemyMarks_[spriteIndex]->SetIsDisplay(true);
		enemyMarks_[spriteIndex]->SetColor(color);
		spriteIndex++;
		};

	//ジェットエネミー処理ラムダ
	auto processJetEnemy = [&](IBaseJetEnemy* enemy, const Vector4& color, int& spriteIndex, float sizeRate) {
		//敵がアクティブでないなら処理しない
		if (enemy->GetState() != BaseCharacter::State::kActive) return;
		//プレイヤー→敵のベクトルを作る
		Vector3 playerToEnemy = enemy->GetWorldTransform().GetWorldTranslate() - player_->GetWorldTransform().GetWorldTranslate();
		if (playerToEnemy.Length() > kSearchLength_) return;
		//カメラの回転を適用
		Vector3 cameraRotated = rotateAttach(playerToEnemy, cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y);
		enemyMarks_[spriteIndex]->SetPosition({
			centerPosition_.x + (cameraRotated.x * kUnitLength_),
			centerPosition_.y - (cameraRotated.z * kUnitLength_)
			});
		//本体の回転を適用（カメラ回転+本体の回転）
		float subjectRotated = -cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y + enemy->GetWorldTransform().GetRotate().y;
		enemyMarks_[spriteIndex]->SetRotation(subjectRotated);
		//サイズ設定
		float markSize = param_["radar"]["markSize"];
		markSize *= sizeRate;
		enemyMarks_[spriteIndex]->SetSize({ markSize,markSize });

		enemyMarks_[spriteIndex]->SetIsDisplay(true);
		enemyMarks_[spriteIndex]->SetColor(color);
		spriteIndex++;
		};

	//ボスエネミー処理ラムダ
	auto processBossEnemy = [&](Boss* enemy, const Vector4& color, int& spriteIndex, float sizeRate) {
		//敵がアクティブでないなら処理しない
		if (enemy->GetState() != BaseCharacter::State::kActive) return;
		//プレイヤー→敵のベクトルを作る
		Vector3 playerToEnemy = enemy->GetWorldTransform().GetWorldTranslate() - player_->GetWorldTransform().GetWorldTranslate();
		if (playerToEnemy.Length() > kSearchLength_) return;
		//カメラの回転を適用
		Vector3 cameraRotated = rotateAttach(playerToEnemy, cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y);
		enemyMarks_[spriteIndex]->SetPosition({
			centerPosition_.x + (cameraRotated.x * kUnitLength_),
			centerPosition_.y - (cameraRotated.z * kUnitLength_)
			});
		//本体の回転を適用（カメラ回転+本体の回転）
		float subjectRotated = -cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y + enemy->GetWorldTransform().GetRotate().y;
		enemyMarks_[spriteIndex]->SetRotation(subjectRotated);
		//サイズ設定
		float markSize = param_["radar"]["markSize"];
		markSize *= sizeRate;
		enemyMarks_[spriteIndex]->SetSize({ markSize,markSize });

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
		processEnemy(canota.get(), { 0, 0, 1, 1 }, spriteIndex, 0.8f);
	}

	//キーキャノ太処理（紫）
	for (const auto& keyCanota : enemyManager_->GetKeyCanotas()) {
		processEnemy(keyCanota.get(), { 1, 0, 1, 1 }, spriteIndex, 1.0f);
	}

	//ジェット処理（緑）
	for (const auto& jet : enemyManager_->GetJets()) {
		processJetEnemy(jet.get(), { 0, 1, 0, 1 }, spriteIndex, 0.8f);
	}

	//ボス処理（赤）
	for (const auto& boss : enemyManager_->GetBosses()) {
		processBossEnemy(boss.get(), { 1, 0, 0, 1 }, spriteIndex, 1.2f);
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
	auto processItem = [&](const Vector3& itemPos, const Vector4& color, int& spriteIndex, float sizeRate) {
		//プレイヤー→アイテムのベクトルを作る
		Vector3 playerToItem = itemPos - player_->GetWorldTransform().GetWorldTranslate();
		if (playerToItem.Length() > kSearchLength_) return;
		//カメラの回転を適用
		Vector3 cameraRotated = rotateAttach(playerToItem, cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y);
		itemMarks_[spriteIndex]->SetPosition({
			centerPosition_.x + (cameraRotated.x * kUnitLength_),
			centerPosition_.y - (cameraRotated.z * kUnitLength_)
			});
		//サイズ設定
		float markSize = param_["radar"]["markSize"];
		markSize *= sizeRate;
		itemMarks_[spriteIndex]->SetSize({ markSize,markSize });

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
	//回復アイテムマークの更新（黄緑）
	for (const auto& healItem : itemManager_->GetHealItems()) {
		//アイテムがアイドル状態なら次へ
		if (healItem->GetState() == BaseCharacter::State::kIdle) {
			continue;
		}

		processItem(healItem->GetWorldTransform().GetWorldTranslate(), { 0.68f,1.0f,0.18f,1.0f }, spriteIndex, 0.6f);
	}
	//スペシャルチャージアイテムマークの更新（水色）
	for (const auto& chargeItem : itemManager_->GetChargeItems()) {
		//アイテムがアイドル状態なら次へ
		if (chargeItem->GetState() == BaseCharacter::State::kIdle) {
			continue;
		}
		processItem(chargeItem->GetWorldTransform().GetWorldTranslate(), { 0.4f,0.8f,1.0f,1.0f }, spriteIndex, 0.6f);
	}
	//キーアイテムマークの更新（黄）
	for (const auto& keyItem : itemManager_->GetKeyItems()) {
		//アイテムがアイドル状態なら次へ
		if (keyItem->GetState() == BaseCharacter::State::kIdle) {
			continue;
		}

		processItem(keyItem->GetWorldTransform().GetWorldTranslate(), { 1, 1, 0, 1 }, spriteIndex, 0.8f);
	}
}

void Radar::UpdateInductionArrow() {
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
	//誘導用矢印処理ラムダ
	auto processInductionArrow = [&](const Vector3& subjectPos, const Vector4& color, int& spriteIndex) {
		//プレイヤー→対象物のベクトルを作る
		Vector3 playerToSubject = subjectPos - player_->GetWorldTransform().GetWorldTranslate();
		//対象物が規定距離より短いなら表示しない
		if (playerToSubject.Length() < param_["radar"]["inductionArrowMinLength"].get<float>()) return;
		//対象物がレーダーの範囲外に外れている場合
		Vector3 rotated;
		if (playerToSubject.Length() > kSearchLength_) {
			//カメラの回転を適用
			rotated = rotateAttach(playerToSubject.Normalized() * kSearchLength_, cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y);
		}
		else {
			//カメラの回転を適用
			rotated = rotateAttach(playerToSubject.Normalized() * (playerToSubject.Length() - param_["radar"]["inductionArrowOffsetLength"].get<float>()), cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y);
		}
		//座標
		inductionArrows_[spriteIndex]->SetPosition({
			centerPosition_.x + (rotated.x * kUnitLength_),
			centerPosition_.y - (rotated.z * kUnitLength_)
			});
		//本体の回転を適用（カメラ回転+本体の回転）
		Vector3 cameraForward = {
			sinf(cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y),
			0.0f,
			cosf(cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y)
		};
		float subjectRotated = MyMath::AngleOf2VectorY(cameraForward, playerToSubject);
		inductionArrows_[spriteIndex]->SetRotation(subjectRotated);

		inductionArrows_[spriteIndex]->SetIsDisplay(true);
		inductionArrows_[spriteIndex]->SetColor(color);
		spriteIndex++;
		};

	//全スプライトを非表示に
	for (int i = 0; i < kInductionArrowNum_; i++) {
		inductionArrows_[i]->SetIsDisplay(false);
	}
	//レーダー中心とスプライトインデックス
	int spriteIndex = 0;

	//キーキャノ太(紫)
	for (const auto& keyCanota : enemyManager_->GetKeyCanotas()) {
		//キーキャノ太がアイドル状態なら次へ
		if (keyCanota->GetState() == BaseCharacter::State::kIdle) {
			continue;
		}

		processInductionArrow(keyCanota->GetWorldTransform().GetWorldTranslate(), { 1,0,1,1 }, spriteIndex);
	}
	//ボス(赤)
	for (const auto& boss : enemyManager_->GetBosses()) {
		//ボスがアイドル状態なら次へ
		if (boss->GetState() == BaseCharacter::State::kIdle) {
			continue;
		}

		processInductionArrow(boss->GetWorldTransform().GetWorldTranslate(), { 1,0,0,1 }, spriteIndex);
	}
	//鍵(黄)
	for (const auto& key : itemManager_->GetKeyItems()) {
		//鍵がアイドル状態なら次へ
		if (key->GetState() == BaseCharacter::State::kIdle) {
			continue;
		}

		processInductionArrow(key->GetWorldTransform().GetWorldTranslate(), { 1,1,0,1 }, spriteIndex);
	}
}

void Radar::UpdateCompass() {
	//カメラの回転をコンパスにそのまま適用
	compass_->SetRotation(-cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y);
}
