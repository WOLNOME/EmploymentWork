#pragma once
#include "application/object/level/base/IBaseLevelObject.h"
#include <cstdint>
#include <unordered_map>

/// <summary>
/// ツリーオブジェクトの管理クラス
/// </summary>
class LevelTree : public IBaseLevelObject {
private:
	/// ============================== ///
	///		構造体
	/// ============================== ///

	/// <summary>
	/// ツリー1つのデータ
	/// </summary>
	struct TreeData {
		bool isAction;
		Norm::Vector3 invertDirection;
		float invertTimer;
	};

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
	void Initialize(const std::string& _name) override;
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
	/// ツリーデータコンテナの取得
	/// </summary>
	/// <returns></returns>
	const std::unordered_map<uint32_t, TreeData>& GetTreeDatas() { return treeDatas_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// アクションフラグの設定
	/// </summary>
	/// <param name="_handle">ハンドル</param>
	/// <param name="_isAction">アクション中かどうか</param>
	void SetIsAction(uint32_t _handle, bool _isAction) { treeDatas_[_handle].isAction = _isAction; }

	/// <summary>
	/// 倒れる方向の設定
	/// </summary>
	/// <param name="_handle">ハンドル</param>
	/// <param name="_direction">倒れる方向</param>
	void SetInvertDirection(uint32_t _handle, const Norm::Vector3& _direction) { treeDatas_[_handle].invertDirection = _direction; }

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

	//ツリーデータのコンテナ
	std::unordered_map<uint32_t, TreeData> treeDatas_;

	const float kInvertTime_ = 1.0f;		//倒木にかかる時間

};

