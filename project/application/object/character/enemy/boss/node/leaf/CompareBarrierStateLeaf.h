#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// バリアの状態を比較するノード
/// </summary>
class CompareBarrierStateLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit CompareBarrierStateLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~CompareBarrierStateLeaf();

	// 更新
	void Update() override;

	// ノードの状態を取得
	NodeResult GetNodeResult () const override;
};

