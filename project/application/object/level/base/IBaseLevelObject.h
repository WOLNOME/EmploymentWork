#pragma once
#include <Object3d.h>
#include <MyMath.h>
#include <string>
#include <memory>

//アプリケーション
#include <application/object/level/collision/LevelObjectCollider.h>

/// <summary>
/// レベルオブジェクト単体基底クラス
/// </summary>
class IBaseLevelObject {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IBaseLevelObject() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IBaseLevelObject() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_name">名前</param>
	/// <param name="_filePath">ファイルパス</param>
	/// <param name="_transform">トランスフォーム(オイラー角)</param>
	virtual void Initialize(const std::string& _name, const std::string& _filePath, const Norm::TransformEuler& _transform);
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();
	/// <summary>
	/// ImGui操作
	/// </summary>
	virtual void DebugWithImGui();

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
	const Norm::WorldTransform& GetWorldTransform() const { return object3d_->worldTransform; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// コリジョン情報の設定
	/// </summary>
	/// <param name="_center">中心点</param>
	/// <param name="_size">サイズ</param>
	void SetCollisionInfo(const Norm::Vector3& _center, const Norm::Vector3& _size);

protected:
	/// ============================== ///
	///		メンバ変数(protected)
	/// ============================== ///

	//テクスチャハンドル
	int32_t textureHandle_ = EOF;
	//オブジェクト3D
	std::unique_ptr<Norm::Object3d> object3d_ = nullptr;
	
private:
	/// ============================== ///
	///		メンバ変数(private)
	/// ============================== ///

	//名前
	std::string name_;

	//コライダー
	std::unique_ptr<LevelObjectCollider> collider_ = nullptr;

};