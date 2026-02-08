#pragma once
#include "leafNode/LeafNodeBase.h"
#include "Object3d.h"
#include <memory>

/// <summary>
/// バリアノード
/// </summary>
class BarrierLeaf : public Norm::LeafNodeBase {
public:
	// コンストラクタ
	explicit BarrierLeaf(Norm::BlackBoard* _blackBoard);
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
	//バリア演出オブジェクト
	std::unique_ptr<Norm::Object3d> dirObject_ = nullptr;

};

