#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// キャノン再装填ノード
/// </summary>
class CannonReloadLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit CannonReloadLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~CannonReloadLeaf();

	//初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;

private:
	// 最大弾倉数
	const int kCannonMaxMagazine_ = 1;
};

