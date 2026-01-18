#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// バリアノード
/// </summary>
class BarrierLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit BarrierLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~BarrierLeaf();

	// 更新
	void Update() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

