#pragma once
#include "leafNode/LeafNodeBase.h"
#include <array>

/// <summary>
/// 召喚ノード
/// </summary>
class SummonLeaf : public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit SummonLeaf(int _nodeID, Norm::BlackBoard* _blackBoard);
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
	Norm::NodeResult GetNodeResult() const override;

private:
	//回転処理
	void Rotate();

private:
	//召喚する座標
	static const int kSummonNum = 2;
	std::array < Norm::Vector3, kSummonNum > summonPositions_;
};

