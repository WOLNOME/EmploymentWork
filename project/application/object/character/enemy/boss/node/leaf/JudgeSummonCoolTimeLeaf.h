#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 召喚のクールタイムを判定するノード
/// </summary>
class JudgeSummonCoolTimeLeaf : public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit JudgeSummonCoolTimeLeaf(Norm::BlackBoard* _blackBoard);
	// デストラクタ
	~JudgeSummonCoolTimeLeaf();

	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	Norm::NodeResult GetNodeResult() const override;
};

