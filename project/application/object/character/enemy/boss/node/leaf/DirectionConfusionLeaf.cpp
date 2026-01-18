#include "DirectionConfusionLeaf.h"

DirectionConfusionLeaf::DirectionConfusionLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

DirectionConfusionLeaf::~DirectionConfusionLeaf() {
}

void DirectionConfusionLeaf::Update() {
}

NodeResult DirectionConfusionLeaf::GetNodeResult() const {
    return NodeResult();
}
