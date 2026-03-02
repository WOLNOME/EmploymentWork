#pragma once
#include "leafNode/LeafNodeBase.h"
#include <CombinedParticle.h>
#include <Object3d.h>
#include "WorldTransform.h"
#include <memory>

/// <summary>
/// バリアノード
/// </summary>
class BarrierLeaf : public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit BarrierLeaf(int _nodeID, Norm::BlackBoard* _blackBoard);
	// デストラクタ
	~BarrierLeaf();

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
	//展開中パーティクル
	std::unique_ptr<Norm::CombinedParticle> barrierMidst_ = nullptr;
	//展開完了パーティクル
	std::unique_ptr<Norm::CombinedParticle> barrierComplete_ = nullptr;

	//バリア演出オブジェクト
	std::unique_ptr<Norm::Object3d> dirObject_ = nullptr;
	Norm::WorldTransform dirWorldTransform_;

};

