#pragma once
#include "branchNode/BranchNodeBase.h"

/// <summary>
/// プレイヤーが前方にいるかチェックするノード
/// </summary>
class CheckPlayerFront :
    public BranchNodeBase {
public:
	// コンストラクタ
	explicit CheckPlayerFront(BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode);
	// デストラクタ
	~CheckPlayerFront();

private:
	// 条件を満たしているかの取得
	const bool IsCondition() override;

private:
	
};

