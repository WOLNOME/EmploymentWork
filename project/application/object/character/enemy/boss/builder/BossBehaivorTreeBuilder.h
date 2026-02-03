#pragma once
#include <memory>

//前方宣言（エンジン層）
namespace Norm {
	class INode;
	class BlackBoard;
}

/// <summary>
/// ボスのビヘイビアツリービルダー
/// </summary>
class BossBehaivorTreeBuilder {
public:
	static std::unique_ptr<Norm::INode> BuildBehaviorTree(Norm::BlackBoard* _blackBoard);
};

