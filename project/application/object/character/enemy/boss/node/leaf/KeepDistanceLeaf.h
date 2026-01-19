#pragma once
#include "leafNode/LeafNodeBase.h"
class KeepDistanceLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit KeepDistanceLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~KeepDistanceLeaf();

	//初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

