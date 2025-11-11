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

//アプリケーション
#include <application/system/DeathDirection.h>

class LevelLoader;
class MessageUI;

/// <summary>
/// プレイヤーの処理全般を管理するクラス
/// </summary>
class Player : public BaseCharacter {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player() override {};
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui() override;

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="attribute">相手の属性</param>
	/// <param name="subjectPos">相手の座標</param>
	void OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) override;
	
	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// 現在のHPを取得する
	/// </summary>
	/// <returns>現在のHP</returns>
	int GetHP() const { return hp_; }
	/// <summary>
	/// 砲弾のリロードタイムタイマーを取得する
	/// </summary>
	/// <returns>砲弾リロードタイムタイマー</returns>
	float GetCannonReloadTimer() const { return cannonReloadTimer_; }
	/// <summary>
	/// 砲弾を発射したかどうかを取得する
	/// </summary>
	/// <returns>発射していればtrue</returns>
	bool GetIsCannonFire() const { return isCannonFire_; }
	/// <summary>
	/// 銃弾のリロードタイムタイマーを取得する
	/// </summary>
	/// <returns>銃弾リロードタイムタイマー</returns>
	float GetBulletReloadTimer() const { return bulletReloadTimer_; }
	/// <summary>
	/// 現在の銃弾の数を取得する
	/// </summary>
	/// <returns>銃弾の残数</returns>
	int GetBulletNum() const { return bulletNum_; }
	/// <summary>
	/// 銃弾の発射間隔タイマーを取得する
	/// </summary>
	/// <returns>発射間隔タイマー</returns>
	float GetBulletFireIntervalTimer() const { return bulletFireIntervalTimer_; }
	/// <summary>
	/// 銃弾を発射したかどうかを取得する
	/// </summary>
	/// <returns>発射していればtrue</returns>
	bool GetIsBulletFire() const { return isBulletFire_; }
	/// <summary>
	/// ダメージを受けたかどうかを取得する
	/// </summary>
	/// <returns>ダメージを受けていればtrue</returns>
	bool GetIsDamage() const { return isDamage_; }
	/// <summary>
	/// 死亡演出中フラグを取得
	/// </summary>
	/// <returns>死亡演出中フラグ</returns>
	bool GetIsDeathDir() const { return deathDirection_->GetIsDirection(); }
	/// <summary>
	/// アイテムによるリロード速度アップ数を取得する
	/// </summary>
	/// <returns>リロード速度アップ数</returns>
	uint32_t GetItemReloadSpeedUp() const { return item_reloadSpeedUp_; }
	/// <summary>
	/// アイテムによる移動速度アップ数を取得する
	/// </summary>
	/// <returns>移動速度アップ数</returns>
	uint32_t GetItemMoveSpeedUp() const { return item_moveSpeedUp_; }
	/// <summary>
	/// アイテムによる回転速度アップ数を取得する
	/// </summary>
	/// <returns>回転速度アップ数</returns>
	uint32_t GetItemTurnSpeedUp() const { return item_turnSpeedUp_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// レベルローダーを設定する
	/// </summary>
	/// <param name="_levelLoader">設定するレベルローダー</param>
	void SetLevelLoader(LevelLoader* _levelLoader);
	/// <summary>
	/// ゲームカメラを設定する
	/// </summary>
	/// <param name="_camera">設定するゲームカメラ</param>
	void SetGameCamera(GameCamera* _camera) {
		camera_ = _camera;
		deathDirection_->SetGameCamera(_camera);
	}
	/// <summary>
	/// メッセージUIを設定する
	/// </summary>
	/// <param name="_messageUI">設定するメッセージUI</param>
	void SetMessageUI(MessageUI* _messageUI) { messageUI_ = _messageUI; }

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

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

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//インプット
	Input* input_ = nullptr;
	//カメラ
	GameCamera* camera_ = nullptr;
	//メッセージUI
	MessageUI* messageUI_ = nullptr;
	//死亡演出
	std::unique_ptr<DeathDirection> deathDirection_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

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

