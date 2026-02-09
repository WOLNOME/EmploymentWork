#pragma once
#include "compositeNode/CompositeNodeBase.h"

namespace Norm {

	/// <summary>
	/// セレクターノード
	/// </summary>
	class Selector : public CompositeNodeBase {
	public:
		//コンストラクタ
		explicit Selector(BlackBoard* _blackBoard) : CompositeNodeBase(_blackBoard) {}

		//初期化
		void Initialize() override;
		//更新
		void Update() override;
		//終了
		void Finalize() override;

	private:
		//ノードのインクリメント
		void NodeIncrement() override;
	};

}