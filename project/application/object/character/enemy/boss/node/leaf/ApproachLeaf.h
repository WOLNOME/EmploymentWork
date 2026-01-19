#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 接近ノード
/// </summary>
class ApproachLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit ApproachLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~ApproachLeaf();

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	
	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;

};

