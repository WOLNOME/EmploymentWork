#include "DecoratorNodeBase.h"

namespace Norm {

	DecoratorNodeBase::DecoratorNodeBase(BlackBoard* _blackBoard) : NodeBase(_blackBoard) {
	}

	DecoratorNodeBase::~DecoratorNodeBase() {
	}

	void DecoratorNodeBase::Initialize() {
		//基底クラスの初期化
		NodeBase::Initialize();
		//子ノードの初期化
		mChildNode->Initialize();
	}

	void DecoratorNodeBase::Finalize() {
		//基底クラスの終了
		NodeBase::Finalize();
		//子ノードの終了
		mChildNode->Finalize();
	}

	void DecoratorNodeBase::SetNode(std::unique_ptr<INode> _node) {
		mChildNode = std::move(_node);
	}

	int DecoratorNodeBase::GetRunningNodeID() const {
		// 子ノードの実行中ノードIDを返す
		return mChildNode->GetRunningNodeID();
	}

}