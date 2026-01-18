#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 感知演出ノード
/// </summary>
class DirectionSensingLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit DirectionSensingLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~DirectionSensingLeaf();

	// 更新
	void Update() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

