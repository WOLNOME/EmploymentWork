#pragma once
#include "branchNode/BranchNodeBase.h"

/// <summary>
/// プレイヤーが前方にいるかチェックするノード
/// </summary>
class CheckPlayerFront :
    public Norm::BranchNodeBase {
public:
	// コンストラクタ
	explicit CheckPlayerFront(Norm::BlackBoard* _blackBoard, std::unique_ptr<Norm::INode> _trueNode, std::unique_ptr<Norm::INode> _falseNode);
	// デストラクタ
	~CheckPlayerFront();

private:
	// 条件を満たしているかの取得
	const bool IsCondition() override;

private:
	
};

