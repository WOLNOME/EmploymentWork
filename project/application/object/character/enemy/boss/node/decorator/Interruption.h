#pragma once
#include "decoratorNode/DecoratorNodeBase.h"
#include <memory>

/// <summary>
/// 条件を満たした時に処理を中断して再評価するノード
/// </summary>
class Interruption : public Norm::DecoratorNodeBase {
public:
	//コンストラクタ
	explicit Interruption(int _nodeID, Norm::BlackBoard* _blackBoard, std::unique_ptr < Norm::INode> _childNode);
	//デストラクタ
	~Interruption();

	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//終了
	void Finalize() override;

private:
	//バリア
	bool CheckBarrierDestroy();
	//索敵範囲から外れたか
	bool CheckOutOfSearchRange();

};

