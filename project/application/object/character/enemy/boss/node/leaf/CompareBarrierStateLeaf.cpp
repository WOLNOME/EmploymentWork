#include "CompareBarrierStateLeaf.h"

CompareBarrierStateLeaf::CompareBarrierStateLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

CompareBarrierStateLeaf::~CompareBarrierStateLeaf() {
}

void CompareBarrierStateLeaf::Update() {
}

NodeResult CompareBarrierStateLeaf::GetNodeResult() const {
    return NodeResult();
}
