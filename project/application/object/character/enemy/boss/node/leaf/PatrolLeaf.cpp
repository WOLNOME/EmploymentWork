#include "PatrolLeaf.h"

PatrolLeaf::PatrolLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

PatrolLeaf::~PatrolLeaf() {
}

void PatrolLeaf::Update() {
}

NodeResult PatrolLeaf::GetNodeResult() const {
    return NodeResult();
}
