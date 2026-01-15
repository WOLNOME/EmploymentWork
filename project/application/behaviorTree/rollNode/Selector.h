#pragma once
#include "application/behaiviorTree/rollNodeBase/CompositeNodeBase.h"
class Selector : public CompositeNodeBase {
public:
	explicit Selector(BlackBoard* _blackBoard) : CompositeNodeBase(_blackBoard) {}

	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//終了
	void Finalize() override;

private:
	const int GetNextIndex() const override { return mRunningNodeIndex + 1; }
};

