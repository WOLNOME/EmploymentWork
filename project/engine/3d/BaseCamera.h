#pragma once
#include <d3d12.h>
#include <type_traits>
#include <wrl.h>
#include <cstdint>
#include <list>
#include "MyMath.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "WorldTransform.h"

// 定数バッファ用データ構造体
struct ViewProjectionTransformationMatrixForVS {
	Matrix4x4 matView;         // ワールド → ビュー変換行列
	Matrix4x4 matProjection;   // ビュー → プロジェクション変換行列
};
struct WorldPositionForPS {
	Vector3 worldPosition;    // カメラ座標（ワールド座標）
};

// 基本的なカメラ機能を提供する基底クラス
class BaseCamera {
public:
	BaseCamera();
	virtual ~BaseCamera() = default;

	// 初期化
	virtual void Initialize();
	// 更新
	virtual void Update();

	// 更新（行列の計算）
	void UpdateMatrix();

	//ImGui操作
	void DebugWithImGui();

	// ゲッター
	const Matrix4x4& GetViewMatrix() const { return viewMatrix; }
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix; }
	const Matrix4x4& GetViewProjectionMatrix() const { return viewProjectionMatrix; }
	float GetFovY() { return fovY; }
	float GetAspectRatio() { return aspectRatio; }
	float GetNearClip() { return nearClip; }
	float GetFarClip() { return farClip; }
	const Microsoft::WRL::ComPtr<ID3D12Resource>& GetViewProjectionConstBuffer() const { return viewProjectionResource_; }
	const Microsoft::WRL::ComPtr<ID3D12Resource>& GetCameraPositionConstBuffer() const { return cameraPositionResource_; }
	const Vector3 GetForwardDirection();
	const Vector3 GetRightDirection();
	const Vector3 GetLeftDirection();
	const Vector3 GetBackDirection();

	// セッター
	void SetFovY(float fovY) { this->fovY = fovY; }
	void SetAspectRatio(float aspectRatio) { this->aspectRatio = aspectRatio; }
	void SetNearClip(float nearClip) { this->nearClip = nearClip; }
	void SetFarClip(float farClip) { this->farClip = farClip; }

	//カメラ演出系の関数
	void RegistShake(float time, float power = 0.5f);

private:
	//カメラの揺れの更新
	void UpdateShake();

public:
	//ワールドトランスフォーム
	WorldTransform worldTransform;

protected:
	// 定数バッファ(座標変換リソース)
	Microsoft::WRL::ComPtr<ID3D12Resource> viewProjectionResource_ = nullptr;
	// マッピング済みアドレス
	ViewProjectionTransformationMatrixForVS* viewProjectionData_ = nullptr;

	// 定数バッファ(カメラ座標リソース)
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraPositionResource_ = nullptr;
	// マッピング済みアドレス
	WorldPositionForPS* cameraPositionData_ = nullptr;


	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 viewProjectionMatrix;

	float fovY;               // 水平方向視野角
	float aspectRatio;        // アスペクト比
	float nearClip;           // ニアクリップ距離
	float farClip;            // ファークリップ距離

	//カメラの(オフセットを考慮しない)座標
	Vector3 standardPosition;   // カメラの位置

private:
	// カメラの揺れを管理する構造体
	struct ShakeData {
		float maxTime;		// 揺れの最大時間
		float time;			// 揺れの時間
		float maxPower;		// 揺れの最大強さ
		float power;		// 揺れの強さ
	};
	// カメラの揺れを管理する変数
	std::list<ShakeData> shakeList_;
	Vector3 shakeOffset_;
};
