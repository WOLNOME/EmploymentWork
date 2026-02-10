#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// HPが半分以下か判定するノード
/// </summary>
class JudgeHPHalfLeaf : public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit JudgeHPHalfLeaf(int _nodeID, Norm::BlackBoard* _blackBoard);
	// デストラクタ
	~JudgeHPHalfLeaf();

	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	Norm::NodeResult GetNodeResult() const override;
};

