#pragma once
#include "compositeNode/CompositeNodeBase.h"

namespace Norm {

	/// <summary>
	/// シークエンスノード
	/// </summary>
	class Sequence : public CompositeNodeBase {
	public:
		//コンストラクタ
		explicit Sequence(BlackBoard* _blackBoard);
		//デストラクタ
		~Sequence();

		//初期化
		void Initialize() override;
		//更新
		void Update() override;
		//終了
		void Finalize() override;

	private:
		//次のインデックスの取得
		const int GetNextIndex() const override;
		//ノードのインクリメント
		void NodeIncrement() override;
	};

}