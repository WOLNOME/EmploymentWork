#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 爆弾攻撃ノード
/// </summary>
class BombAttackLeaf : public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit BombAttackLeaf(Norm::BlackBoard* _blackBoard);
	// デストラクタ
	~BombAttackLeaf();

	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	Norm::NodeResult GetNodeResult() const override;
};

