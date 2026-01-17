#pragma once
#include "nodeBase/NodeBase.h"
#include <vector>
#include <memory>

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
	void AddNode(std::unique_ptr<INode> _node);

	//実行中のノードIDの取得
	virtual int GetRunningNodeID() const override;

protected:
	//ノードのインクリメント
	virtual void NodeIncrement();
	//次のインデックスの取得
	virtual const int GetNextIndex() const = 0;

protected:
	// 子ノード群
	std::vector<std::unique_ptr<INode>> mChildNodes;
	// 現在動かしているノードのインデックス
	int mRunningNodeIndex{ 0 };
};

