#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// キャノン再装填ノード
/// </summary>
class CannonReloadLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit CannonReloadLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~CannonReloadLeaf();

	// 更新
	void Update() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

