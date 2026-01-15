#include "Sequence.h"

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
