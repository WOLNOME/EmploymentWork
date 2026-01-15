#pragma once
#include "application/behaviorTree/rollNodeBase/CompositeNodeBase.h"

/// <summary>
/// シークエンスノード
/// </summary>
class Sequence : public CompositeNodeBase {
public:
	//コンストラクタ
	explicit Sequence(BlackBoard* _blackBoard) : CompositeNodeBase(_blackBoard) {}

	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//終了
	void Finalize() override;

private:
	//次のインデックスの取得
	const int GetNextIndex() const override { return mRunningNodeIndex + 1; }
};

