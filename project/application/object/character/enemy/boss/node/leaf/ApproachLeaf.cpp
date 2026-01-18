#include "ApproachLeaf.h"

ApproachLeaf::ApproachLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

ApproachLeaf::~ApproachLeaf() {
}

void ApproachLeaf::Update() {
}

NodeResult ApproachLeaf::GetNodeResult() const {
    return NodeResult();
}
