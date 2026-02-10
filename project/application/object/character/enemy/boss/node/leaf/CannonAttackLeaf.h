#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// キャノン攻撃ノード
/// </summary>
class CannonAttackLeaf : public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit CannonAttackLeaf(int _nodeID, Norm::BlackBoard* _blackBoard);
	// デストラクタ
	~CannonAttackLeaf();

	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	Norm::NodeResult GetNodeResult() const override;
};

