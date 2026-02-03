#include "Player.h"
#include "WinApp.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "Object3dManager.h"
#include <LevelLoader.h>
#include <algorithm>
#include <cassert>

//アプリケーション
#include <application/object/character/weapon/player/manager/PlayerWeaponManager.h>
#include <application/ui/player/PlayerUI.h>
#include <application/ui/message/MessageUI.h>

void Player::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//インプットの初期化
	input_ = Input::GetInstance();
	input_->SetIsMouseDisplay(false);
	input_->SetIsMouseFixed(true);
	//インスタンスの生成と初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Player"), "player");

	deathDirection_ = std::make_unique<DeathDirection>();
	deathDirection_->Initialize();

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/player");

	//当たり判定の形状を設定
	collisionShapeKind_ = Collider::CollisionShapeKind::OBB;
	//当たり判定のパラメーター入力
	collisionCenterOffsetOBB_ = { param_["collisionCenterOffsetOBB"]["x"],param_["collisionCenterOffsetOBB"]["y"] ,param_["collisionCenterOffsetOBB"]["z"] };
	collisionSizeOBB_ = { param_["collisionSizeOBB"]["x"],param_["collisionSizeOBB"]["y"] ,param_["collisionSizeOBB"]["z"] };

	//当たり判定の属性を設定
	SetCollisionAttribute(CollisionAttribute::Player);

	//アクティブ状態として初期化
	SetState(State::kActive);

	//パラメータのセット
	int maxHP = param_["maxHP"];
	hp_ = maxHP;
	cannonReloadTimer_ = 0.0f;
	bulletReloadTimer_ = 0.0f;
	int bulletMaxNum = param_["bulletMagazine"];
	bulletNum_ = bulletMaxNum;
	bulletFireIntervalTimer_ = 0.0f;
	item_reloadSpeedUp_ = 0;
	item_moveSpeedUp_ = 0;
	item_turnSpeedUp_ = 0;

}

void Player::Update() {
	//カメラがセットされていなかったら警告
	assert(camera_ && "カメラがセットされていません。");
	//メッセージUIがセットされていなかったら警告
	assert(messageUI_ && "メッセージUIがセットされていません。");

	//ベースキャラクターの更新
	BaseCharacter::Update();
	//ダメージ更新
	isDamage_ = false;

	//回転処理
	Rotate();
	//移動処理
	Move();
	//攻撃処理
	CannonAttack();
	BulletAttack();
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

	//当たり判定可視化用ラインの色を変更
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };

#endif // _DEBUG

}

void Player::OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) {
	//ローカル変数
	int maxHP = param_["maxHP"];
	int item_maxNum = param_["item_maxNum"];
	//当たり判定時の処理
	switch (attribute) {
	case CollisionAttribute::Enemy:
	{
		//HPを減らす
		hp_ -= param_["tackleDamage"];
		//0~MaxHPの範囲に収める
		hp_ = std::clamp(hp_, 0, maxHP);
		//カメラシェイクを入れるmaxHP
		camera_->RegistShake(0.4f, 0.8f);

		//ダメージヒット
		isDamage_ = true;

		//相手の座標の方向と反対方向のベクトルを速度に加算
		Vector3 reflectVec = -(subjectPos - GetWorldPosition()).Normalized() * 10.0f;
		reflectVelocity_.x = reflectVec.x;
		reflectVelocity_.z = reflectVec.z;

		break;
	}
	case CollisionAttribute::EnemyCannon:
		//HPを減らす
		hp_ -= param_["cannonDamage"];
		//0~MaxHPの範囲に収める
		hp_ = std::clamp(hp_, 0, maxHP);
		//カメラシェイクを入れる
		camera_->RegistShake(0.4f, 0.8f);

		//ダメージヒット
		isDamage_ = true;

		break;
	case CollisionAttribute::EnemyBullet:
		//HPを減らす
		hp_ -= param_["bulletDamage"];
		//0~MaxHPの範囲に収める
		hp_ = std::clamp(hp_, 0, maxHP);
		//カメラシェイクを入れる
		camera_->RegistShake(0.2f, 0.3f);

		//ダメージヒット
		isDamage_ = true;

		break;
	case CollisionAttribute::EnemyBlast:
	{
		//HPを減らす
		hp_ -= param_["bombDamage"];
		//0~MaxHPの範囲に収める
		hp_ = std::clamp(hp_, 0, maxHP);
		//カメラシェイクを入れるmaxHP
		camera_->RegistShake(0.4f, 0.8f);

		//ダメージヒット
		isDamage_ = true;

		//相手の座標の方向と反対方向のベクトルを速度に加算(大きく)
		Vector3 reflectVec = -(subjectPos - GetWorldPosition()).Normalized() * 40.0f;
		reflectVelocity_.x = reflectVec.x;
		reflectVelocity_.z = reflectVec.z;

		break;
	}
	case CollisionAttribute::Item_Heal:
	{
		//HPを回復
		int healValue = param_["item_healValue"];
		hp_ += healValue;
		//0~MaxHPの範囲に収める
		hp_ = std::clamp(hp_, 0, maxHP);
		//メッセージUIにアイテム取得を通知
		std::wstring message = L"HPを" + std::to_wstring(healValue) + L"回復！";
		messageUI_->AddMessage(message);
		break;
	}
	case CollisionAttribute::Item_ReloadSpeedUp:
		//アイテムが制限を超えていなければ取得
		if (item_reloadSpeedUp_ < (uint32_t)item_maxNum) {
			//アイテム取得
			item_reloadSpeedUp_++;
			//メッセージUIにアイテム取得を通知
			std::wstring message = L"リロード速度UP！";
			messageUI_->AddMessage(message);
		}

		break;
	case CollisionAttribute::Item_MoveSpeedUp:
		//アイテムが制限を超えていなければ取得
		if (item_moveSpeedUp_ < (uint32_t)item_maxNum) {
			//アイテム取得
			item_moveSpeedUp_++;
			//メッセージUIにアイテム取得を通知
			std::wstring message = L"移動速度UP！";
			messageUI_->AddMessage(message);
		}

		break;
	case CollisionAttribute::Item_TurnSpeedUp:
		//アイテムが制限を超えていなければ取得
		if (item_turnSpeedUp_ < (uint32_t)item_maxNum) {
			//アイテム取得
			item_turnSpeedUp_++;
			//メッセージUIにアイテム取得を通知
			std::wstring message = L"回転速度UP！";
			messageUI_->AddMessage(message);
		}

		break;
	default:
		break;
	}
}

void Player::SetLevelLoader(LevelLoader* _levelLoader) {
	//プレイヤーの座標を読み込む
	for (const auto& playerSpawnData : _levelLoader->GetPlayerSpawnData()) {
		object3d_->worldTransform.SetTranslate(playerSpawnData.translation);
		object3d_->worldTransform.SetRotate(playerSpawnData.rotation);

		//最初のデータのみを読み込む
		break;
	}
}

void Player::Rotate() {
	//死亡演出中なら処理をしない
	if (deathDirection_->GetIsDirection())
		return;
	//アクティブでないなら処理をしない
	if (state_ != State::kActive)
		return;

	auto ShortestAngleDiff = [=](float from, float to) -> float {
		float diff = to - from;
		while (diff > pi)  diff -= 2.0f * pi;
		while (diff < -pi) diff += 2.0f * pi;
		return diff;
		};

	float cameraRotateY = camera_->worldTransform.GetRotate().y;
	float vehicleRotateY = object3d_->worldTransform.GetRotate().y;

	//最短角度差を求める
	float angleDiff = ShortestAngleDiff(vehicleRotateY, cameraRotateY);

	//回転速度の上限
	float turnSpeed = param_["turnSpeed"];
	float item_turnSpeedUpValue = param_["item_turnSpeedUpValue"];
	turnSpeed += item_turnSpeedUp_ * item_turnSpeedUpValue;
	float addRotation = turnSpeed * kDeltaTime;

	//回転すべき角度が小さい場合は目標角度を代入
	if (std::abs(angleDiff) <= addRotation) {
		vehicleRotateY = cameraRotateY;
	}
	else {
		//回転方向に応じて加算または減算
		vehicleRotateY += (angleDiff > 0 ? 1 : -1) * addRotation;

		//-π ～ π に整える
		if (vehicleRotateY > pi)  vehicleRotateY -= 2.0f * pi;
		if (vehicleRotateY < -pi) vehicleRotateY += 2.0f * pi;
	}

	//オブジェクトの水平回転量に代入
	Vector3 rotate = object3d_->worldTransform.GetRotate();
	rotate.y = vehicleRotateY;
	object3d_->worldTransform.SetRotate(rotate);

}

void Player::Move() {
	//死亡演出中なら処理をしない
	if (deathDirection_->GetIsDirection())
		return;
	//アクティブでないなら処理をしない
	if (state_ != State::kActive)
		return;

	//移動前に前フレームの座標を保存
	prePosition_ = object3d_->worldTransform.GetWorldTranslate();

	//現在の向き(水平向きのみを考慮)
	Vector3 currentDir = {
		std::sinf(object3d_->worldTransform.GetRotate().y),
		0.0f,
		std::cosf(object3d_->worldTransform.GetRotate().y)
	};
	currentDir.Normalize();
	//WSキー入力で前後移動
	float speed = param_["speed"];
	float item_moveSpeedUpValue = param_["item_moveSpeedUpValue"];
	speed += item_moveSpeedUp_ * item_moveSpeedUpValue;
	if (input_->PushKey(DIK_W) || (input_->GetLStickDir().y > 0.0f)) {
		//速度を加算
		velocity_ += currentDir * speed;
	}
	if (input_->PushKey(DIK_S) || (input_->GetLStickDir().y < 0.0f)) {
		//速度を減算
		velocity_ += -currentDir * speed;
	}

	//床の抵抗値を加算
	Vector3 frictionDir = -velocity_.Normalized();
	Vector3 frictionAccel = frictionDir * floorFriction_;
	velocity_ += frictionAccel * kDeltaTime;

	//移動量の大きさを制限
	float maxSpeed_ = param_["maxSpeed"];
	float item_maxMoveSpeedUpValue = param_["item_maxMoveSpeedUpValue"];
	maxSpeed_ += item_moveSpeedUp_ * item_maxMoveSpeedUpValue;
	if (velocity_.Length() > maxSpeed_) {
		velocity_.Normalize();
		velocity_ *= maxSpeed_;
	}
	//移動量の小ささを制限
	if (Vector3(velocity_ * kDeltaTime).Length() < 0.01f) {
		velocity_ = { 0.0f,0.0f,0.0f };
	}

	//反発速度を加算
	velocity_ += reflectVelocity_;

	//反発速度を徐々に減衰させる
	if (reflectVelocity_.x != 0.0f || reflectVelocity_.y != 0.0f || reflectVelocity_.z != 0.0f) {
		//0に近づける
		Vector3 decayDir = -reflectVelocity_.Normalized();
		Vector3 decayAccel = decayDir * 40.0f;
		reflectVelocity_ += decayAccel * kDeltaTime;
		if (reflectVelocity_.Length() < 1.0f) {
			reflectVelocity_ = { 0.0f,0.0f,0.0f };
		}
	}

	//速度を加算
	Vector3 newTranslate = object3d_->worldTransform.GetTranslate() + velocity_ * kDeltaTime;
	object3d_->worldTransform.SetTranslate(newTranslate);
	object3d_->worldTransform.SetTranslate(newTranslate);
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
		//計算後はこの関数を抜ける
		return;
	}

	//スペースキーで砲弾を発射
	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerPadButton(GamePadButton::A)) {
		//リロードタイムをセット
		float cannonReloadTime = param_["cannonReloadTime"];
		float reloadSpeedUpValue = param_["item_reloadSpeedUpValue"];
		cannonReloadTime -= item_reloadSpeedUp_ * reloadSpeedUpValue;
		cannonReloadTimer_ = cannonReloadTime;
		//初期位置と発射方向の計算
		float orx = camera_->worldTransform.GetRotate().x;
		float ory = camera_->worldTransform.GetRotate().y;
		Vector3 currentDir = {
			std::cosf(orx) * std::sinf(ory),
			-std::sinf(orx),		//←角度
			std::cosf(orx) * std::cosf(ory)
		};
		currentDir.Normalize();
		Vector3 cannonPos = object3d_->worldTransform.GetTranslate();
		cannonPos.y += 1.7f;	//砲弾の初期位置を調整
		Vector3 cannonDirection = currentDir;
		//スポーン
		playerWeaponManager_->SpawnCannon(cannonPos, cannonDirection);
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
		//計算後はこの関数を抜ける
		return;
	}

	//左クリックで銃弾を発射
	if (input_->PushMouseButton(MouseButton::LeftButton) || (input_->GetRT() > 0.5f)) {
		//間隔計測用タイマーをセット
		float bulletFireIntervalTime = param_["bulletFireIntervalTime"];
		bulletFireIntervalTimer_ = bulletFireIntervalTime;
		//現在の銃弾数を減らす
		bulletNum_--;
		//銃弾数が0になったらリロードタイマーをセット
		if (bulletNum_ <= 0) {
			float bulletReloadTime = param_["bulletReloadTime"];
			float reloadSpeedUpValue = param_["item_reloadSpeedUpValue"];
			bulletReloadTime -= item_reloadSpeedUp_ * reloadSpeedUpValue;
			bulletReloadTimer_ = bulletReloadTime;
		}
		//初期位置と発射方向を計算
		float orx = camera_->worldTransform.GetRotate().x;
		float ory = camera_->worldTransform.GetRotate().y;
		Vector3 currentDir = {
			std::cosf(orx) * std::sinf(ory),
			-std::sinf(orx),		//←角度
			std::cosf(orx) * std::cosf(ory)
		};
		currentDir.Normalize();
		Vector3 bulletPos = camera_->worldTransform.GetTranslate();
		bulletPos += currentDir * 8.0f;	//銃弾の初期位置を調整
		//スポーン
		playerWeaponManager_->SpawnBullet(bulletPos, currentDir);
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

	//カメラの操作にオブジェクトの回転を合わせる
	Vector2 moveValue;
	Vector2 mouseMoveValue = input_->GetMousePosition();
	Vector2 padMoveValue = input_->GetRStickDir() * 40.0f;
	padMoveValue.y *= -1.0f;
	moveValue = mouseMoveValue + padMoveValue;
	//デッドゾーン
	float deadZone = 2.5f;
	if (moveValue.Length() > deadZone) {
		Vector3 newRotate = camera_->worldTransform.GetRotate();
		newRotate.x += moveValue.y * (0.0005f + (item_turnSpeedUp_ * 0.1f * 0.0005f));
		newRotate.y += moveValue.x * (0.0005f + (item_turnSpeedUp_ * 0.1f * 0.0005f));
		camera_->worldTransform.SetRotate(newRotate);
	}
	//回転制限
	const float maxPitch = (pi / 30.0f);		//下向き制限
	const float minPitch = -(pi / 9.0f);		//上向き制限
	Vector3 newRotate = camera_->worldTransform.GetRotate();
	newRotate.x = std::clamp(camera_->worldTransform.GetRotate().x, minPitch, maxPitch);
	//水平回転をπ~-πの間に収める
	if (camera_->worldTransform.GetRotate().y > pi)
		newRotate.y -= pi * 2.0f;
	else if (camera_->worldTransform.GetRotate().y < -pi)
		newRotate.y += pi * 2.0f;
	camera_->worldTransform.SetRotate(newRotate);

	//カメラの座標を決める
	Vector3 newTranslate = object3d_->worldTransform.GetTranslate();
	newTranslate.y += 0.5f;
	camera_->worldTransform.SetTranslate(newTranslate);
}
