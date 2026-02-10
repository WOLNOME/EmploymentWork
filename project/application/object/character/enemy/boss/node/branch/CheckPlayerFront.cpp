#include "CheckPlayerFront.h"
#include <MyMath.h>

using namespace Norm;

CheckPlayerFront::CheckPlayerFront(int _nodeID, BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode) : BranchNodeBase(_nodeID, _blackBoard, std::move(_trueNode), std::move(_falseNode)) {
}

CheckPlayerFront::~CheckPlayerFront() {
}

const bool CheckPlayerFront::IsCondition() {
	bool result = false;
	//ブラックボードから必要な情報を取得
	Vector3 playerPos = mpBlackBoard->GetValue<Vector3>("PlayerPos");
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	Vector3 bossRotate = mpBlackBoard->GetValue<Vector3>("BossRotate");

	// ボスからプレイヤーへのベクトルを計算
	Vector3 toPlayer = Vector3(playerPos - bossPos);
	toPlayer.y = 0.0f;
	toPlayer.Normalize();
	// ボスの前方ベクトルを計算（Y軸回転のみ考慮）
	Vector3 bossForward = Vector3(
		std::sin(bossRotate.y),
		0.0f,
		std::cos(bossRotate.y)
	).Normalized();
	// 内積を計算
	float dotProduct = MyMath::Dot(bossForward, toPlayer);
	// 内積が0.984以上(およそ±10°)なら前方にいると判断
	if (dotProduct >= 0.984f) {
		result = true;
	}

    return result;
}
