#pragma once
#include "application/behaviorTree/allNodeBase/NodeBase.h"

/// <summary>
/// 条件分岐ノードの基底クラス
/// </summary>
class DecoratorNodeBase : public NodeBase {
public:
    //コンストラクタ
    explicit DecoratorNodeBase(BlackBoard* black_board) : NodeBase(black_board) {}
    //デストラクタ
    virtual ~DecoratorNodeBase();

    //初期化
    virtual void Initialize() override;
    //更新
    virtual void Finalize() override;

    //子ノードのセット
    void SetNode(INode* node) { mChildNode = node; }

protected:
    INode* mChildNode = nullptr;
};

