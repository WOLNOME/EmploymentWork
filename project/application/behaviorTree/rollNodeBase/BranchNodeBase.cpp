#include "BranchNodeBase.h"

BranchNodeBase::~BranchNodeBase() {
	// ブランチノードの配列を解放
	for (int i = 0; i < 2; ++i) {
		if (mpBranchNodes[i] != nullptr) {
			delete mpBranchNodes[i];
			mpBranchNodes[i] = nullptr;
		}
	}
}

void BranchNodeBase::Initialize() {
	//基底クラスの初期化
	NodeBase::Initialize();

	if (IsCondition()) mSatisfyIndex = 0;
	else mSatisfyIndex = 1;

	mpBranchNodes[mSatisfyIndex]->Initialize();
}

void BranchNodeBase::Update() {
	//条件を満たしているノードの更新
	mpBranchNodes[mSatisfyIndex]->Update();
	mNodeResult = mpBranchNodes[mSatisfyIndex]->GetNodeResult();
}

void BranchNodeBase::Finalize() {
	//基底クラスの終了
	NodeBase::Finalize();
	mpBranchNodes[mSatisfyIndex]->Finalize();
	mSatisfyIndex = -1;
}
