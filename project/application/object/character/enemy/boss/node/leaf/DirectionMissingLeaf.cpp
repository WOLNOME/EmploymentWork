#include "DirectionMissingLeaf.h"

DirectionMissingLeaf::DirectionMissingLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

DirectionMissingLeaf::~DirectionMissingLeaf() {
}

void DirectionMissingLeaf::Update() {
}

NodeResult DirectionMissingLeaf::GetNodeResult() const {
    return NodeResult();
}
