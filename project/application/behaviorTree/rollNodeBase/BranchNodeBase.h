#pragma once
#include "application/behaviorTree/allNodeBase/NodeBase.h"

/// <summary>
/// 条件に応じてTrue/Falseのいずれかの子を実行するためのノード
/// </summary>
class BranchNodeBase : public NodeBase {
public:
	//コンストラクタ
	explicit BranchNodeBase(BlackBoard* black_board, INode* true_node, INode* false_node) : NodeBase(black_board) {
		mpBranchNodes[0] = true_node;
		mpBranchNodes[1] = false_node;
	}
	//デストラクタ
	virtual ~BranchNodeBase();
	//初期化
	virtual void Initialize() override;
	//更新
	virtual void Update() override;
	//終了
	virtual void Finalize() override;

protected:
	//状態フラグの取得
	virtual const bool IsCondition() = 0;

protected:
	INode* mpBranchNodes[2] = { nullptr, nullptr }; // True,Falseそれぞれのノード
	int mSatisfyIndex = -1; // 条件を満たしているノードのインデックス
};

