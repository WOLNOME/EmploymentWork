#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 機関銃弾リロードノード
/// </summary>
class BulletReloadLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit BulletReloadLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~BulletReloadLeaf();

	// 更新
	void Update() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

