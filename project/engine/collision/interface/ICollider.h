#pragma once
#include "CollisionConfig.h"
#include "WorldTransform.h"
#include "Vector3.h"

namespace Norm {

	/// ============================== ///
	///		列挙体
	/// ============================== ///

	//コライダーの形状
	enum class ColliderShape {
		Sphere,
		AABB,
		OBB,

		ShapeCount,
	};

	/// <summary>
	/// 衝突判定のインターフェース
	/// </summary>
	class ICollider {
	public:
		/// ============================== ///
		///		メンバ関数
		/// ============================== ///

		/// <summary>
		/// 仮想デストラクタ
		/// </summary>
		virtual ~ICollider() = default;

		/// <summary>
		/// デバッグ
		/// </summary>
		virtual void Debug();

		/// <summary>
		/// 衝突時のコールバック
		/// </summary>
		/// <param name="_other">相手</param>
		virtual void OnCollision([[maybe_unused]] ICollider* _other) = 0;

		/// ============================== ///
		///		getter
		/// ============================== ///

		/// <summary>
		/// コリジョン属性を取得する
		/// </summary>
		/// <returns>コリジョン判定に使用される属性値</returns>
		CollisionAttribute GetCollisionAttribute() { return collisionAttribute_; }

		/// <summary>
		/// コライダーの形状を取得する
		/// </summary>
		/// <returns>コライダーの形状</returns>
		ColliderShape GetCollderShape() { return colliderShape_; }

		/// <summary>
		/// ワールドトランスフォームを取得する
		/// </summary>
		/// <returns>ワールドトランスフォームのポインタ</returns>
		const WorldTransform& GetWorldTransform() { return *worldTransform_; }

		/// <summary>
		/// 中心座標の取得
		/// </summary>
		/// <returns>中心座標</returns>
		const Vector3& GetCenter();

		/// <summary>
		/// オフセットの取得
		/// </summary>
		/// <returns>オフセット</returns>
		const Vector3& GetOffset() { return offset_; }

		/// ============================== ///
		///		setter
		/// ============================== ///

		/// <summary>
		/// コリジョン属性を設定する
		/// </summary>
		/// <param name="collisionAttribute">設定するコリジョン属性</param>
		void SetCollisionAttribute(const CollisionAttribute collisionAttribute) { collisionAttribute_ = collisionAttribute; }

		/// <summary>
		/// ワールドトランスフォームを設定する
		/// </summary>
		/// <param name="_worldTransform">設定するワールドトランスフォームのポインタ</param>
		void SetWorldTransform(WorldTransform* _worldTransform) { worldTransform_ = _worldTransform; }

		/// <summary>
		/// オフセットを設定する
		/// </summary>
		/// <param name="_offset">設定するオフセット</param>
		void SetOffset(const Vector3& _offset) { offset_ = _offset; }

	protected:
		/// ============================== ///
		///		メンバ変数(protected)
		/// ============================== ///

		//コリジョン属性
		CollisionAttribute collisionAttribute_ = CollisionAttribute::Nothingness;

		//コライダーの形状
		ColliderShape colliderShape_;

		//対象のワールドトランスフォーム
		WorldTransform* worldTransform_ = nullptr;

		//中心座標
		Vector3 center_ = { 0,0,0 };
		//オフセット
		Vector3 offset_ = { 0,0,0 };

	};

}