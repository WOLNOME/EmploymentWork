#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Matrix4x4.h"
#include "Vector3.h"
#include "MyMath.h"

/// <summary>
/// 頂点シェーダー用ワールド変換行列データ
/// </summary>
struct WorldTransformationMatrixForVS {
	Matrix4x4 matWorld; // ローカル → ワールド変換行列
	Matrix4x4 matWorldInverseTranspose; //ワールド逆転置行列
};

/// <summary>
/// ワールド変換データ
/// </summary>
class WorldTransform {
public:
	/// ============================== ///
	///		メンバ変数(public)
	/// ============================== ///

	// ローカルのスケール
	Vector3 scale = { 1, 1, 1 };
	// ローカルの回転角
	Vector3 rotate = { 0, 0, 0 };
	// ローカルの座標
	Vector3 translate = { 0, 0, 0 };
	//ワールド座標
	Vector3 worldTranslate = { 0, 0, 0 };
	// ローカル → ワールド変換行列
	Matrix4x4 matWorld;
	// 親となるワールド変換へのポインタ
	const WorldTransform* parent = nullptr;

	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	WorldTransform() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~WorldTransform() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// データの更新
	/// </summary>
	void UpdateMatrix();

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// 定数バッファの取得
	/// </summary>
	/// <returns>定数バッファ</returns>
	const Microsoft::WRL::ComPtr<ID3D12Resource>& GetConstBuffer() const { return resource_; }

private:
	/// ============================== ///
	///		メンバ変数(private)
	/// ============================== ///

	// 定数バッファ(座標変換リソース)
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
	// マッピング済みアドレス
	WorldTransformationMatrixForVS* data_ = nullptr;

	// コピー禁止
	WorldTransform(const WorldTransform&) = delete;
	WorldTransform& operator=(const WorldTransform&) = delete;

};

static_assert(!std::is_copy_assignable_v<WorldTransform>);