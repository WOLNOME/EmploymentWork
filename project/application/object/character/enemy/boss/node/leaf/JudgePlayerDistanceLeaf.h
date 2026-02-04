#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// プレイヤーとの距離を判定するノード
/// </summary>
class JudgePlayerDistanceLeaf : public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit JudgePlayerDistanceLeaf(Norm::BlackBoard* _blackBoard, float _distance);
	// デストラクタ
	~JudgePlayerDistanceLeaf();

	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	Norm::NodeResult GetNodeResult() const override;

private:
	// 判定距離
	float distance_ = 0.0f;
};

