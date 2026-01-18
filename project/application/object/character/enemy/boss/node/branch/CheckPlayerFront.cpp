#include "CheckPlayerFront.h"

CheckPlayerFront::CheckPlayerFront(BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode) : BranchNodeBase(_blackBoard, std::move(_trueNode), std::move(_falseNode)) {
}

CheckPlayerFront::~CheckPlayerFront() {
}

const bool CheckPlayerFront::IsCondition() {
    return false;
}
