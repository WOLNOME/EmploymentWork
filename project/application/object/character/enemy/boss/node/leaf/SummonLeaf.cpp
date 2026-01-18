#include "SummonLeaf.h"

SummonLeaf::SummonLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

SummonLeaf::~SummonLeaf() {
}

void SummonLeaf::Update() {
}

NodeResult SummonLeaf::GetNodeResult() const {
    return NodeResult();
}
