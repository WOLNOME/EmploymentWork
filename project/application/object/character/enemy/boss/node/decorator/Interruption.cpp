#include "Interruption.h"

using namespace Norm;

Interruption::Interruption(int _nodeID, Norm::BlackBoard* _blackBoard, std::unique_ptr<Norm::INode> _childNode) : DecoratorNodeBase(_nodeID, _blackBoard) {
	//子ノードのセット
	SetNode(std::move(_childNode));
}

Interruption::~Interruption() {
}

void Interruption::Initialize() {
	//基底クラスの初期化
	DecoratorNodeBase::Initialize();
}

void Interruption::Update() {
	// 子ノードを実行
	mChildNode->Update();
	// 子ノードの結果を取得
	NodeResult result = mChildNode->GetNodeResult();
	mIsRevaluation = mChildNode->GetIsRevaluation();

	//runningが帰ってきたとき
	bool isInterruption = false;		//中断するか
	if (result == NodeResult::Running) {
		//バリアが破壊されているかor外に外れたかのチェック
		isInterruption = CheckBarrierDestroy()||CheckOutOfSearchRange();
	}

	//中断するなら
	if (isInterruption) {
		//結果をFailにする
		mNodeResult = NodeResult::Fail;
		//再評価通知を送信
		mIsRevaluation = true;

		return;
	}

	mNodeResult = result;
}

void Interruption::Finalize() {
	//基底クラスの終了
	DecoratorNodeBase::Finalize();
}

bool Interruption::CheckBarrierDestroy() {
	//ブラックボードから必要な情報を取得
	bool isBarrier = mpBlackBoard->GetValue<bool>("IsBarrier");
	bool isPreBarrier = mpBlackBoard->GetValue<bool>("IsPreBarrier");

	//バリアが壊れた瞬間かを返す
	return (!isBarrier && isPreBarrier);
}

bool Interruption::CheckOutOfSearchRange() {
	//ブラックボードから必要な情報を取得
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	bossPos.y = 0.0f;
	Vector3 bossPrePos = mpBlackBoard->GetValue<Vector3>("BossPrePos");
	bossPrePos.y = 0.0f;
	Vector3 playerPos = mpBlackBoard->GetValue<Vector3>("PlayerPos");
	playerPos.y = 0.0f;
	Vector3 playerPrePos = mpBlackBoard->GetValue<Vector3>("PlayerPrePos");
	playerPrePos.y = 0.0f;

	//現フレームと前フレームの距離を求める
	const float kSearchDist = 450.0f;
	float dist = Vector3(bossPos - playerPos).Length();
	float preDist = Vector3(bossPrePos - playerPrePos).Length();

	//索敵範囲から外れた瞬間かを返す
	return (dist > kSearchDist && preDist <= kSearchDist);
}
