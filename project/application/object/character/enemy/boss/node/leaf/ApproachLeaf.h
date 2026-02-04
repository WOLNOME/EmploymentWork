#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 接近ノード
/// </summary>
class ApproachLeaf : public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit ApproachLeaf(Norm::BlackBoard* _blackBoard);
	// デストラクタ
	~ApproachLeaf();

	// 更新
	void Update() override;
	
	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	Norm::NodeResult GetNodeResult() const override;

};

