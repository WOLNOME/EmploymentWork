#include "CompositeNodeBase.h"

CompositeNodeBase::~CompositeNodeBase() {
	//子ノードを削除する
	for (auto node : mChildNodes) {
		delete node;
	}
	mChildNodes.clear();
}

void CompositeNodeBase::Initialize() {
	//基底クラスの初期化
	NodeBase::Initialize();

	mRunningNodeIndex = 0;

	// 最初のノードを初期化
	if (mChildNodes.size() > 0) {
		mChildNodes[mRunningNodeIndex]->Initialize();
	}
	else {
		mNodeResult = NodeResult::Fail;
	}
}

void CompositeNodeBase::Finalize() {
	//終了時に追加したい処理があったらここに書く
}

void CompositeNodeBase::NodeIncrement() {
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
