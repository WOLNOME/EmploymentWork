#pragma once
#include "BaseCamera.h"
#include "MyMath.h"
#include "Vector3.h"
#include "Vector4.h"
#include "CollisionConfig.h"
#include "WorldTransform.h"

namespace Norm {

	/// <summary>
	/// 当たり判定に必要な情報を持つクラス
	/// </summary>
	class Collider {
	public:
		/// ============================== ///
		///		列挙体
		/// ============================== ///

		/// <summary>
		/// コリジョン形状の種類
		/// </summary>
		enum class CollisionShapeKind {
			Sphere,		//球体
			AABB,		//AABB
			OBB,		//OBB
		};

		/// ============================== ///
		///		メンバ関数
		/// ============================== ///

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~Collider() = default;

		/// <summary>
		/// 当たり判定時の処理
		/// </summary>
		/// <param name="attribute">属性</param>
		/// <param name="subjectPos">相手の座標</param>
		virtual void OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) = 0;

		/// <summary>
		/// デバッグ用パラメーター調整
		/// </summary>
		virtual void DebugWithImGui();

		/// ============================== ///
		///		getter
		/// ============================== ///

		/// <summary>
		/// 形状の種類を取得する
		/// </summary>
		/// <returns>コリジョン形状の種類（Sphere, AABB, OBB など）</returns>
		CollisionShapeKind GetCollisionShapeKind() const { return collisionShapeKind_; }
		/// <summary>
		/// 球体の半径を取得する
		/// </summary>
		/// <returns>コリジョン球の半径</returns>
		float GetRadius() { return collisionRadius_; }
		/// <summary>
		/// AABB（軸平行境界ボックス）の最小座標を取得する
		/// </summary>
		/// <returns>AABB の最小ローカル座標（min）</returns>
		const Vector3& GetMinAABB() { return collisionMinAABB_; }
		/// <summary>
		/// AABB（軸平行境界ボックス）の最大座標を取得する
		/// </summary>
		/// <returns>AABB の最大ローカル座標（max）</returns>
		const Vector3& GetMaxAABB() { return collisionMaxAABB_; }
		/// <summary>
		/// OBB（有向境界ボックス）の中心オフセットを取得する
		/// </summary>
		/// <returns>OBB のローカル中心オフセット</returns>
		const Vector3& GetCenterOffsetOBB() { return collisionCenterOffsetOBB_; }
		/// <summary>
		/// OBB（有向境界ボックス）のサイズを取得する
		/// </summary>
		/// <returns>OBB の幅・高さ・奥行きを表すローカルサイズ</returns>
		const Vector3& GetSizeOBB() { return collisionSizeOBB_; }
		/// <summary>
		/// オブジェクトの現在のワールド座標を取得する
		/// </summary>
		/// <returns>ワールド座標系での位置ベクトル</returns>
		virtual Vector3 GetWorldPosition() = 0;
		/// <summary>
		/// オブジェクトの回転量を取得する
		/// </summary>
		/// <returns>各軸回転を表すベクトル（ラジアン）</returns>
		virtual Vector3 GetRotate() = 0;
		/// <summary>
		/// オブジェクトのスケールを取得する
		/// </summary>
		/// <returns>各軸方向の拡縮率を表すベクトル</returns>
		virtual Vector3 GetScale() = 0;
		/// <summary>
		/// 前フレーム時点のワールド座標を取得する
		/// </summary>
		/// <returns>1フレーム前のワールド座標</returns>
		virtual Vector3 GetPreWorldPosition() = 0;
		/// <summary>
		/// コリジョン属性を取得する
		/// </summary>
		/// <returns>コリジョン判定に使用される属性値</returns>
		CollisionAttribute GetCollisionAttribute() { return collisionAttribute_; }

		/// ============================== ///
		///		setter
		/// ============================== ///

		/// <summary>
		/// コリジョン属性を設定する
		/// </summary>
		/// <param name="collisionAttribute">設定するコリジョン属性</param>
		void SetCollisionAttribute(const CollisionAttribute collisionAttribute) { collisionAttribute_ = collisionAttribute; }

	private:
		/// ============================== ///
		///		メンバ変数(private)
		/// ============================== ///

		CollisionAttribute collisionAttribute_ = CollisionAttribute::Nothingness;
	protected:
		/// ============================== ///
		///		メンバ変数(protected)
		/// ============================== ///

		//形状の種類
		CollisionShapeKind collisionShapeKind_ = CollisionShapeKind::Sphere;

		//球体用変数
		float collisionRadius_ = 1.0f;	//半径

		//AABB用変数
		Vector3 collisionMinAABB_ = { -1,-1,-1 };
		Vector3 collisionMaxAABB_ = { 1,1,1 };

		//OBB用変数
		Vector3 collisionCenterOffsetOBB_ = { 0,0,0 };
		Vector3 collisionSizeOBB_ = { 1,1,1 };

		//デバッグ用変数
		Vector4 debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };
	};

}