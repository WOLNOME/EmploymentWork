#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include "GameCamera.h"
#include "Input.h"
#include "Object3d.h"
#include "Sprite.h"
#include "MyMath.h"
#include "JsonUtil.h"
#include <Vector3.h>
#include <vector>
#include <memory>

class LevelLoader;
class MessageUI;

/// <summary>
/// プレイヤーの処理全般を管理するクラス
/// </summary>
class Player : public BaseCharacter {
public:
	//デストラクタ
	~Player() override {};
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui() override;

	//当たり判定処理
	void OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) override;

public://getter
	//HP
	int GetHP() const { return hp_; }
	//砲弾リロードタイムタイマー
	float GetCannonReloadTimer() const { return cannonReloadTimer_; }
	//砲弾を発射したかどうか
	bool GetIsCannonFire() const { return isCannonFire_; }
	//銃弾リロードタイムタイマー
	float GetBulletReloadTimer() const { return bulletReloadTimer_; }
	//現在の銃弾の数
	int GetBulletNum() const { return bulletNum_; }
	//銃弾の発射間隔タイマー
	float GetBulletFireIntervalTimer() const { return bulletFireIntervalTimer_; }
	//銃弾を発射したかどうか
	bool GetIsBulletFire() const { return isBulletFire_; }
	//ダメージを受けたか
	bool GetIsDamage() const { return isDamage_; }
	//アイテムのリロード速度アップ数
	uint32_t GetItemReloadSpeedUp() const { return item_reloadSpeedUp_; }
	//アイテムの移動速度アップ数
	uint32_t GetItemMoveSpeedUp() const { return item_moveSpeedUp_; }
	//アイテムの回転速度アップ数
	uint32_t GetItemTurnSpeedUp() const { return item_turnSpeedUp_; }

public://setter
	void SetLevelLoader(LevelLoader* _levelLoader);
	void SetGameCamera(GameCamera* _camera) { camera_ = _camera; }
	void SetMessageUI(MessageUI* _messageUI) { messageUI_ = _messageUI; }

private://非公開関数
	//回転
	void Rotate();
	//移動
	void Move();
	//キャノン攻撃
	void CannonAttack();
	//銃弾攻撃
	void BulletAttack();
	//死亡処理
	void DeadProcess();

	//カメラ処理
	void CameraAlgorithm();

private:
	//インプット
	Input* input_ = nullptr;
	//カメラ
	GameCamera* camera_ = nullptr;
	//メッセージUI
	MessageUI* messageUI_ = nullptr;

private:
	//パラメータ
	json param_;

	//変数
	int hp_;	//現在のHP

	float cannonReloadTimer_;		//砲弾リロードタイム計測用タイマー
	bool isCannonFire_ = false;		//砲弾を発射したかどうか

	float bulletReloadTimer_;		//銃弾リロードタイム計測用タイマー
	int bulletNum_;					//現在の銃弾の数
	float bulletFireIntervalTimer_;	//銃弾の発射間隔タイマー
	bool isBulletFire_ = false;		//銃弾を発射したかどうか

	bool isDamage_ = false;			//ダメージを受けたか

	Vector3 reflectVelocity_ = { 0.0f,0.0f,0.0f };

	//アイテム
	uint32_t item_reloadSpeedUp_;
	uint32_t item_moveSpeedUp_;
	uint32_t item_turnSpeedUp_;
};

