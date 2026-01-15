#pragma once
#include "application/behaiviorTree/interface/INode.h"
#include "Vector3.h"

/// <summary>
/// ブラックボード
/// </summary>
struct BlackBoard {
	// 共通情報
	Vector3 selfPosition;
	Vector3 targetPosition;
	float   distanceToTarget;

	// 状態
	float   hp;
	bool    isTargetVisible;
	bool    isInFront;

	// クールタイム
	float   gunCooldown;
	float   bombCooldown;
	float   barrierCooldown;

	// フラグ
	bool    isAlert;
	bool    isPhase2;
};

/// <summary>
/// 全てのノードの基底クラス
/// </summary>
class NodeBase : public INode {
protected:
	explicit NodeBase(BlackBoard* _blackBoard) : mpBlackBoard(_blackBoard) {}
	virtual ~NodeBase() = default;
	virtual void Initialize() override { mNodeResult = NodeResult::Running; }
	virtual void Update() override {}
	virtual void Finalize() override {}

	NodeResult mNodeResult = NodeResult::Idle;  //ノードの状態
	BlackBoard* mpBlackBoard = nullptr; //ブラックボード
};

