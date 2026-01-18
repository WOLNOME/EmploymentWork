#pragma once
#include "branchNode/BranchNodeBase.h"

/// <summary>
/// プレイヤーとの距離をチェックするノード
/// </summary>
class CheckPlayerDistance : public BranchNodeBase {
public:
	// コンストラクタ
	explicit CheckPlayerDistance(BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode,float _distance);
	// デストラクタ
	~CheckPlayerDistance();

private:
	// 条件を満たしているかの取得
	const bool IsCondition() override;

private:
	float distance_ = 0.0f;		// 計測するプレイヤーとの距離
};

