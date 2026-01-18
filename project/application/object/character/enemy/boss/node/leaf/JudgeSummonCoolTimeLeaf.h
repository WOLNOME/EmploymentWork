#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 召喚のクールタイムを判定するノード
/// </summary>
class JudgeSummonCoolTimeLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit JudgeSummonCoolTimeLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~JudgeSummonCoolTimeLeaf();

	// 更新
	void Update() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

