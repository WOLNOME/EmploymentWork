#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include "JsonUtil.h"
#include "interface/INode.h"
#include "blackBoard/BlackBoard.h"
#include <memory>

//前方宣言
class Player;

/// <summary>
/// ボスクラス
/// </summary>
class Boss : public BaseCharacter {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Boss() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Boss();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// デバッグ用パラメータ調整
	/// </summary>
	void DebugWithImGui() override;

private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///



	/// ============================== ///
	///		メンバ変数(private)
	/// ============================== ///
	
	//パラメーター
	json param_;

	//ビヘイビアツリー
	std::unique_ptr<INode> behaviorTreeRoot_ = nullptr;
	//ブラックボード
	std::unique_ptr<BlackBoard> blackBoard_ = nullptr;


};

