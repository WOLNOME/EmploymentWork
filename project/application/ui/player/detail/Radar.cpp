#include "Radar.h"
#include <SpriteManager.h>
#include <TextureManager.h>
#include <ImGuiManager.h>
#include <BaseCamera.h>

#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>
#include <application/object/character/enemy/manager/EnemyManager.h>
#include <application/object/character/enemy/base/IBaseEnemy.h>

void Radar::Initialize() {
	//スプライトの生成・初期化
	{
		//プレイヤー
		thPlayerMark_ = TextureManager::GetInstance()->LoadTexture("whiteHeart.png");
		playerMark_ = std::make_unique<Sprite>();
		playerMark_->Initialize(SpriteManager::GetInstance()->GenerateName("playerMark"), Sprite::Order::Front1, thPlayerMark_);
		playerMark_->SetAnchorPoint({ 0.5f,0.5f });
		playerMark_->SetSize(playerMark_->GetSize() * 0.2f);
		playerMark_->SetPosition(centerPosition);
		playerMark_->SetColor({ 1,0,0,1 });
	}
	{
		//エネミー
		for (int i = 0; i < kEnemyUINum_; i++) {
			thEnemyMarks_[i] = TextureManager::GetInstance()->LoadTexture("whiteHeart.png");
			enemyMarks_[i] = std::make_unique<Sprite>();
			enemyMarks_[i]->Initialize(SpriteManager::GetInstance()->GenerateName("enemyMark"), Sprite::Order::Front1, thEnemyMarks_[i]);
			enemyMarks_[i]->SetAnchorPoint({ 0.5f,0.5f });
			enemyMarks_[i]->SetSize(enemyMarks_[i]->GetSize() * 0.1f);

		}
	}
	{
		//コンパス
		thCompass_ = TextureManager::GetInstance()->LoadTexture("compass.png");
		compass_ = std::make_unique<Sprite>();
		compass_->Initialize(SpriteManager::GetInstance()->GenerateName("compass"), Sprite::Order::Front2, thCompass_);
		compass_->SetAnchorPoint({ 0.5f,0.5f });
		compass_->SetPosition(centerPosition);
	}
}

void Radar::Update() {
	//カメラがセットされていなければ警告
	assert(camera_ != nullptr && "RadarにBaseCameraインスタンスを渡してください");
	//プレイヤーがセットされていなければ警告
	assert(player_ != nullptr && "RadarにPlayerインスタンスを渡してください");
	//エネミーマネージャーがセットされていなければ警告
	assert(enemyManager_ != nullptr && "RadarにEnemyManagerインスタンスを渡してください");

	//エネミーマークの更新
	UpdateEnemyMark();
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

	//エネミー処理ラムダ
	auto processEnemy = [&](IBaseEnemy* enemy, const Vector4& color, int& spriteIndex) {
		if (!enemy || enemy->GetHP() == 0) return;
		//プレイヤー→敵のベクトルを作る
		Vector3 playerToEnemy = enemy->GetWorldPosition() - player_->GetWorldPosition();
		if (playerToEnemy.Length() > searchLength) return;
		//カメラの回転を適用
		Vector3 rotated = rotateAttach(playerToEnemy, camera_->worldTransform.rotate.y);

		enemyMarks_[spriteIndex]->SetPosition({
			centerPosition.x + (rotated.x * unitLength),
			centerPosition.y - (rotated.z * unitLength)
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
	for (const auto& boss : enemyManager_->GetBosses()) {
		processEnemy(boss.get(), { 1, 0, 1, 1 }, spriteIndex);
	}
}

void Radar::UpdateCompass() {
	//カメラの回転をコンパスにそのまま適用
	compass_->SetRotation(-camera_->worldTransform.rotate.y);
}
