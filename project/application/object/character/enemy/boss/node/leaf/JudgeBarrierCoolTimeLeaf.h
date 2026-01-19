#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// バリアの状態を比較するノード
/// </summary>
class JudgeBarrierCoolTimeLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit JudgeBarrierCoolTimeLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~JudgeBarrierCoolTimeLeaf();

	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	NodeResult GetNodeResult () const override;
};

