#pragma once
#include "BaseScene.h"
#include <string>

namespace Norm {

	/// <summary>
	/// 文字列を入力してシーンを生成する基底クラス
	/// </summary>
	class AbstractSceneFactory {
	public:
		/// ============================== ///
		///		メンバ関数
		///	============================== ///

		/// <summary>
		/// 仮想デストラクタ
		/// </summary>
		virtual ~AbstractSceneFactory() = default;
		/// <summary>
		/// シーン生成
		/// </summary>
		/// <param name="sceneName">シーン名</param>
		/// <returns>シーンクラスのポインタ</returns>
		virtual BaseScene* CreateScene(const std::string& sceneName) = 0;
	};

}