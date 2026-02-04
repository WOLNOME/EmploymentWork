#pragma once
#include "branchNode/BranchNodeBase.h"

/// <summary>
/// ボムのリロードをチェックするノード
/// </summary>
class CheckBombReload : public Norm::BranchNodeBase {
public:
	// コンストラクタ
	explicit CheckBombReload(Norm::BlackBoard* _blackBoard, std::unique_ptr<Norm::INode> _trueNode, std::unique_ptr<Norm::INode> _falseNode);
	// デストラクタ
	~CheckBombReload();

private:
	// 条件を満たしているかの取得
	const bool IsCondition() override;

private:
};

