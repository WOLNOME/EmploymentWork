#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 弾丸攻撃ノード
/// </summary>
class BulletAttackLeaf : public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit BulletAttackLeaf(Norm::BlackBoard* _blackBoard);
	// デストラクタ
	~BulletAttackLeaf();

	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	Norm::NodeResult GetNodeResult() const override;
};

