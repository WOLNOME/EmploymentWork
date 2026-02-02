#include "CheckBulletReload.h"

CheckBulletReload::CheckBulletReload(BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode) : BranchNodeBase(_blackBoard, std::move(_trueNode), std::move(_falseNode)) {
}

CheckBulletReload::~CheckBulletReload() {
}

const bool CheckBulletReload::IsCondition() {
	bool result = false;
	//ブラックボードから銃弾の弾倉を取得
	int bulletMagazine = mpBlackBoard->GetValue<int>("BulletMagazine");
	int bulletMaxMagazine = mpBlackBoard->GetValue<int>("BulletMaxMagazine");

	//弾倉が空でなければ撃つ
	if (bulletMagazine != 0) {
		result = true;
	}

	return result;
}
