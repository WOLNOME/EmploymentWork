#include "Selector.h"

namespace Norm {

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

	const int Selector::GetNextIndex() const {
		return mRunningNodeIndex + 1;
	}

	void Selector::NodeIncrement() {
		// 現在のノードの後始末
		mChildNodes[mRunningNodeIndex]->Finalize();
		// インデックスを進める
		mRunningNodeIndex = GetNextIndex();
		// もしすべての子ノードを試しても失敗したら
		if (mRunningNodeIndex > mChildNodes.size() - 1) {
			mNodeResult = NodeResult::Fail;
			Finalize();
			return;
		}
		// 次に回すノードの初期化
		mChildNodes[mRunningNodeIndex]->Initialize();
	}

}