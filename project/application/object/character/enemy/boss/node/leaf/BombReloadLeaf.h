#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// ボムリロードノード
/// </summary>
class BombReloadLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit BombReloadLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~BombReloadLeaf();

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
	const int kBombMaxMagazine_ = 1;
};

