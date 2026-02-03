#pragma once
#include "nodeBase/NodeBase.h"

namespace Norm {

	/// <summary>
	/// 葉ノード（Actionノード）の基底クラス
	/// </summary>
	class LeafNodeBase : public NodeBase {
	protected:
		//コンストラクタ
		explicit LeafNodeBase(BlackBoard* _blackBoard);
		//デストラクタ
		virtual ~LeafNodeBase() = default;
	};

}