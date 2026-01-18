#include "CheckCannonReload.h"

CheckCannonReload::CheckCannonReload(BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode) : BranchNodeBase(_blackBoard, std::move(_trueNode), std::move(_falseNode)) {
}

CheckCannonReload::~CheckCannonReload() {
}

const bool CheckCannonReload::IsCondition() {
    return false;
}
