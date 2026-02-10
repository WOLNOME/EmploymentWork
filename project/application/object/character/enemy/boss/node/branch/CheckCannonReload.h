#pragma once
#include "branchNode/BranchNodeBase.h"

/// <summary>
/// キャノンのリロードをチェックするノード
/// </summary>
class CheckCannonReload : public Norm::BranchNodeBase {
public:
	// コンストラクタ
	explicit CheckCannonReload(int _nodeID, Norm::BlackBoard* _blackBoard, std::unique_ptr<Norm::INode> _trueNode, std::unique_ptr<Norm::INode> _falseNode);
	// デストラクタ
	~CheckCannonReload();

private:
	// 条件を満たしているかの取得
	const bool IsCondition() override;

private:
};

