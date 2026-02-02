#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// パトロールノード
/// </summary>
class PatrolLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit PatrolLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~PatrolLeaf();

	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

