#include "BombAttackLeaf.h"

BombAttackLeaf::BombAttackLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

BombAttackLeaf::~BombAttackLeaf() {
}

void BombAttackLeaf::Update() {
}

NodeResult BombAttackLeaf::GetNodeResult() const {
	return NodeResult();
}
