#pragma once
#include "Collider.h"
#include <Object3d.h>
#include <MyMath.h>
#include <string>
#include <memory>

namespace Norm {

	/// <summary>
	/// レベルオブジェクト単体の処理全般を管理するクラス
	/// </summary>
	class LevelObject : public ColliderBase {
	public:
		/// ============================== ///
		///		メンバ関数
		/// ============================== ///

		/// <summary>
		/// コンストラクタ
		/// </summary>
		LevelObject() = default;
		/// <summary>
		/// デストラクタ
		/// </summary>
		~LevelObject() override = default;
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="_name">名前</param>
		/// <param name="_filePath">ファイルパス</param>
		/// <param name="_transform">トランスフォーム(オイラー角)</param>
		void Initialize(const std::string& _name, const std::string& _filePath, const TransformEuler& _transform);
		/// <summary>
		/// 更新
		/// </summary>
		void Update();
		/// <summary>
		/// ImGui操作
		/// </summary>
		void DebugWithImGui();

		/// <summary>
		/// 当たり判定時の処理
		/// </summary>
		/// <param name="attribute">相手の属性</param>
		/// <param name="subjectPos">相手の座標</param>
		void OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) override;

		/// ============================== ///
		///		getter
		/// ============================== ///

		/// <summary>
		/// 名前の取得
		/// </summary>
		/// <returns>名前</returns>
		const std::string& GetName() const { return name_; }
		/// <summary>
		/// ワールド変換情報の取得
		/// </summary>
		/// <returns>ワールド変換情報</returns>
		const WorldTransform& GetWorldTransform() const { return object3d_->worldTransform; }
		/// <summary>
		///	オブジェクトの現在のワールド座標を取得する
		/// </summary>
		/// <returns>オブジェクトの現在のワールド座標</returns>
		Vector3 GetWorldPosition() override { return object3d_->worldTransform.GetWorldTranslate(); }
		/// <summary>
		/// オブジェクトの回転量を取得する
		/// </summary>
		/// <returns>オブジェクトの回転量</returns>
		Vector3 GetRotate() override { return object3d_->worldTransform.GetRotate(); }
		/// <summary>
		/// オブジェクトのスケールを取得する
		/// </summary>
		/// <returns>オブジェクトのスケール</returns>
		Vector3 GetScale() override { return object3d_->worldTransform.GetScale(); }
		/// <summary>
		/// 前フレーム時点のワールド座標を取得する
		/// </summary>
		/// <returns>前フレーム時点のワールド座標</returns>
		Vector3 GetPreWorldPosition() override { return prePosition_; }

		/// ============================== ///
		///		setter
		/// ============================== ///

		/// <summary>
		/// コリジョン情報の設定
		/// </summary>
		/// <param name="_center">中心点</param>
		/// <param name="_size">サイズ</param>
		void SetCollisionInfo(const Vector3& _center, const Vector3& _size);

	protected:
		/// ============================== ///
		///		メンバ変数(protected)
		/// ============================== ///

		//テクスチャハンドル
		int32_t textureHandle_ = EOF;
		//オブジェクト3D
		std::unique_ptr<Object3d> object3d_ = nullptr;
		//前フレームの座標
		Vector3 prePosition_ = {};

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

		//名前
		std::string name_;

		//コリジョンが有効か
		bool isCollisionEnabled_ = false;


		//ツリーオブジェクト限定変数
		bool isAction_ = false;		// 倒れるアクションをするかどうか
		Vector3 invertDirection_ = { 0.0f,0.0f,0.0f };	// 倒れる方向
		const float time_ = 1.0f;
		float timer_ = 0.0f;	// タイマー

	};

}