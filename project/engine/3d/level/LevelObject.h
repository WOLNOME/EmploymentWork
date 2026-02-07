#pragma once
#include <Object3d.h>
#include <MyMath.h>
#include <string>
#include <memory>

//アプリケーション
#include <application/object/level/collision/LevelObjectCollider.h>

namespace Norm {

	/// <summary>
	/// レベルオブジェクト単体の処理全般を管理するクラス
	/// </summary>
	class LevelObject {
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
		~LevelObject() = default;
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
		/// アクションフラグの取得
		/// </summary>
		/// <returns>アクションフラグ</returns>
		bool GetIsAction() { return isAction_; }
		/// <summary>
		/// 倒れる方向の取得
		/// </summary>
		/// <returns>倒れる方向</returns>
		const Vector3& GetInvertDirection() { return invertDirection_; }

		/// ============================== ///
		///		setter
		/// ============================== ///

		/// <summary>
		/// アクションフラグの設定
		/// </summary>
		/// <param name="_isAction">アクション中かどうか</param>
		void SetIsAction(bool _isAction) { isAction_ = _isAction; }
		/// <summary>
		/// 倒れる方向の設定
		/// </summary>
		/// <param name="_direction">倒れる方向</param>
		void SetInvertDirection(const Vector3& _direction) {invertDirection_ = _direction;}

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

		//コライダー
		std::unique_ptr<LevelObjectCollider> collider_ = nullptr;

		//ツリーオブジェクト限定変数
		bool isAction_ = false;		// 倒れるアクションをするかどうか
		Vector3 invertDirection_ = { 0.0f,0.0f,0.0f };	// 倒れる方向
		const float time_ = 1.0f;
		float timer_ = 0.0f;	// タイマー

	};

}