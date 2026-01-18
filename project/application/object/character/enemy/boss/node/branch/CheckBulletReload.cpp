#include "CheckBulletReload.h"

CheckBulletReload::CheckBulletReload(BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode) : BranchNodeBase(_blackBoard, std::move(_trueNode), std::move(_falseNode)) {
}

CheckBulletReload::~CheckBulletReload() {
}

const bool CheckBulletReload::IsCondition() {
	return false;
}
