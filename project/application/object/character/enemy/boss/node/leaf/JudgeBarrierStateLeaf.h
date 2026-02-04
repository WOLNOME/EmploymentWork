#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 前フレームと現フレームのバリア状態を比較して、現フレームで壊れていたら成功を返すノード
/// </summary>
class JudgeBarrierStateLeaf : public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit JudgeBarrierStateLeaf(Norm::BlackBoard* _blackBoard);
	// デストラクタ
	~JudgeBarrierStateLeaf();

	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	Norm::NodeResult GetNodeResult() const override;
};

