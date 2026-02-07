#include "Boss.h"
#include <ImGuiManager.h>
#include <TextureManager.h>
#include <Object3dManager.h>
#include <cassert>

//アプリケーション
#include <application/object/character/enemy/boss/builder/BossBehaivorTreeBuilder.h>
#include <application/object/character/player/Player.h>
#include <application/object/character/enemy/boss/collision/BossCollider.h>

using namespace Norm;

Boss::~Boss() {
}

void Boss::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/boss");

	//モデルの生成・初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Boss"), "boss");
	object3d_->SetIsLightProcess(true);
	object3d_->SetIsDisplay(false);

	//当たり判定の生成・初期化
	collider_ = std::make_unique<BossCollider>(this);
	auto* bossCollider = dynamic_cast<BossCollider*>(collider_.get());
	collider_->SetWorldTransform(&object3d_->worldTransform);
	collider_->SetCollisionAttribute(CollisionAttribute::Nothingness);
	collider_->SetOffset({
		param_["collisionCenterOffsetOBB"]["x"],
		param_["collisionCenterOffsetOBB"]["y"],
		param_["collisionCenterOffsetOBB"]["z"]
		});
	bossCollider->SetOBBSize({
		param_["collisionSizeOBB"]["x"],
		param_["collisionSizeOBB"]["y"],
		param_["collisionSizeOBB"]["z"]
		});

	//影の大きさを調整
	circleShadow_->worldTransform.SetScale({ 16.0f,1.0f,16.0f });
	circleShadow_->SetIsDisplay(false);

	//ブラックボードの生成
	blackBoard_ = std::make_unique<BlackBoard>();
	//ブラックボードに必要な情報を登録
	{
		//定数情報を登録
		ConstantInfoToBlackBoard();
		//変数情報を登録
		VariableInfoToBlackBoard(true);
	}

	//ビヘイビアツリーのノードを生成
	behaviorTreeRoot_ = BossBehaivorTreeBuilder::BuildBehaviorTree(blackBoard_.get());
	behaviorTreeRoot_->Initialize();

	//メンバ変数の初期化
	hp_ = param_["maxHP"];
	isAlive_ = false;
}

void Boss::Update() {
	//プレイヤーがセットされていなかったら警告
	assert(player_ && "プレイヤーがセットされていません。");

	//ベースキャラクターの更新
	BaseCharacter::Update();

	//変数情報をブラックボードに転送
	VariableInfoToBlackBoard(false);

	//ビヘイビアツリーの更新
	behaviorTreeRoot_->Update();

	//ブラックボードから変数情報を取得
	BlackBoardToVariableInfo();

}

void Boss::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

	//ビヘイビアツリーのデバッグ処理
	behaviorTreeRoot_->Debug();

#endif // _DEBUG
}

void Boss::Spawn(const Vector3& _position) {
	//生存状態に変更
	isAlive_ = true;
	//当たり判定を有効化
	collider_->SetCollisionAttribute(CollisionAttribute::Enemy);
	//モデルを表示
	object3d_->SetIsDisplay(true);
	circleShadow_->SetIsDisplay(true);
	//指定座標に出現
	Vector3 position = _position;
	position.y = 16.0f;
	object3d_->worldTransform.SetTranslate(position);
	//ステートをアクティブに変更する
	SetState(State::kActive);
}

void Boss::SetEnemyWeaponManager(EnemyWeaponManager* _enemyWeaponManager) {
	//インスタンスをセット
	enemyWeaponManager_ = _enemyWeaponManager;
	//ブラックボードに書き込む
	blackBoard_->SetValue<EnemyWeaponManager*>("EnemyWeaponManager", enemyWeaponManager_);
}

void Boss::ConstantInfoToBlackBoard() {
	//ボスの情報
	blackBoard_->SetValue<float>("BossSpeed", param_["speed"]);
	blackBoard_->SetValue<float>("BossTurnSpeed", param_["turnSpeed"]);
	blackBoard_->SetValue<float>("BossMaxSpeed", param_["maxSpeed"]);
	blackBoard_->SetValue<int>("BossMaxHP", param_["maxHP"]);
	//武器の情報
	blackBoard_->SetValue<float>("BombReloadTime", param_["bombReloadTime"]);
	blackBoard_->SetValue<float>("CannonReloadTime", param_["cannonReloadTime"]);
	blackBoard_->SetValue<float>("BulletReloadTime", param_["bulletReloadTime"]);
	blackBoard_->SetValue<float>("BulletIntervalTime", param_["bulletIntervalTime"]);
	blackBoard_->SetValue<int>("BulletMaxMagazine", param_["bulletMaxMagazine"]);
	//特殊攻撃の情報
	blackBoard_->SetValue<float>("BarrierCoolTime", param_["barrierCoolTime"]);
	blackBoard_->SetValue<float>("BarrierDirTime", param_["barrierDirTime"]);
	blackBoard_->SetValue<int>("BarrierMaxHP", param_["barrierMaxHP"]);
	blackBoard_->SetValue<float>("SummonCoolTime", param_["summonCoolTime"]);
	blackBoard_->SetValue<float>("SummonDirTime", param_["summonDirTime"]);
	//演出の情報
	blackBoard_->SetValue<float>("ConfusionDirTime", param_["confusionDirTime"]);
	blackBoard_->SetValue<float>("MissingDirTime", param_["missingDirTime"]);
	blackBoard_->SetValue<float>("SensingDirTime", param_["sensingDirTime"]);
	//その他
	blackBoard_->SetValue<float>("KeepDistanceTime", param_["keepDistanceTime"]);
	blackBoard_->SetValue<float>("FloorFriction", floorFriction_);
}

void Boss::VariableInfoToBlackBoard(bool _isInit) {
	//初期化・更新共通処理
	//ボスの情報を入れる
	blackBoard_->SetValue<Vector3>("BossPos", object3d_->worldTransform.GetTranslate());
	blackBoard_->SetValue<Vector3>("BossRotate", object3d_->worldTransform.GetRotate());
	blackBoard_->SetValue<Vector3>("BossVelocity", velocity_);
	blackBoard_->SetValue<Vector3>("BossPrePos", GetWorldTransform().GetPreWorldTranslate());
	//プレイヤーの情報を入れる
	if (player_) {
		blackBoard_->SetValue<Vector3>("PlayerPos", player_->GetWorldTransform().GetTranslate());
		blackBoard_->SetValue<Vector3>("PlayerPrePos", player_->GetWorldTransform().GetPreWorldTranslate());
	}

	//初期化時なら
	if (_isInit) {
		//ボスの情報を入れる
		blackBoard_->SetValue<int>("BossHP", param_["maxHP"]);
		//プレイヤーの情報を入れる
		if (player_) {
			blackBoard_->SetValue<Vector3>("PlayerPos", player_->GetWorldTransform().GetTranslate());
			blackBoard_->SetValue<Vector3>("PlayerPrePos", player_->GetWorldTransform().GetPreWorldTranslate());
		}
		//武器の情報を入れる
		blackBoard_->SetValue<float>("BombReloadTimer", 0.0f);
		blackBoard_->SetValue<int>("BombMagazine", 1);
		blackBoard_->SetValue<float>("CannonReloadTimer", 0.0f);
		blackBoard_->SetValue<int>("CannonMagazine", 1);
		blackBoard_->SetValue<float>("BulletReloadTimer", 0.0f);
		blackBoard_->SetValue<float>("BulletIntervalTimer", 0.0f);
		blackBoard_->SetValue<int>("BulletMagazine", param_["bulletMaxMagazine"]);
		//特殊攻撃の情報を入れる
		blackBoard_->SetValue<bool>("IsBarrier", false);
		blackBoard_->SetValue<bool>("IsPreBarrier", false);
		blackBoard_->SetValue<int>("BarrierHP", 0);
		blackBoard_->SetValue<float>("BarrierDirTimer", 0.0f);
		blackBoard_->SetValue<float>("BarrierCoolTimer", 0.0f);
		blackBoard_->SetValue<float>("SummonDirTimer", 0.0f);
		blackBoard_->SetValue<float>("SummonCoolTimer", 0.0f);
		//演出の情報を入れる
		blackBoard_->SetValue<float>("ConfusionDirTimer", 0.0f);
		blackBoard_->SetValue<float>("MissingDirTimer", 0.0f);
		blackBoard_->SetValue<float>("SensingDirTimer", 0.0f);
		//その他
		blackBoard_->SetValue<float>("KeepDistanceTimer", 0.0f);
	}
	//更新時なら
	else {
		//ボスの情報を入れる
		blackBoard_->SetValue<int>("BossHP", hp_);
		//プレイヤーの情報を入れる
		if (player_) {
			blackBoard_->SetValue<Vector3>("PlayerPos", player_->GetWorldTransform().GetTranslate());
			blackBoard_->SetValue<Vector3>("PlayerPrePos", player_->GetWorldTransform().GetPreWorldTranslate());
		}
		//その他ノード以外でいじった情報はここに記入
	}
}

void Boss::BlackBoardToVariableInfo() {
	//ボスの情報を取得
	object3d_->worldTransform.SetTranslate(blackBoard_->GetValue<Vector3>("BossPos"));
	object3d_->worldTransform.SetRotate(blackBoard_->GetValue<Vector3>("BossRotate"));
	velocity_ = blackBoard_->GetValue<Vector3>("BossVelocity");
	hp_ = blackBoard_->GetValue<int>("BossHP");
}

