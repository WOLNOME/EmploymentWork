#pragma once
#include <Audio.h>
#include <Sprite.h>
#include <Handle.h>
#include <cstdint>
#include <array>
#include <memory>

/// <summary>
/// チュートリアルのポーズシステム
/// </summary>
class Tuto_PauseSystem {
private:
	/// ============================== ///
	///		列挙体
	/// ============================== ///

	/// <summary>
	/// メニュー
	/// </summary>
	enum class Menu {
		kContinue,			//続行
		kOperationGuide,	//操作ガイド
		kMoveTuto,			//移動チュートリアルへ
		kAttackTuto,		//攻撃チュートリアルへ
		kSkip,				//スキップ
		kTItle,				//タイトルへ

		kMaxMenuNum			//メニューの数
	};


public:
	/// ============================== ///
	///		メンバ関数（public）
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Tuto_PauseSystem();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Tuto_PauseSystem();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// デバッグ
	/// </summary>
	void Debug();

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// ポーズ中かを取得
	/// </summary>
	/// <returns>ポーズ中か</returns>
	bool GetIsPause() { return isPause_; }

private:
	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//SE
	std::unique_ptr<Norm::Audio> decideSE_ = nullptr;		//決定
	std::unique_ptr<Norm::Audio> cursolMoveSE_ = nullptr;	//カーソル移動

	//背景マスク
	uint32_t maskTextureHandle_ = 0u;
	std::unique_ptr<Norm::Sprite> maskSprite_ = nullptr;
	//操作ガイド
	uint32_t guideTextureHandle_ = 0u;
	std::unique_ptr<Norm::Sprite> guideSprite_ = nullptr;
	//メニューの文字
	static const uint32_t kMaxStringNum_ = int(Menu::kMaxMenuNum) + 1;
	std::array<Norm::Handle, kMaxStringNum_> stringMenuTextureHandle_;
	std::array<std::unique_ptr<Norm::Sprite>, kMaxStringNum_> stringMenuSprite_;

	//選択中のメニュー
	Menu selectMenu_;
	//ポーズ中か
	bool isPause_;
	//操作ガイド表示中か
	bool isOperationGuideDisplay_;

};

