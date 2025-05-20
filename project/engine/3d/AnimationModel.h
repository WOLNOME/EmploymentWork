#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include <optional>
#include <map>
#include <span>
#include <array>
#include <string>
#include "WorldTransform.h"
#include "BaseCamera.h"
#include "MyMath.h"
#include "ModelFormat.h"
#include "LineDrawer.h"
//assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class AnimationModel {
private://アニメーション関連構造体
	//頂点データ
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	//キーフレーム
	template <typename tValue>
	struct Keyframe {
		float time;
		tValue value;
	};
	using keyframeVector3 = Keyframe<Vector3>;
	using keyframeQuaternion = Keyframe<Quaternion>;
	//ノードアニメーション
	template <typename tValue>
	struct AnimationCurve {
		std::vector<Keyframe<tValue>> keyframes;
	};

	struct NodeAnimation {
		AnimationCurve<Vector3> translate;
		AnimationCurve<Quaternion> rotate;
		AnimationCurve<Vector3> scale;
	};
	//アニメーション
	struct Animation {
		float duration;//アニメーション全体の尺(秒)
		//NodeAnimationの集合、Node名で開けるようにしておく
		std::map<std::string, NodeAnimation> nodeAnimations;
	};
	//joint
	struct Joint {
		TransformQuaternion transform;
		Matrix4x4 localMatrix;
		Matrix4x4 skeletonSpaceMatrix;
		std::string name;
		std::vector<int32_t> children;
		int32_t index;
		std::optional<int32_t> parent;
	};
	//skeleton
	struct Skeleton {
		int32_t root;
		std::map<std::string, int32_t> jointMap;
		std::vector<Joint> joints;
	};
	//頂点ウェイト
	struct VertexWeightData {
		float weight;
		uint32_t vertexIndex;
	};
	//ジョイントウェイト
	struct JointWeightData {
		Matrix4x4 inverseBindPoseMatrix;
		std::vector<VertexWeightData> vertexWeights;
	};
	//インフルエンス
	static const uint32_t kNumMaxInfluence = 4;
	struct VertexInfluence {
		std::array<float, kNumMaxInfluence> weights;
		std::array<int32_t, kNumMaxInfluence> jointIndices;
	};
	//CSに送る用MatrixPalette
	struct WellForGPU {
		Matrix4x4 skeletonSpaceMatrix;			//位置用
		Matrix4x4 skeletonSpaceInverseMatrix;	//法線用
	};
	//CSに送る用SkinningInformation
	struct SkinningInformationForGPU {
		uint32_t numVertices;
	};

	//スキンクラスター
	struct SkinCluster {
		std::vector<Matrix4x4> inverseBindPoseMatrices;
		//MatrixPalette
		Microsoft::WRL::ComPtr<ID3D12Resource> paletteResource;
		std::span<WellForGPU> mappedPalette;
		std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> paletteSrvHandle;
		//入力頂点
		Microsoft::WRL::ComPtr<ID3D12Resource> inputVertexResource;
		std::span<VertexData> mappedInputVertex;
		std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> inputVertexSrvHandle;
		//インフルエンス
		Microsoft::WRL::ComPtr<ID3D12Resource> influenceResource;
		D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
		std::span<VertexInfluence> mappedInfluence;
		std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> influenceSrvHandle;
		//出力頂点
		Microsoft::WRL::ComPtr<ID3D12Resource> outputVertexResource;
		std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> outputVertexSrvHandle;
		//スキニング情報
		Microsoft::WRL::ComPtr<ID3D12Resource> skinningInfoResource;
		std::span<SkinningInformationForGPU> mappedSkinningInfo;
	};

private://メッシュ関連構造体
	//マテリアル
	struct Material {
		Vector4 color;
		Matrix4x4 uvTransform;
		float isTexture;
		float shininess;
	};
	//ノード
	struct Node {
		TransformQuaternion transform;
		Matrix4x4 localMatrix;
		std::string name;
		std::vector<Node> children;
	};
	//マテリアルデータ
	struct MaterialData {
		std::string materialName;
		std::string textureFilePath;
		Vector4 colorData;
		uint32_t textureHandle;
	};
	//モデルデータ
	struct ModelData {
		std::map<std::string, JointWeightData> skinClusterData;
		std::vector<VertexData> vertices;
		std::vector<uint32_t> indices;
		MaterialData material;
		Node rootNode;
	};
	//モデルリソース作成用データ型
	struct ModelResource {
		std::vector<ModelData> modelData;
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> indexResource;
		std::vector<D3D12_INDEX_BUFFER_VIEW> indexBufferView;
		std::vector<uint32_t*> indexData;
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> materialResource;
		std::vector<Material*> materialData;
		std::vector<TransformEuler> uvTransform;
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> textureResorce;
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> textureSrvHandleCPU;
		std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> textureSrvHandleGPU;
	};

public:
	void Initialize(const std::string& filename, ModelFormat format = OBJ, std::string directorypath = "Resources/models/");
	void Update();
	/// <summary>
	/// モデル描画
	/// </summary>
	/// <param name="materialRootParameterIndex">マテリアル設定用ルートパラメータの番号</param>
	/// <param name="textureRootParameterIndex">テクスチャ設定用ルートパラメータの番号</param>
	/// <param name="instancingNum">インスタンス数</param>
	void Draw(uint32_t materialRootParameterIndex, uint32_t textureRootParameterIndex, uint32_t instancingNum = 1, int32_t textureHandle = EOF);
	/// <summary>
	/// 描画前処理
	/// </summary>
	void SettingCSPreDraw();
	/// <summary>
	/// 描画後処理
	/// </summary>
	void SettingCSPostDraw();

public://ゲッター
	const ModelResource& GetModelResource() { return modelResource_; }
public://セッター
	//色のセット
	void SetColor(Vector4& color) { color_ = &color; }
public://アニメーション関連セッター
	//新しいアニメーションのセット
	void SetNewAnimation(const std::string& _name, const std::string& _fileName);
	//現在のアニメーションのセット
	void SetCurrentAnimation(const std::string& _name);

private:
	//モデルファイルの読み取り
	std::vector<ModelData> LoadModelFile();
	//アニメーションの読み取り
	Animation LoadAnimationFile(const std::string& fileName);
	//assimpのノード→構造体ノード変換関数
	Node ReadNode(aiNode* node);
	//モデルリソース作成関数
	ModelResource MakeModelResource();
	//スキンクラスター生成関数
	SkinCluster CreateSkinCluster();
	//テクスチャ読み込み
	void SettingTexture();

	//任意の時刻に対する値を取得する関数
	Vector3 CalculateValue(const std::vector<Keyframe<Vector3>>& keyframes, float time);
	Quaternion CalculateValue(const std::vector<Keyframe<Quaternion>>& keyframes, float time);
	//NodeからJointを作り出す
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);
	//NodeからSkeletonを作り出す関数
	Skeleton CreateSkeleton(const Node& rootNode);
	//joint(骨)の更新
	void UpdateJoints(Skeleton& skeleton);
	//アニメーションを適用する関数
	void ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime);
	//SkinClusterの更新
	void UpdateSkinCluster(SkinCluster& skinCluster, const Skeleton& skeleton);

private:
	//モデル用リソース
	ModelResource modelResource_;
	//モデル数
	size_t modelNum_;
	//色
	Vector4* color_ = nullptr;

	//ディレクトリパス
	std::string directoryPath_;
	//モデルデータファイル名
	std::string fileName_;
	//形式名
	ModelFormat mf_;
	std::string format_;

	//アニメーション用変数
	std::map<std::string, Animation> animations_;
	std::string currentAnimation_;
	float animationTime_ = 0.0f;

	//スケルトン
	Skeleton skeleton_;
	bool isSkeleton_ = false;

	//スキンクラスター
	SkinCluster skinCluster_;

};

