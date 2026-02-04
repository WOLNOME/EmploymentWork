#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 混乱演出ノード
/// </summary>
class DirectionConfusionLeaf : public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit DirectionConfusionLeaf(Norm::BlackBoard* _blackBoard);
	// デストラクタ
	~DirectionConfusionLeaf();

	//初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	Norm::NodeResult GetNodeResult() const override;
};

