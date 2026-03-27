#include "Player.h"
#include "WinApp.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "Object3dManager.h"
#include <algorithm>
#include <cassert>

//アプリケーション
#include "application/system/CameraManager.h"
#include "application/object/level/loader/LevelLoader.h"
#include <application/object/character/player/collision/PlayerCollider.h>
#include <application/object/character/weapon/player/manager/PlayerWeaponManager.h>
#include <application/ui/player/PlayerUI.h>
#include <application/ui/message/MessageUI.h>

using namespace Norm;

void Player::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//SEの初期化
	moveSE_ = std::make_unique<Audio>();
	moveSE_->Initialize("se/tank_move.mp3");
	volumeMoveSE_ = 0.0f;
	moveSE_->Play(true, volumeMoveSE_);

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/player");

	//インプットの初期化
	input_ = Input::GetInstance();
	input_->SetIsMouseDisplay(false);
	input_->SetIsMouseFixed(true);
	//オブジェクトの生成・初期化
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Player"), "player");

	//死亡演出の生成・初期化
	deathDirection_ = std::make_unique<DeathDirection>();
	deathDirection_->Initialize();

	//当たり判定の生成・初期化
	collider_ = std::make_unique<PlayerCollider>(this);
	auto* playerCollider = dynamic_cast<PlayerCollider*>(collider_.get());
	collider_->SetCollisionAttribute(CollisionAttribute::Player);
	collider_->SetWorldTransform(&worldTransform_);
	collider_->SetOffset({
		param_["collisionCenterOffsetOBB"]["x"],
		param_["collisionCenterOffsetOBB"]["y"],
		param_["collisionCenterOffsetOBB"]["z"]
		});
	playerCollider->SetOBBSize({
		param_["collisionSizeOBB"]["x"],
		param_["collisionSizeOBB"]["y"],
		param_["collisionSizeOBB"]["z"]
		});

	//アクティブ状態として初期化
	SetState(State::kActive);

	//パラメータのセット
	int maxHP = param_["maxHP"];
	hp_ = maxHP;
	keyNum_ = 0;
	cannonReloadTimer_ = 0.0f;
	bulletReloadTimer_ = 0.0f;
	int bulletMaxNum = param_["bulletMagazine"];
	bulletNum_ = bulletMaxNum;
	bulletFireIntervalTimer_ = 0.0f;
	specialNum_ = 0;
	specialFireIntervalTimer_ = 0.0f;

}

void Player::Update() {
	//カメラマネージャーがセットされていなかったら警告
	assert(cameraManager_ && "カメラマネージャーがセットされていません。");
	//メッセージUIがセットされていなかったら警告
	assert(messageUI_ && "メッセージUIがセットされていません。");

	//ベースキャラクターの更新
	BaseCharacter::Update();
	//ダメージ更新
	isDamage_ = false;

	//移動処理
	Move();
	//攻撃処理
	CannonAttack();
	BulletAttack();
	SpecialAttack();
	//死亡処理
	DeadProcess();
	//死亡演出更新
	deathDirection_->Update();

	//カメラ処理
	CameraAlgorithm();
}

void Player::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();
	//死亡演出のデバッグ処理
	deathDirection_->DebugWithImGui();

	ImGui::Begin("プレイヤー");
	//HP
	int maxHP = param_["maxHP"];
	ImGui::DragInt("HP", &hp_, 1, 0, maxHP);
	//機関銃
	ImGui::Text("機関銃");
	ImGui::Text("リロードタイム: %.2f", bulletReloadTimer_);
	ImGui::Text("発射間隔タイマー: %.2f", bulletFireIntervalTimer_);
	ImGui::Text("現在の弾数: %d", bulletNum_);

	ImGui::End();

#endif // _DEBUG

}

void Player::SetLevelLoader(LevelLoader* _levelLoader) {
	//プレイヤーの座標を読み込む
	for (const auto& playerSpawnData : _levelLoader->GetPlayerSpawnData()) {
		TransformEuler initTransform;
		initTransform.translate = playerSpawnData.translation;
		initTransform.translate.y = 0.0f;
		initTransform.rotate = playerSpawnData.rotation;
		initTransform.scale = { 1,1,1 };

		//車体を合わせる
		worldTransform_.SetTranslate(initTransform.translate);
		worldTransform_.SetRotate(initTransform.rotate);
		//カメラの向きを車体に合わせる
		cameraManager_->GetActiveCamera()->worldTransform.SetRotate(worldTransform_.GetRotate());

		//最初のデータのみを読み込む
		break;
	}
}

void Player::SetCameraManager(CameraManager* _cameraManager) {
	cameraManager_ = _cameraManager;
	//死亡演出にカメラをセット
	deathDirection_->SetCameraManager(cameraManager_);
}

void Player::Move() {
	// 死亡演出中なら処理をしない
	if (deathDirection_->GetIsDirection())
		return;

	// アクティブでないなら処理をしない
	if (state_ != State::kActive)
		return;

	// 移動前に前フレームの座標を保存
	prePosition_ = worldTransform_.GetWorldTranslate();

	//----------------------------------------------------------
	// カメラ基準の移動方向を取得
	//----------------------------------------------------------
	Vector3 camRot = cameraManager_->GetActiveCamera()->worldTransform.GetRotate();
	Vector3 camForward = {
		std::sinf(camRot.y),
		0.0f,
		std::cosf(camRot.y)
	};
	camForward.Normalize();
	camForward.y = 0.0f;
	Vector3 camRight = {
		camForward.z,
		0.0f,
		-camForward.x
	};
	camRight.y = 0.0f;

	Vector3 inputDir = { 0.0f, 0.0f, 0.0f };

	// キー & スティック入力を合成
	if (isInput_) {
		if (input_->PushKey(DIK_W) || input_->GetLStickDir().y > 0.0f) {
			inputDir += camForward;
		}
		if (input_->PushKey(DIK_S) || input_->GetLStickDir().y < 0.0f) {
			inputDir -= camForward;
		}
		if (input_->PushKey(DIK_D) || input_->GetLStickDir().x > 0.0f) {
			inputDir += camRight;
		}
		if (input_->PushKey(DIK_A) || input_->GetLStickDir().x < 0.0f) {
			inputDir -= camRight;
		}
	}

	float speed = param_["speed"];

	if (inputDir.LengthSq() > 0.0f) {
		inputDir.Normalize();
		velocity_ += inputDir * speed;
	}

	//----------------------------------------------------------
	// 床の抵抗
	//----------------------------------------------------------
	if (velocity_.LengthSq() > 0.0f) {
		Vector3 frictionDir = -velocity_.Normalized();
		Vector3 frictionAccel = frictionDir * floorFriction_;
		velocity_ += frictionAccel * kDeltaTime;
	}

	//----------------------------------------------------------
	// 最大速度制限
	//----------------------------------------------------------
	float maxSpeed = param_["maxSpeed"];
	if (velocity_.Length() > maxSpeed) {
		velocity_.Normalize();
		velocity_ *= maxSpeed;
	}

	// 微小速度を0に
	if ((velocity_ * kDeltaTime).Length() < 0.01f) {
		velocity_ = { 0.0f,0.0f,0.0f };
	}

	//----------------------------------------------------------
	// 反発速度
	//----------------------------------------------------------
	velocity_ += reflectVelocity_;

	if (reflectVelocity_.LengthSq() > 0.0f) {
		Vector3 decayDir = -reflectVelocity_.Normalized();
		Vector3 decayAccel = decayDir * 40.0f;
		reflectVelocity_ += decayAccel * kDeltaTime;

		if (reflectVelocity_.Length() < 1.0f) {
			reflectVelocity_ = { 0.0f,0.0f,0.0f };
		}
	}

	//----------------------------------------------------------
	// 位置更新
	//----------------------------------------------------------
	Vector3 newTranslate = worldTransform_.GetTranslate() + velocity_ * kDeltaTime;

	const float limit = 995.0f;
	newTranslate.x = std::clamp(newTranslate.x, -limit, limit);
	newTranslate.z = std::clamp(newTranslate.z, -limit, limit);

	worldTransform_.SetTranslate(newTranslate);

	//----------------------------------------------------------
	// 車体回転（進行方向へ補間）
	//----------------------------------------------------------
	Vector3 moveDir = velocity_;
	moveDir.y = 0.0f;

	if (moveDir.LengthSq() > 0.0001f) {

		moveDir.Normalize();

		// 現在の車体前方向
		Vector3 forward = {
			std::sinf(worldTransform_.GetRotate().y),
			0.0f,
			std::cosf(worldTransform_.GetRotate().y)
		};
		forward.Normalize();

		// 前向きと後ろ向きのどちらが近いか
		float dotForward = MyMath::Dot(forward, moveDir);
		float dotBackward = MyMath::Dot(-forward, moveDir);

		Vector3 targetDir = (dotBackward > dotForward) ? -moveDir : moveDir;

		// 目標角度
		float targetYaw = std::atan2(targetDir.x, targetDir.z);

		// 現在角度
		Vector3 rot = worldTransform_.GetRotate();
		float currentYaw = rot.y;

		// 角度差（-π～π）
		float diff = MyMath::NormalizeAngle(targetYaw - currentYaw);

		// 補間
		float turnSpeed = param_["turnSpeed"];
		currentYaw += diff * turnSpeed * kDeltaTime;

		rot.y = currentYaw;
		worldTransform_.SetRotate(rot);
	}

	//----------------------------------------------------------
	// 移動SEの音量
	//----------------------------------------------------------
	volumeMoveSE_ = MyMath::Lerp(0.0f, 1.0f, velocity_.Length() / maxSpeed);
	moveSE_->SetVolume(volumeMoveSE_);
}

void Player::CannonAttack() {
	//死亡演出中なら処理をしない
	if (deathDirection_->GetIsDirection())
		return;
	//アクティブでないなら処理をしない
	if (state_ != State::kActive)
		return;

	//リロードタイムの計算
	if (cannonReloadTimer_ > 0.0f) {
		cannonReloadTimer_ -= kDeltaTime;
		//リロードタイムがマイナスになったら0にする
		if (cannonReloadTimer_ < 0.0f) {
			cannonReloadTimer_ = 0.0f;
		}

		return;
	}

	//スペースキーで砲弾を発射
	if (isInput_) {
		if (input_->TriggerMouseButton(MouseButton::RightButton) || (input_->GetLT() > 0.5f)) {
			//リロードタイムをセット
			cannonReloadTimer_ = param_["cannonReloadTime"];
			//初期位置と発射方向の計算
			float orx = cameraManager_->GetActiveCamera()->worldTransform.GetRotate().x;
			float ory = cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y;
			Vector3 currentDir = {
				std::cosf(orx) * std::sinf(ory),
				-std::sinf(orx),		//←角度
				std::cosf(orx) * std::cosf(ory)
			};
			currentDir.Normalize();
			Vector3 cannonPos = worldTransform_.GetTranslate();
			cannonPos.y += 7.2f;	//砲弾の初期位置を調整
			Vector3 cannonDirection = currentDir;
			//スポーン
			playerWeaponManager_->SpawnCannon(cannonPos, cannonDirection);
		}
	}
}

void Player::BulletAttack() {
	//死亡演出中なら処理をしない
	if (deathDirection_->GetIsDirection())
		return;
	//アクティブでないなら処理をしない
	if (state_ != State::kActive)
		return;

	//発射間隔の計算
	bool isInterval = false;
	if (bulletFireIntervalTimer_ > 0.0f) {
		isInterval = true;
		bulletFireIntervalTimer_ -= kDeltaTime;
		//発射間隔がマイナスになったら0にする
		if (bulletFireIntervalTimer_ < 0.0f) {
			bulletFireIntervalTimer_ = 0.0f;
		}
	}

	//リロードタイムの計算
	bool isReload = false;
	if (bulletReloadTimer_ > 0.0f) {
		isReload = true;
		bulletReloadTimer_ -= kDeltaTime;
		//リロードタイムがマイナスになったら0にする
		if (bulletReloadTimer_ < 0.0f) {
			bulletReloadTimer_ = 0.0f;
			//銃弾数をリロード
			int bulletMaxNum_ = param_["bulletMagazine"];
			bulletNum_ = bulletMaxNum_;
		}
	}

	//インターバルおよびリロード中は発射しない
	if (isInterval || isReload) {
		return;
	}

	//左クリックで銃弾を発射
	if (isInput_) {
		if (input_->PushMouseButton(MouseButton::LeftButton) || (input_->GetRT() > 0.5f)) {
			//間隔計測用タイマーをセット
			float bulletFireIntervalTime = param_["bulletFireIntervalTime"];
			bulletFireIntervalTimer_ = bulletFireIntervalTime;
			//現在の銃弾数を減らす
			bulletNum_--;
			//銃弾数が0になったらリロードタイマーをセット
			if (bulletNum_ <= 0) {
				bulletReloadTimer_ = param_["bulletReloadTime"];
			}
			//初期位置と発射方向を計算
			float orx = cameraManager_->GetActiveCamera()->worldTransform.GetRotate().x;
			float ory = cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y;
			Vector3 currentDir = {
				std::cosf(orx) * std::sinf(ory),
				-std::sinf(orx),		//←角度
				std::cosf(orx) * std::cosf(ory)
			};
			currentDir.Normalize();
			Vector3 bulletPos = cameraManager_->GetActiveCamera()->worldTransform.GetTranslate();
			bulletPos += currentDir * 8.0f;	//銃弾の初期位置を調整
			//スポーン
			playerWeaponManager_->SpawnBullet(bulletPos, currentDir);
		}
	}
}

void Player::SpecialAttack() {
	//死亡演出中なら処理をしない
	if (deathDirection_->GetIsDirection())
		return;
	//アクティブでないなら処理をしない
	if (state_ != State::kActive)
		return;

	//発射間隔の計算
	bool isInterval = false;
	if (specialFireIntervalTimer_ > 0.0f) {
		isInterval = true;
		specialFireIntervalTimer_ -= kDeltaTime;
		//発射間隔がマイナスになったら0にする
		if (specialFireIntervalTimer_ < 0.0f) {
			specialFireIntervalTimer_ = 0.0f;
		}
	}

	//インターバル中は発射しない
	if (isInterval) {
		return;
	}
	//必殺弾の数が0以下なら発射しない
	if (specialNum_ <= 0) {
		return;
	}

	//右クリックで必殺弾を発射
	if (isInput_) {
		if (input_->PushMouseButton(MouseButton::MiddleButton) || input_->TriggerPadButton(GamePadButton::B)) {
			//間隔計測用タイマーをセット
			float specialFireIntervalTime = param_["specialFireIntervalTime"];
			specialFireIntervalTimer_ = specialFireIntervalTime;
			//必殺弾の数を減らす
			specialNum_--;
			//初期位置と発射方向を計算
			float orx = cameraManager_->GetActiveCamera()->worldTransform.GetRotate().x;
			float ory = cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y;
			Vector3 currentDir = {
				std::cosf(orx) * std::sinf(ory),
				-std::sinf(orx),		//←角度
				std::cosf(orx) * std::cosf(ory)
			};
			currentDir.Normalize();
			Vector3 specialPos = worldTransform_.GetTranslate();
			specialPos.y += 7.2f;	//必殺弾の初期位置を調整
			//スポーン
			playerWeaponManager_->SpawnSpecial(specialPos, currentDir);
		}
	}
}

void Player::DeadProcess() {
	//HPが0になったら死亡演出開始
	if (hp_ <= 0) {
		//死亡演出開始
		deathDirection_->SetIsDirection(true);
	}
	//死亡演出が終了したら死亡
	if (deathDirection_->GetIsDirFinished()) {
		//アイドル状態にする
		SetState(State::kIdle);
	}
}

void Player::CameraAlgorithm() {
	//死亡演出中なら処理をしない
	if (deathDirection_->GetIsDirection())
		return;
	//アクティブでないなら処理をしない
	if (state_ != State::kActive)
		return;
	//プレイヤー側のカメラ処理を行わないなら
	if (isCameraFree_)
		return;

	//カメラの操作にオブジェクトの回転を合わせる
	Vector2 moveValue;
	Vector2 mouseMoveValue = {};
	Vector2 padMoveValue = {};
	if (isInput_) {
		mouseMoveValue = input_->GetMousePosition();
		padMoveValue = {
			input_->GetRStickDir().x * 20.0f,
			input_->GetRStickDir().y * 10.0f,
		};
	}
	padMoveValue.y *= -1.0f;
	moveValue = mouseMoveValue + padMoveValue;
	//デッドゾーン
	float deadZone = 1.5f;
	if (moveValue.Length() > deadZone) {
		Vector3 newRotate = cameraManager_->GetActiveCamera()->worldTransform.GetRotate();
		newRotate.x += moveValue.y * 0.0005f;
		newRotate.y += moveValue.x * 0.0005f;
		cameraManager_->GetActiveCamera()->worldTransform.SetRotate(newRotate);
	}

	//回転制限
	const float maxPitch = (pi / 30.0f);		//下向き制限
	const float minPitch = -(pi / 9.0f);		//上向き制限
	Vector3 newRotate = cameraManager_->GetActiveCamera()->worldTransform.GetRotate();
	newRotate.x = std::clamp(cameraManager_->GetActiveCamera()->worldTransform.GetRotate().x, minPitch, maxPitch);
	//水平回転をπ~-πの間に収める
	if (cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y > pi)
		newRotate.y -= pi * 2.0f;
	else if (cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y < -pi)
		newRotate.y += pi * 2.0f;
	cameraManager_->GetActiveCamera()->worldTransform.SetRotate(newRotate);

	//カメラの座標を決める
	Vector3 newTranslate = worldTransform_.GetTranslate();
	newTranslate.y += 5.5f;
	cameraManager_->GetActiveCamera()->worldTransform.SetTranslate(newTranslate);
}
