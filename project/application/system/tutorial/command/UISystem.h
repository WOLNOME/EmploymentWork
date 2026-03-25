#pragma once
#include <memory>
#include <Sprite.h>

/// <summary>
/// UIシステム
/// </summary>
class UISystem {
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
	/// 指定したテクスチャでスプライトを表示する
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void ShowSprite(uint32_t textureHandle);

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// 完了状態を取得
	/// </summary>
	/// <returns> 完了状態</returns>
	bool GetIsTutorialSpriteFinished() const { return isFinishedShowSprite; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// 完了状態を設定
	/// </summary>
	/// <param name="isFinished">完了状態</param>
	void SetIsTutorialSpriteFinished(bool isFinished) { isFinishedShowSprite = isFinished; }

private:
	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	std::unique_ptr<Norm::Sprite> sprite_; // 表示用スプライト

	bool isFinishedShowSprite = false;
};
