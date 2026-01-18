#include "CheckPlayerDistance.h"

CheckPlayerDistance::CheckPlayerDistance(BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode, float _distance) : BranchNodeBase(_blackBoard, std::move(_trueNode), std::move(_falseNode)),
distance_(_distance) {
}

CheckPlayerDistance::~CheckPlayerDistance() {
}

const bool CheckPlayerDistance::IsCondition() {
	return false;
}
