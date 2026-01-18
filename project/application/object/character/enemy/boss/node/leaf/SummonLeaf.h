#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 召喚ノード
/// </summary>
class SummonLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit SummonLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~SummonLeaf();

	// 更新
	void Update() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

