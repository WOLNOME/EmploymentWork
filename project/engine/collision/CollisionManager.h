#pragma once
#include <list>
#include "Collider.h"

namespace Norm {

	/// <summary>
	/// 全ての当たり判定処理を行うクラス
	/// シングルトンパターンで実装
	/// </summary>
	class CollisionManager {
	private://コンストラクタ等の隠蔽
		static CollisionManager* instance;
		CollisionManager() = default;//コンストラクタ隠蔽
		~CollisionManager() = default;//デストラクタ隠蔽
		CollisionManager(CollisionManager&) = delete;//コピーコンストラクタ封印
		CollisionManager& operator=(CollisionManager&) = delete;//コピー代入演算子封印
	public:
		/// ============================== ///
		///		メンバ関数
		/// ============================== ///

		/// <summary>
		/// インスタンス取得関数
		/// </summary>
		/// <returns></returns>
		static CollisionManager* GetInstance();

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();
		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

		/// <summary>
		/// 当たり判定チェック
		/// </summary>
		void CheckCollision();
		/// <summary>
		/// コライダーのセット
		/// </summary>
		/// <param name="colliders">コライダーのポインタ</param>
		void SetColliders(Collider* colliders);
		/// <summary>
		/// コライダーリストクリア関数
		/// </summary>
		void ClearColliders() { colliders_.clear(); }

	private:
		/// ============================== ///
		///		非公開メンバ関数
		/// ============================== ///

		/// <summary>
		/// 当たり判定演算
		/// </summary>
		/// <param name="colliderA">コライダー1のポインタ</param>
		/// <param name="colliderB">コライダー2のポインタ</param>
		void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

		/// ============================== ///
		///		メンバ変数
		/// ============================== ///

		// コライダーリスト
		std::list<Collider*> colliders_;
	};

}