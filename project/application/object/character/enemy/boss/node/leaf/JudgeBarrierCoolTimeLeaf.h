#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// バリアの状態を比較するノード
/// </summary>
class JudgeBarrierCoolTimeLeaf : public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit JudgeBarrierCoolTimeLeaf(int _nodeID, Norm::BlackBoard* _blackBoard);
	// デストラクタ
	~JudgeBarrierCoolTimeLeaf();

	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	Norm::NodeResult GetNodeResult () const override;
};

