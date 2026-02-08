#include "BarrierLeaf.h"
#include "ImGuiManager.h"
#include <cstdint>
#include <TextureManager.h>
#include <Object3dManager.h>
#include <MyMath.h>

//アプリケーション
#include <application/object/character/enemy/boss/Barrier.h>

using namespace Norm;

BarrierLeaf::BarrierLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
	//バリア演出オブジェクトの生成・初期化
	uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("sky.png");
	dirObject_ = std::make_unique<Object3d>();
	dirObject_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Barrier"), Shape::ShapeKind::kSphere);
	dirObject_->SetTexture(textureHandle);
	dirObject_->SetIsDisplay(false);
	dirObject_->SetIsLightProcess(false);

}

BarrierLeaf::~BarrierLeaf() {
}

void BarrierLeaf::Initialize() {
	//基底クラスの初期化
	LeafNodeBase::Initialize();

	//ブラックボードの情報を初期化
	mpBlackBoard->SetValue<bool>("IsBarrier", false);
	float barrierDirTime = mpBlackBoard->GetValue<float>("BarrierDirTime");
	mpBlackBoard->SetValue<float>("BarrierDirTimer", barrierDirTime);
}

void BarrierLeaf::Update() {
	//ブラックボードから必要な情報を取得
	Barrier* barrier = mpBlackBoard->GetValue<Barrier*>("Barrier");
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	bool isBarrier = mpBlackBoard->GetValue<bool>("IsBarrier");
	float barrierDirTime = mpBlackBoard->GetValue<float>("BarrierDirTime");
	float barrierDirTimer = mpBlackBoard->GetValue<float>("BarrierDirTimer");
	float barrierCoolTime = mpBlackBoard->GetValue<float>("BarrierCoolTime");
	float barrierCoolTimer = mpBlackBoard->GetValue<float>("BarrierCoolTimer");
	int barrierHP = mpBlackBoard->GetValue<int>("BarrierHP");
	int barrierMaxHP = mpBlackBoard->GetValue<int>("BarrierMaxHP");

	//演出タイマーを更新
	barrierDirTimer -= kDeltaTime;

	//演出中の処理
	{
		//サイズを定義
		float size = 0.0f;

		//演出オブジェクトを表示
		dirObject_->SetIsDisplay(true);

		//サイズをタイマーに合わせて線形補完
		float targetSize = barrier->GetParam()["collisionRadiusSphere"];
		size = MyMath::Lerp(0.0f, targetSize, MyMath::EaseOutCubic(1.0f - (barrierDirTimer / barrierDirTime)));

		//オブジェクトに座標を指定
		dirObject_->worldTransform.SetTranslate(bossPos);

		//サイズをオブジェクトに適用
		dirObject_->worldTransform.SetScale({ size,size ,size });

	}

	//演出タイマーが0以下になったらバリアを貼る
	if (barrierDirTimer <= 0.0f) {
		//バリアを貼る
		isBarrier = true;
		//バリア演出タイマーをリセット
		barrierDirTimer = 0.0f;
		//バリアのクールタイムをセット
		barrierCoolTimer = barrierCoolTime;
		//バリアのHPを最大にセット
		barrierHP = barrierMaxHP;
		//演出オブジェクトを非表示
		dirObject_->SetIsDisplay(false);
		//バリアをスポーン
		barrier->Spawn(bossPos);
	}

	//ブラックボードに更新した情報を保存
	mpBlackBoard->SetValue<bool>("IsBarrier", isBarrier);
	mpBlackBoard->SetValue<float>("BarrierDirTimer", barrierDirTimer);
	mpBlackBoard->SetValue<float>("BarrierCoolTimer", barrierCoolTimer);
	mpBlackBoard->SetValue<int>("BarrierHP", barrierHP);
}

void BarrierLeaf::Finalize() {
	//基底クラスの終了処理
	LeafNodeBase::Finalize();
}

void BarrierLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("バリア");
	ImGui::End();
#endif // _DEBUG
}

NodeResult BarrierLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	bool isBarrier = mpBlackBoard->GetValue<bool>("IsBarrier");

	//バリアがアクティブでないならrunningを返す
	if (!isBarrier) {
		return NodeResult::Running;
	}
	//バリアがアクティブならsuccessを返す
	return NodeResult::Success;
}
