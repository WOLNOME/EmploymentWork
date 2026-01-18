#include "CannonAttackLeaf.h"

CannonAttackLeaf::CannonAttackLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

CannonAttackLeaf::~CannonAttackLeaf() {
}

void CannonAttackLeaf::Update() {
}

NodeResult CannonAttackLeaf::GetNodeResult() const {
    return NodeResult();
}
