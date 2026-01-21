#include "CheckBulletReload.h"

CheckBulletReload::CheckBulletReload(BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode) : BranchNodeBase(_blackBoard, std::move(_trueNode), std::move(_falseNode)) {
}

CheckBulletReload::~CheckBulletReload() {
}

const bool CheckBulletReload::IsCondition() {
	bool result = false;
	// ブラックボードから機関銃弾リロードタイム取得
	float bulletReloadTime = mpBlackBoard->GetValue<float>("BulletReloadTime");

	// リロードタイムが0ならリロード可能
	if (bulletReloadTime == 0.0f) {
		result = true;
	}

	return result;
}
