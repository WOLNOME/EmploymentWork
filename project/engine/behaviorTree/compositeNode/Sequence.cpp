#include "Sequence.h"

Sequence::Sequence(BlackBoard* _blackBoard)
	: CompositeNodeBase(_blackBoard) {
}

Sequence::~Sequence() {
}

void Sequence::Initialize() {
	//基底クラスの初期化
	CompositeNodeBase::Initialize();
}

void Sequence::Update() {
	//稼働中の子ノードの更新
	mChildNodes[mRunningNodeIndex]->Update();
	auto result = mChildNodes[mRunningNodeIndex]->GetNodeResult();

	if (result == NodeResult::Success) {
		// 次回Sequenceに向けてノード番号を進める
		NodeIncrement();
		return;
	}

	// もし失敗が返されたらノード終了
	if (result == NodeResult::Fail) {
		Finalize();
	}

	mNodeResult = result;
}

void Sequence::Finalize() {
	//基底クラスの終了
	CompositeNodeBase::Finalize();
}

const int Sequence::GetNextIndex() const {
	return mRunningNodeIndex + 1;
}

void Sequence::NodeIncrement() {
	// 現在のノードの後始末
	mChildNodes[mRunningNodeIndex]->Finalize();
	// インデックスを進める
	mRunningNodeIndex = GetNextIndex();
	// もしすべての子ノードを試しても成功したら
	if (mRunningNodeIndex > mChildNodes.size() - 1) {
		mNodeResult = NodeResult::Success;
		Finalize();
		return;
	}
	// 次に回すノードの初期化
	mChildNodes[mRunningNodeIndex]->Initialize();
}
