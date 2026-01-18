#include "BarrierLeaf.h"

BarrierLeaf::BarrierLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

BarrierLeaf::~BarrierLeaf() {
}

void BarrierLeaf::Update() {
}

NodeResult BarrierLeaf::GetNodeResult() const {
    return NodeResult();
}
