#pragma once
#include "interface/INode.h"
#include "blackBoard/BlackBoard.h"

/// <summary>
/// 各ノードが継承する基底クラス
/// </summary>
class NodeBase : public INode {
public: 
	//ノードIDのセット
	void SetNodeID(const int id) override { mNodeID = id; }

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
	//デバッグ
	virtual void Debug() override {}

	//実行中のノードIDの取得
	virtual NodeResult GetNodeResult() const override { return mNodeResult; }
	//実行中のノードIDの取得
	virtual int GetRunningNodeID() const override { return mNodeID; }

	NodeResult mNodeResult = NodeResult::Idle;  //ノードの状態
	BlackBoard* mpBlackBoard = nullptr; //ブラックボード

private:
	int mNodeID = -1; //ノードID
};

