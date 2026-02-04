#pragma once
#include "leafNode/LeafNodeBase.h"
class KeepDistanceLeaf : public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit KeepDistanceLeaf(Norm::BlackBoard* _blackBoard);
	// デストラクタ
	~KeepDistanceLeaf();

	//初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	Norm::NodeResult GetNodeResult() const override;
};

