#include "CheckPlayerDistance.h"
#include <MyMath.h>

using namespace Norm;

CheckPlayerDistance::CheckPlayerDistance(BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode, float _distance) : BranchNodeBase(_blackBoard, std::move(_trueNode), std::move(_falseNode)),
distance_(_distance) {
}

CheckPlayerDistance::~CheckPlayerDistance() {
}

const bool CheckPlayerDistance::IsCondition() {
	bool result = false;
	// ブラックボードからプレイヤーの座標とボスの座標を取得
	Vector3 playerPos = mpBlackBoard->GetValue<Vector3>("PlayerPos");
	playerPos.y = 0.0f;
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	bossPos.y = 0.0f;
	float playerDistance = Vector3(playerPos - bossPos).Length();

	// プレイヤーとの距離が設定した距離以下かどうかを判定
	if (playerDistance <= distance_) {
		result = true;
	}

	return result;
}
