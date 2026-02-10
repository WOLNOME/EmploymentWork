#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// パトロールノード
/// </summary>
class PatrolLeaf : public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit PatrolLeaf(int _nodeID, Norm::BlackBoard* _blackBoard);
	// デストラクタ
	~PatrolLeaf();

	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	Norm::NodeResult GetNodeResult() const override;
};

