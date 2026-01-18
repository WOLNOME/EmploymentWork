#include "KeepDistanceLeaf.h"

KeepDistanceLeaf::KeepDistanceLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

KeepDistanceLeaf::~KeepDistanceLeaf() {
}

void KeepDistanceLeaf::Update() {
}

NodeResult KeepDistanceLeaf::GetNodeResult() const {
    return NodeResult();
}
