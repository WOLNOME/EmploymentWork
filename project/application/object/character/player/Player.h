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

//前方宣言（エンジン）
namespace Norm {
	class LevelLoader;
}

//前方宣言（アプリケーション）
class PlayerWeaponManager;
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

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// パラメータ(json)を取得する
	/// </summary>
	/// <returns>パラメータ(json)</returns>
	const json& GetParam() { return param_; }
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
	/// 反射速度を取得する
	/// </summary>
	/// <returns>反射速度</returns>
	const Norm::Vector3& GetReflectVelocity() { return reflectVelocity_; }
	/// <summary>
	/// カメラを取得する
	/// </summary>
	/// <returns>カメラ</returns>
	Norm::GameCamera* GetCamera()  { return camera_; }
	/// <summary>
	/// メッセージUIを取得する
	/// </summary>
	/// <returns>メッセージUI</returns>
	MessageUI* GetMessageUI()  { return messageUI_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// HPのセット
	/// </summary>
	/// <param name="_hp">HP</param>
	void SetHP(int _hp) { hp_ = _hp; }
	/// <summary>
	/// ダメージフラグのセット
	/// </summary>
	/// <param name="_isDamage"></param>
	void SetIsDamage(bool _isDamage) { isDamage_ = _isDamage; }

	/// <summary>
	/// 反射速度のセット
	/// </summary>
	/// <param name="_reflectVelocity">反射速度</param>
	void SetReflectVelocity(const Norm::Vector3& _reflectVelocity) { reflectVelocity_ = _reflectVelocity; }

	/// <summary>
	/// プレイヤー武器マネージャーを設定する
	/// </summary>
	/// <param name="_playerWeaponManager">設定するプレイヤー武器マネージャー</param>
	void SetPlayerWeaponManager(PlayerWeaponManager* _playerWeaponManager) { playerWeaponManager_ = _playerWeaponManager; }
	/// <summary>
	/// レベルローダーを設定する
	/// </summary>
	/// <param name="_levelLoader">設定するレベルローダー</param>
	void SetLevelLoader(Norm::LevelLoader* _levelLoader);
	/// <summary>
	/// ゲームカメラを設定する
	/// </summary>
	/// <param name="_camera">設定するゲームカメラ</param>
	void SetGameCamera(Norm::GameCamera* _camera) {
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
	Norm::Input* input_ = nullptr;
	//カメラ
	Norm::GameCamera* camera_ = nullptr;
	//プレイヤー武器マネージャー
	PlayerWeaponManager* playerWeaponManager_ = nullptr;
	//メッセージUI
	MessageUI* messageUI_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメータ
	json param_;
	//死亡演出
	std::unique_ptr<DeathDirection> deathDirection_ = nullptr;


	//変数
	int hp_;	//現在のHP

	float cannonReloadTimer_;		//砲弾リロードタイム計測用タイマー

	float bulletReloadTimer_;		//銃弾リロードタイム計測用タイマー
	int bulletNum_;					//現在の銃弾の数
	float bulletFireIntervalTimer_;	//銃弾の発射間隔タイマー

	bool isDamage_ = false;			//ダメージを受けたか

	Norm::Vector3 prePosition_ = { 0,0,0 };	//前フレームのポジション

	Norm::Vector3 reflectVelocity_ = { 0.0f,0.0f,0.0f };

};

