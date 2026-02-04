#include "CheckBombReload.h"

using namespace Norm;

CheckBombReload::CheckBombReload(BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode) : BranchNodeBase(_blackBoard, std::move(_trueNode), std::move(_falseNode)) {
}

CheckBombReload::~CheckBombReload() {
}

const bool CheckBombReload::IsCondition() {
	bool result = false;
	//ブラックボードから爆弾の弾倉を取得
	int bombMagazine = mpBlackBoard->GetValue<int>("BombMagazine");

	//弾倉が1ならリロード完了
	if (bombMagazine == 1) {
		result = true;
	}

	return result;
}
