#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 見失った演出ノード
/// </summary>
class DirectionMissingLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit DirectionMissingLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~DirectionMissingLeaf();

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

