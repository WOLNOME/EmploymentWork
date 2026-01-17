#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include "Vector4.h"
#include "Vector3.h"


/// <summary>
/// 点光源データ構造体
/// </summary>
struct PointLightData {
	Vector4 color;		//ライトの色
	Vector3 position;	//ライトの位置
	float intensity = 0.0f;	//輝度
	float radius = 0.0f;		//ライトの届く最大距離
	float decay = 0.0f;		//減衰率
	uint32_t isActive = 0u;
	float padding[1] = {};
};

/// <summary>
/// 点光源
/// </summary>
class PointLight {
public:
	/// ============================== ///
	///		メンバ変数(public)
	/// ============================== ///

	//色
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	//向き
	Vector3 position_ = { 0.0f, 0.0f, 0.0f };
	//輝度
	float intencity_ = 1.0f;
	//光の有効距離
	float radius_ = 5.0f;
	//減衰率
	float decay_ = 0.5f;
	//オンオフ
	bool isActive_ = true;

	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PointLight() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~PointLight() = default;
	/// <summary>
	/// 行列の更新
	/// </summary>
	void Update();
	/// <summary>
	/// デバッグ用ImGui
	/// </summary>
	/// <param name="_name">名前</param>
	void DebugWithImGui(const std::wstring& _name);

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// 平行光源のデータを取得
	/// </summary>
	/// <returns>スポットライトのデータ</returns>
	const PointLightData& GetData() { return data_; }

private:
	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	// データ
	PointLightData data_;

	// コピー禁止
	PointLight(const PointLight&) = delete;
	PointLight& operator=(const PointLight&) = delete;
};

static_assert(!std::is_copy_assignable_v<PointLight>);
