#include "CheckBombReload.h"

CheckBombReload::CheckBombReload(BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode) : BranchNodeBase(_blackBoard, std::move(_trueNode), std::move(_falseNode)) {
}

CheckBombReload::~CheckBombReload() {
}

const bool CheckBombReload::IsCondition() {
    return false;
}
