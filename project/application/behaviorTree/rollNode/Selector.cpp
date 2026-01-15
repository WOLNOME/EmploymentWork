#include "Selector.h"

void Selector::Initialize() {
	//基底クラスの初期化
	CompositeNodeBase::Initialize();
}

void Selector::Update() {
	//稼働中の子ノードの更新
	mChildNodes[mRunningNodeIndex]->Update();
	auto result = mChildNodes[mRunningNodeIndex]->GetNodeResult();

	if (result == NodeResult::Fail) {
		// 次回Sequenceに向けてノード番号を進める
		NodeIncrement();
		return;
	}

	// もし成功が返されたらノード終了
	if (result == NodeResult::Success) {
		Finalize();
	}

	mNodeResult = result;
}

void Selector::Finalize() {
	//基底クラスの終了処理
	CompositeNodeBase::Finalize();
}
