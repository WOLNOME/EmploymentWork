#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// 機関銃弾リロードノード
/// </summary>
class BulletReloadLeaf : public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit BulletReloadLeaf(Norm::BlackBoard* _blackBoard);
	// デストラクタ
	~BulletReloadLeaf();

	//初期化
	void Initialize() override;
	// 更新
	void Update() override;

	// デバッグ
	void Debug() override;

	// ノードの状態を取得
	Norm::NodeResult GetNodeResult() const override;
};

