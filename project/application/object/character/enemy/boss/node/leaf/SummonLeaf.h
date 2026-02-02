#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 召喚ノード
/// </summary>
class SummonLeaf : public LeafNodeBase {
public:
	// コンストラクタ
	explicit SummonLeaf(BlackBoard* _blackBoard);
	// デストラクタ
	~SummonLeaf();

	//初期化
	void Initialize() override;
	// 更新
	void Update() override;
	//終了
	void Finalize() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	NodeResult GetNodeResult() const override;
};

