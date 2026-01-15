#pragma once
#include "application/behaviorTree/allNodeBase/NodeBase.h"
#include <vector>

/// <summary>
/// Compositeノードの基底クラス
/// </summary>
class CompositeNodeBase : public NodeBase {
public:
	//コンストラクタ
	explicit CompositeNodeBase(BlackBoard* _blackBoard) : NodeBase(_blackBoard) {}
	//デストラクタ
	virtual ~CompositeNodeBase();

	//初期化
	virtual void Initialize() override;
	//終了
	virtual void Finalize() override;

	//ノードの追加
	void AddNode(INode* _node) { mChildNodes.push_back(_node); }

protected:
	//ノードのインクリメント
	void NodeIncrement();
	//次のインデックスの取得
	virtual const int GetNextIndex() const = 0;

protected:
	// 子ノード群
	std::vector<INode*> mChildNodes;
	// 現在動かしているノードのインデックス
	int mRunningNodeIndex{ 0 };
};

