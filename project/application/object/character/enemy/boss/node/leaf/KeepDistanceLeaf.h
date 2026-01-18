#pragma once
#include "leafNode/LeafNodeBase.h"
class KeepDistanceLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit KeepDistanceLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~KeepDistanceLeaf();

	// 更新
	void Update() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

