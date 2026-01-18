#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// キャノン攻撃ノード
/// </summary>
class CannonAttackLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit CannonAttackLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~CannonAttackLeaf();

	// 更新
	void Update() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

