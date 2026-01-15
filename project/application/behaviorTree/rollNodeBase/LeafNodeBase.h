#pragma once
#include "application/behaviorTree/allNodeBase/NodeBase.h"

/// <summary>
/// 葉ノード（Actionノード）の基底クラス
/// </summary>
class LeafNodeBase : public NodeBase {
protected:
	//コンストラクタ
	explicit LeafNodeBase(BlackBoard* black_board) : NodeBase{ black_board } {}
	//デストラクタ
	virtual ~LeafNodeBase() = default;
};

