#pragma once
#include "branchNode/BranchNodeBase.h"

/// <summary>
/// ボムのリロードをチェックするノード
/// </summary>
class CheckBombReload : public BranchNodeBase {
public:
	// コンストラクタ
	explicit CheckBombReload(BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode);
	// デストラクタ
	~CheckBombReload();

private:
	// 条件を満たしているかの取得
	const bool IsCondition() override;

private:
};

