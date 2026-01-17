#pragma once
#include "nodeBase/NodeBase.h"
#include <memory>

/// <summary>
/// 条件分岐ノードの基底クラス
/// </summary>
class DecoratorNodeBase : public NodeBase {
public:
    //コンストラクタ
    explicit DecoratorNodeBase(BlackBoard* _blackBoard);
    //デストラクタ
    virtual ~DecoratorNodeBase();

    //初期化
    virtual void Initialize() override;
    //更新
    virtual void Finalize() override;

    //子ノードのセット
    void SetNode(std::unique_ptr<INode> _node);
	//実行中のノードIDの取得
	int GetRunningNodeID() const override;

protected:
    std::unique_ptr<INode> mChildNode = nullptr;
};

