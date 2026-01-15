#pragma once
#include "application/behaiviorTree/allNodeBase/NodeBase.h"
#include <vector>

/// <summary>
/// Compositeノードの基底クラス
/// </summary>
class CompositeNodeBase : public NodeBase {
public:
	explicit CompositeNodeBase(BlackBoard* _blackBoard) : NodeBase(_blackBoard) {}
	virtual ~CompositeNodeBase();

	virtual void Initialize() override;
	virtual void Finalize() override;

	void AddNode(INode* _node) { mChildNodes.push_back(_node); }

protected:
	void NodeIncrement();

	virtual const int GetNextIndex() const = 0; // 派生クラスで実装

protected:
	// 子ノード群
	std::vector<INode*> mChildNodes;
	// 現在動かしているノードのインデックス
	int mRunningNodeIndex{ 0 };
};

