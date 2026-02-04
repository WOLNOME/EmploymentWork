#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// プレイヤーの前フレームとの距離を判定するノード
/// </summary>
class JudgePlayerPreDistanceLeaf :
    public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit JudgePlayerPreDistanceLeaf(Norm::BlackBoard* _blackBoard,float _preDistance);
	// デストラクタ
	~JudgePlayerPreDistanceLeaf();

	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	Norm::NodeResult GetNodeResult() const override;

private:
	float preDistance_ = 0.0f;		// プレイヤーの前フレームとの距離
};

