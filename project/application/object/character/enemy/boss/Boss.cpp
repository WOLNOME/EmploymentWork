#include "Boss.h"
#include <ImGuiManager.h>
#include <TextureManager.h>
#include <Object3dManager.h>
#include <cassert>

//アプリケーション
#include <application/object/character/enemy/boss/builder/BossBehaivorTreeBuilder.h>
#include <application/object/character/player/Player.h>

Boss::~Boss() {
}

void Boss::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/boss");

	//当たり判定の形状を設定
	collisionShapeKind_ = CollisionShapeKind::OBB;
	//当たり判定の属性を設定
	SetCollisionAttribute(CollisionAttribute::Enemy);
	//当たり判定のパラメーター入力
	collisionCenterOffsetOBB_ = {
		param_["collisionCenterOffsetOBB"]["x"],
		param_["collisionCenterOffsetOBB"]["y"],
		param_["collisionCenterOffsetOBB"]["z"]
	};
	collisionSizeOBB_ = {
		param_["collisionSizeOBB"]["x"],
		param_["collisionSizeOBB"]["y"],
		param_["collisionSizeOBB"]["z"]
	};

	//モデルの生成・初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Boss"), "boss");

	//影の大きさを調整
	circleShadow_->worldTransform.scale = { 16.0f,1.0f,16.0f };

	//ブラックボードの生成
	blackBoard_ = std::make_unique<BlackBoard>();
	//ブラックボードに必要な情報を登録
	{
		//定数情報を登録
		ConstantInfoToBlackBoard();
		//変数情報を登録
		VariableInfoToBlackBoard();
	}

	//ビヘイビアツリーのノードを生成
	behaviorTreeRoot_ = BossBehaivorTreeBuilder::BuildBehaviorTree(blackBoard_.get());
	behaviorTreeRoot_->Initialize();
}

void Boss::Update() {
	//プレイヤーがセットされていなかったら警告
	assert(player_ && "プレイヤーがセットされていません。");


	//ベースキャラクターの更新
	BaseCharacter::Update();

	//ビヘイビアツリーの更新
	behaviorTreeRoot_->Update();


}

void Boss::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

	//ビヘイビアツリーのデバッグ処理
	behaviorTreeRoot_->Debug();


#endif // _DEBUG
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

void Boss::VariableInfoToBlackBoard() {
	//ボスの情報を入れる
	blackBoard_->SetValue<Vector3>("BossPos", object3d_->worldTransform.translate);
	blackBoard_->SetValue<Vector3>("BossRotate", object3d_->worldTransform.rotate);
	blackBoard_->SetValue<Vector3>("BossVelocity", velocity_);
	blackBoard_->SetValue<int>("BossHP", param_["maxHP"]);
	blackBoard_->SetValue<Vector3>("BossPrePos", GetPreWorldPosition());
	//プレイヤーの情報を入れる
	if (player_) {
		blackBoard_->SetValue<Vector3>("PlayerPos", player_->GetWorldTransform().translate);
		blackBoard_->SetValue<Vector3>("PlayerPrePos", player_->GetPreWorldPosition());
	}
	//武器の情報を入れる
	blackBoard_->SetValue<float>("BombReloadTimer", 0.0f);
	blackBoard_->SetValue<float>("CannonReloadTimer", 0.0f);
	blackBoard_->SetValue<float>("BulletReloadTimer", 0.0f);
	blackBoard_->SetValue<int>("BulletMaxMagazine", param_["bulletMaxMagazine"]);
	//特殊攻撃の情報を入れる
	blackBoard_->SetValue<bool>("IsBarrier", false);
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

