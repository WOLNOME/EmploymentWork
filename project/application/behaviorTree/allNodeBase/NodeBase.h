#pragma once
#include "application/behaviorTree/interface/INode.h"
#include "application/behaviorTree/blackBoard/BlackBoard.h"

/// <summary>
/// 全てのノードの基底クラス
/// </summary>
class NodeBase : public INode {
protected:
	//コンストラクタ
	explicit NodeBase(BlackBoard* _blackBoard) : mpBlackBoard(_blackBoard) {}
	//仮想デストラクタ
	virtual ~NodeBase() = default;
	//初期化
	virtual void Initialize() override { mNodeResult = NodeResult::Running; }
	//更新
	virtual void Update() override {}
	//終了
	virtual void Finalize() override {}

	NodeResult mNodeResult = NodeResult::Idle;  //ノードの状態
	BlackBoard* mpBlackBoard = nullptr; //ブラックボード
};

