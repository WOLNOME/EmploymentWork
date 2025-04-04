#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include "MyMath.h"
#include "Model.h"
#include "Shape.h"
#include "ModelFormat.h"
#include "WorldTransform.h"
#include "SceneLight.h"

class BaseCamera;
//モデル
class Object3d {
private://非公開列挙型
	enum ObjectKind {
		kModel,
		kShape,
	};

public://構造体
	struct LightFlagForPS {
		uint32_t isActiveLights;
	};

public://メンバ関数
	Object3d();

	//初期化
	void InitializeModel(const std::string& filePath, ModelFormat format = OBJ);
	//形状初期化
	void InitializeShape(Shape::ShapeKind kind);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform">ワールドトランスフォーム</param>
	/// <param name="camera">カメラ</param>
	/// <param name="dirLight">シーン内光源</param>
	void Draw(
		WorldTransform& worldTransform,
		const BaseCamera& camera,
		const SceneLight* sceneLight = nullptr,
		int32_t textureHandle = EOF
	);
private://非公開メンバ関数

private://メンバ変数
	//モデル
	Model* model_ = nullptr;

	//形状
	std::unique_ptr<Shape> shape_ = nullptr;
	//オブジェクトの種類
	ObjectKind objKind_;

	//ライト有無用定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> lightFlagResource_;
	//ライト有無用データ
	LightFlagForPS* lightFlagData_ = nullptr;

};

