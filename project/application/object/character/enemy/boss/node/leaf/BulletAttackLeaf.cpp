#include "BulletAttackLeaf.h"

BulletAttackLeaf::BulletAttackLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

BulletAttackLeaf::~BulletAttackLeaf() {
}

void BulletAttackLeaf::Update() {
}

NodeResult BulletAttackLeaf::GetNodeResult() const {
    return NodeResult();
}
