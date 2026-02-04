#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 見失った演出ノード
/// </summary>
class DirectionMissingLeaf : public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit DirectionMissingLeaf(Norm::BlackBoard* _blackBoard);
	// デストラクタ
	~DirectionMissingLeaf();

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	Norm::NodeResult GetNodeResult() const override;
};

