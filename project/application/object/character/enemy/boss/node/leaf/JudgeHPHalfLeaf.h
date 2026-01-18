#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// HPが半分以下か判定するノード
/// </summary>
class JudgeHPHalfLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit JudgeHPHalfLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~JudgeHPHalfLeaf();

	// 更新
	void Update() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

