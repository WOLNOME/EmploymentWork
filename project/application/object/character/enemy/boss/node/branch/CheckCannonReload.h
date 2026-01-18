#pragma once
#include "branchNode/BranchNodeBase.h"

/// <summary>
/// キャノンのリロードをチェックするノード
/// </summary>
class CheckCannonReload : public BranchNodeBase {
public:
	// コンストラクタ
	explicit CheckCannonReload(BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode);
	// デストラクタ
	~CheckCannonReload();

private:
	// 条件を満たしているかの取得
	const bool IsCondition() override;

private:
};

