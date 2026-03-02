#pragma once
#include <Sprite.h>
#include <memory>

/// <summary>
/// チュートリアルシーンのUIや演出、操作等を管理するクラス
/// </summary>
class TutorialSystem {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui();

private:

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//
	uint32_t textureHandle_;
	std::unique_ptr<Norm::Sprite> sprite_ = nullptr;

	int i = 0;

};

