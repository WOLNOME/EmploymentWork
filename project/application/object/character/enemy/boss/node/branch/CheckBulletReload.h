#pragma once
#include "branchNode/BranchNodeBase.h"

/// <summary>
/// 弾薬がリロード可能かチェックするノード
/// </summary>
class CheckBulletReload : public BranchNodeBase {
public:
	// コンストラクタ
	explicit CheckBulletReload(BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode);
	// デストラクタ
	~CheckBulletReload();

private:
	// 条件を満たしているかの取得
	const bool IsCondition() override;

private:
};

