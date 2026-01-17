#pragma once
#include <memory>

//前方宣言
class INode;
class BlackBoard;

/// <summary>
/// ボスのビヘイビアツリービルダー
/// </summary>
class BossBehaivorTreeBuilder {
public:
	static std::unique_ptr<INode> BuildBehaviorTree(BlackBoard* _blackBoard);
};

