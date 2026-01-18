#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 弾丸攻撃ノード
/// </summary>
class BulletAttackLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit BulletAttackLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~BulletAttackLeaf();

	// 更新
	void Update() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

