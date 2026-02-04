#pragma once
#include "leafNode/LeafNodeBase.h"

/// <summary>
/// キャノン再装填ノード
/// </summary>
class CannonReloadLeaf : public Norm::LeafNodeBase {
public:
	/// ============================== ///
	///		メンバ関数（public）
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_blackBoard">ブラックボード</param>
	explicit CannonReloadLeaf(Norm::BlackBoard* _blackBoard);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~CannonReloadLeaf();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// デバッグ
	/// </summary>
	void Debug() override;

	/// <summary>
	/// ノードの状態を取得
	/// </summary>
	/// <returns></returns>
	Norm::NodeResult GetNodeResult() const override;

private:
	/// ============================== ///
	///		メンバ関数（private）
	/// ============================== ///

	/// <summary>
	/// リロード
	/// </summary>
	void Reload();
	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();


	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	// 最大弾倉数
	const int kCannonMaxMagazine_ = 1;
};

