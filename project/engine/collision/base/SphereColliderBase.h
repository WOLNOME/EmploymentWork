#pragma once
#include "ICollider.h"
#include <MyMath.h>

namespace Norm {

	/// <summary>
	/// 球体衝突判定用の基底クラス
	/// </summary>
	class SphereColliderBase : public ICollider {
	public:
		/// ============================== ///
		///		メンバ関数
		/// ============================== ///

		/// <summary>
		/// コンストラクタ
		/// </summary>
		SphereColliderBase();
		/// <summary>
		/// 仮想デストラクタ
		/// </summary>
		virtual ~SphereColliderBase() = default;

		/// ============================== ///
		///		getter
		/// ============================== ///

		/// <summary>
		/// カプセルの取得
		/// </summary>
		/// <returns>カプセル</returns>
		const Capsule& GetCapsule();

		/// <summary>
		/// 球体の半径を取得する
		/// </summary>
		/// <returns>コリジョン球の半径</returns>
		float GetRadius() { return radius_; }

		/// <summary>
		/// 前フレーム時点のワールド座標を取得する
		/// </summary>
		/// <returns>1フレーム前のワールド座標</returns>
		virtual Vector3 GetPreWorldTranslate() = 0;

	protected:
		/// ============================== ///
		///		メンバ変数(protected)
		/// ============================== ///

		//球体用変数
		float radius_ = 1.0f;	//半径

	};

};

