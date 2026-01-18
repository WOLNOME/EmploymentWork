#include "CheckBulletReload.h"

CheckBulletReload::CheckBulletReload(BlackBoard* _blackBoard, std::unique_ptr<INode> _trueNode, std::unique_ptr<INode> _falseNode) : BranchNodeBase(_blackBoard, std::move(_trueNode), std::move(_falseNode)) {
}

CheckBulletReload::~CheckBulletReload() {
}

const bool CheckBulletReload::IsCondition() {
	bool result = false;
	// ブラックボードから機関銃弾クールタイム取得
	float bulletCoolTime = mpBlackBoard->GetValue<float>("BulletCoolTime");

	// クールタイムが0ならリロード可能
	if (bulletCoolTime == 0.0f) {
		result = true;
	}

	return result;
}
