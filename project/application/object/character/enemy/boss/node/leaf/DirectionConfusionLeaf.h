#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 混乱演出ノード
/// </summary>
class DirectionConfusionLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit DirectionConfusionLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~DirectionConfusionLeaf();

	// 更新
	void Update() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

