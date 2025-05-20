#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "MyMath.h"

class Shape {
public://列挙型
	enum ShapeKind {
		kSphere,		//球体
		kCube,			//立方体
		kSkyBox,		//スカイボックス
		kPlane,			//平面
		kRing,			//リング
		kTube,			//筒

		kMaxShapeKindNum//最大形状数
	};//※ここに新しい形状を追加する場合、パーティクルクリエイターシーンの形状選択部分も変更すること
private:
	//頂点データ
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	//マテリアル
	struct Material {
		Vector4 color;
		Matrix4x4 uvTransform;
		float isTexture;
		float shininess;
	};
	//形状リソース作成用データ型
	struct ShapeResource {
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		VertexData* vertexData;
		uint32_t vertexNum;
		Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
		Material* materialData;
	};

public:
	void Initialize(ShapeKind shapeKind);
	void Update();
	void Draw(uint32_t materialRootParameterIndex, uint32_t textureRootParameterIndex, uint32_t instancingNum = 1, int32_t textureHandle = EOF);

public://getter
	//形状の種類を取得
	ShapeKind GetShapeKind() const { return shapeKind_; }

private://非公開メンバ関数
	//形状リソース作成関数
	ShapeResource MakeShapeResource();

	//球体リソースの作成関数
	ShapeResource MakeSphereResource();
	//立方体リソースの作成関数
	ShapeResource MakeCubeResource();
	//スカイボックスリソースの作成関数
	ShapeResource MakeSkyBoxResource();
	//平面リソースの作成関数
	ShapeResource MakePlaneResource();
	//リングリソースの作成関数
	ShapeResource MakeRingResource();
	//筒リソースの作成関数
	ShapeResource MakeTubeResource();

private:
	//形状の種類
	ShapeKind shapeKind_;
	//形状用リソース
	ShapeResource shapeResource_;
	//球体の分割数
	const uint32_t kSubdivision = 15;

};

