#pragma once
#include "leafNode/LeafNodeBase.h"
#include <CombinedParticle.h>
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

	static const int kSummonNum = 2;
	//召喚中パーティクル
	std::array<std::unique_ptr<Norm::CombinedParticle>,kSummonNum> summonMidst_;
	//召喚完了パーティクル
	std::array<std::unique_ptr<Norm::CombinedParticle>,kSummonNum> summonComplete_;

	//召喚する座標
	std::array < Norm::Vector3, kSummonNum > summonPositions_;
};

