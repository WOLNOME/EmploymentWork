#include "CheckCannonReload.h"

CheckCannonReload::CheckCannonReload(BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode) : BranchNodeBase(_blackBoard, std::move(_trueNode), std::move(_falseNode)) {
}

CheckCannonReload::~CheckCannonReload() {
}

const bool CheckCannonReload::IsCondition() {
	bool result = false;
	//ブラックボードからキャノンのリロードタイムを取得
	float cannonReloadTime = mpBlackBoard->GetValue<float>("CannonReloadTime");

	//リロードタイムが0ならリロード完了
	if (cannonReloadTime == 0.0f) {
		result = true;
	}

    return result;
}
