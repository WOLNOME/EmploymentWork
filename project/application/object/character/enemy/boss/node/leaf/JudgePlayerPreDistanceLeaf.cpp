#include "JudgePlayerPreDistanceLeaf.h"

JudgePlayerPreDistanceLeaf::JudgePlayerPreDistanceLeaf(BlackBoard* _blackBoard, float _preDistance) : LeafNodeBase(_blackBoard), preDistance_(_preDistance) {
}

JudgePlayerPreDistanceLeaf::~JudgePlayerPreDistanceLeaf() {
}

void JudgePlayerPreDistanceLeaf::Update() {
}

NodeResult JudgePlayerPreDistanceLeaf::GetNodeResult() const {
	return NodeResult();
}
