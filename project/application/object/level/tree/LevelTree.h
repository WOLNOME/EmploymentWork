#pragma once
#include "application/object/level/base/IBaseLevelObject.h"
#include <cstdint>
#include <unordered_map>

/// <summary>
/// ツリーオブジェクトの管理クラス
/// </summary>
class LevelTree : public IBaseLevelObject {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	LevelTree() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~LevelTree() override = default;
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_name">名前</param>
	/// <param name="_filePath">ファイルパス</param>
	void Initialize(const std::string& _name, const std::string& _filePath) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// ImGui操作
	/// </summary>
	void DebugWithImGui() override;

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// アクションフラグの取得
	/// </summary>
	/// <returns>アクションフラグ</returns>
	bool GetIsAction() { return isAction_; }

	/// <summary>
	/// 倒れる方向の取得
	/// </summary>
	/// <returns>倒れる方向</returns>
	const Norm::Vector3& GetInvertDirection() { return invertDirection_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// アクションフラグの設定
	/// </summary>
	/// <param name="_isAction">アクション中かどうか</param>
	void SetIsAction(bool _isAction) { isAction_ = _isAction; }

	/// <summary>
	/// 倒れる方向の設定
	/// </summary>
	/// <param name="_direction">倒れる方向</param>
	void SetInvertDirection(const Norm::Vector3& _direction) { invertDirection_ = _direction; }

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// 木が倒れる処理
	/// </summary>
	void TreeInvertProcess();

	/// ============================== ///
	///		メンバ変数(private)
	/// ============================== ///

	//ツリーオブジェクト限定変数
	std::unordered_map<uint32_t, bool> isActions_;	// 倒れるアクションをするかどうか
	Norm::Vector3 invertDirection_ = { 0.0f,0.0f,0.0f };	// 倒れる方向
	const float time_ = 1.0f;
	float timer_ = 0.0f;	// タイマー

};

