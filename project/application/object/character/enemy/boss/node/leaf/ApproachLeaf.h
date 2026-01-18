#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 接近ノード
/// </summary>
class ApproachLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit ApproachLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~ApproachLeaf();

	// 更新
	void Update() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

