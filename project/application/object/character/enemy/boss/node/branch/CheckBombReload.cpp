#include "CheckBombReload.h"

CheckBombReload::CheckBombReload(BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode) : BranchNodeBase(_blackBoard, std::move(_trueNode), std::move(_falseNode)) {
}

CheckBombReload::~CheckBombReload() {
}

const bool CheckBombReload::IsCondition() {
	bool result = false;
	// ブラックボードからボムのクールタイムを取得
	const float bombCoolTime = mpBlackBoard->GetValue<float>("BombCoolTime");

	// ボムのクールタイムが0ならリロード可能
	if (bombCoolTime == 0.0f) {
		result = true;
	}

    return result;
}
