#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// ボムリロードノード
/// </summary>
class BombReloadLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit BombReloadLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~BombReloadLeaf();

	// 更新
	void Update() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

