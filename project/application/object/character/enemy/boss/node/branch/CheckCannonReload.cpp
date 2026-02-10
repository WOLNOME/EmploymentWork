#include "CheckCannonReload.h"

using namespace Norm;

CheckCannonReload::CheckCannonReload(int _nodeID, BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode) : BranchNodeBase(_nodeID, _blackBoard, std::move(_trueNode), std::move(_falseNode)) {
}

CheckCannonReload::~CheckCannonReload() {
}

const bool CheckCannonReload::IsCondition() {
	bool result = false;
	//ブラックボードから砲弾の弾倉を取得
	int cannonMagazine = mpBlackBoard->GetValue<int>("CannonMagazine");

	//弾倉が1ならリロード完了
	if (cannonMagazine == 1) {
		result = true;
	}

	return result;
}
