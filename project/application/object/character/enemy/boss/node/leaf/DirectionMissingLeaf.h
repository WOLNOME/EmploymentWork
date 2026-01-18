#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 見失った演出ノード
/// </summary>
class DirectionMissingLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit DirectionMissingLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~DirectionMissingLeaf();

	// 更新
	void Update() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

