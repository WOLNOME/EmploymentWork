#include "BombReloadLeaf.h"

BombReloadLeaf::BombReloadLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

BombReloadLeaf::~BombReloadLeaf() {
}

void BombReloadLeaf::Update() {
}

NodeResult BombReloadLeaf::GetNodeResult() const {
	return NodeResult();
}
