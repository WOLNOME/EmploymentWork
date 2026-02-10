#pragma once
#include "leafNode/LeafNodeBase.h"
#include <MyMath.h>

/// <summary>
/// 混乱演出ノード
/// </summary>
class DirectionConfusionLeaf : public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit DirectionConfusionLeaf(int _nodeID, Norm::BlackBoard* _blackBoard);
	// デストラクタ
	~DirectionConfusionLeaf();

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	Norm::NodeResult GetNodeResult() const override;

private:
	//混乱処理
	void Confusion(float _confusionTimer);
	//回復処理
	void Recovery(float _confusionTimer);

private:
	//ベースの回転量
	Norm::Vector3 baseRotate_ = {};
	//混乱回転の最終的な回転量
	Norm::Vector3 confusionEndRotate_ = {};

	//回転タイマー
	const float kCycleTime_ = 2.3f;
	float cycleTimer_ = 0.0f;

	//振幅
	const float kAmplitudeX_ = (1.0f / 12.0f) * Norm::pi;
	const float kAmplitudeY_ = (2.5f / 12.0f) * Norm::pi;

	//回復タイマー
	const float kRecoveryTime_ = 1.0f;
	float recoveryTimer_ = 0.0f;

};

