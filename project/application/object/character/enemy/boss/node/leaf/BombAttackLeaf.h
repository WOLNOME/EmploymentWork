#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 爆弾攻撃ノード
/// </summary>
class BombAttackLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit BombAttackLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~BombAttackLeaf();

	// 更新
	void Update() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

