#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include <string>
#include <list>
#include <memory>
#include "json.hpp"
#include "Shape.h"
#include "MyMath.h"

using json = nlohmann::json;

/// <summary>
/// パーティクル単体の処理全般を管理するクラス
/// </summary>
class Particle {
	//パーティクルマネージャーに公開
	friend class ParticleManager;
	//パーティクルクリエイターシーンに公開
	friend class ParticleEditorScene;
	//複合パーティクルに公開
	friend class CombinedParticle;
public:
	/// ============================== ///
	///		列挙体
	/// ============================== ///

	/// <summary>
	/// 生成方法
	/// </summary>
	enum class GenerateMethod {
		Random,		//ランダム
		Clump,		//塊	

		kMaxGenerateMethodNum,
	};
	/// <summary>
	/// エフェクトスタイル
	/// </summary>
	enum class EffectStyle {
		Loop,		//ループ(永続)
		OneShot,	//一度きり

		kMaxEffectStyleNum,
	};

private:
	/// ============================== ///
	///		構造体(private)
	/// ============================== ///

	/// <summary>
	/// CS用トランスフォーム情報
	/// </summary>
	struct TransformForCS {
		Vector4 scale;
		Vector4 rotate;
		Vector4 translate;
	};
	/// <summary>
	/// CS用粒情報
	/// </summary>
	struct GrainForCS {
		TransformForCS initialTransform;
		TransformForCS transform;
		Vector4 velocity;
		Vector4 angularVelocity;
		float sizeVelocity;
		float lifeTime = 0.0f;
		float currentTime = 0.0f;
		uint32_t startColor;
		uint32_t endColor;
	};
	/// <summary>
	/// CS用エミッター情報
	/// </summary>
	struct EmitterForCS {
		TransformForCS worldTransform;
		uint32_t isAffectedField = 0u;
		uint32_t isPlay = 0u;
	};
	/// <summary>
	/// CS用JSON情報
	/// </summary>
	struct JsonInfoForCS {
		TransformForCS localTransform;
		Vector4 velocityMax;
		Vector4 velocityMin;
		Vector4 initRotateMax;
		Vector4 initRotateMin;
		Vector4 initScaleMax;
		Vector4 initScaleMin;
		Vector4 startColorMax;
		Vector4 startColorMin;
		Vector4 endColorMax;
		Vector4 endColorMin;
		Vector4 angularVelocityMax;
		Vector4 angularVelocityMin;
		float sizeVelocityMax = 0.0f;
		float sizeVelocityMin = 0.0f;
		float lifeTimeMax = 0.0f;
		float lifeTimeMin = 0.0f;
		float gravity = 0.0f;
		float repulsion = 0.0f;
		float floorHeight = 0.0f;
		int emitRate = 0;
		int maxGrains = 0;
		int generateMethod = 0;
		int clumpNum = 0;
		int effectStyle = 0;
		int isGravity = 0;
		int isBound = 0;
		int isBillboard = 0;
	};
	/// <summary>
	/// CS用フレーム情報
	/// </summary>
	struct PerFrameForCS {
		float time = 0.0f;
		float deltaTime = 0.0f;
	};
	/// <summary>
	/// CS用全リソース
	/// </summary>
	struct AllResourceForCS {
		//粒の情報
		Microsoft::WRL::ComPtr<ID3D12Resource> grainsResource;
		uint32_t grainsSrvIndex = 0u;	//VS用
		uint32_t grainsUavIndex = 0u;	//CS用
		//フリーリストのインデックス情報
		Microsoft::WRL::ComPtr<ID3D12Resource> freeListIndexResource;
		uint32_t freeListIndexUavIndex = 0u;		//CS用
		//フリーリストの情報
		Microsoft::WRL::ComPtr<ID3D12Resource> freeListResource;
		uint32_t freeListUavIndex = 0u;		//CS用
		//エミッター情報
		Microsoft::WRL::ComPtr<ID3D12Resource> emitterResource;
		std::span<EmitterForCS> mappedEmitter;
		//JSON情報
		Microsoft::WRL::ComPtr<ID3D12Resource> jsonInfoResource;
		std::span<JsonInfoForCS> mappedJsonInfo;
		//時間情報
		Microsoft::WRL::ComPtr<ID3D12Resource> perFrameResource;
		std::span<PerFrameForCS> mappedPerFrame;
	};

public:
	/// ============================== ///
	///		構造体(public)
	/// ============================== ///

	/// <summary>
	/// エミッター情報
	/// </summary>
	struct Emitter {
		TransformEuler worldTransform;	//エミッターのトランスフォーム
		bool isAffectedField = false;	//フィールドに影響を受けるか
		bool isPlay = false;			//パーティクルを生成するか
	};

public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Particle();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="name">インスタンスの名前</param>
	/// <param name="fileName">使用するパーティクルの名前(.jsonは省略)</param>
	void Initialize(const std::string& name, const std::string& fileName);

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	///　パラメーターの取得
	/// </summary>
	/// <returns>パラメーター</returns>
	const json& GetParam() { return param_; }

	/// ============================== ///
	///		メンバ変数(public)
	/// ============================== ///

	Emitter emitter_;

private:
	/// ============================== ///
	///		メンバ関数(private)
	/// ============================== ///

	/// <summary>
	/// CS用全リソースの作成
	/// </summary>
	/// <returns>CS用全リソース</returns>
	AllResourceForCS CreateAllResourceForCS();

	/// <summary>
	/// 形状の変更
	/// </summary>
	void ShapeChange();
	/// <summary>
	/// テクスチャの変更
	/// </summary>
	void TextureChange();

	/// <summary>
	/// エミッター情報反映(CSに反映)
	/// </summary>
	void TraceEmitterForCS();
	/// <summary>
	/// JSON情報反映(CSに反映)
	/// </summary>
	void TraceJsonDataForCS();

private:
	/// ============================== ///
	///		メンバ変数(private)
	/// ============================== ///

	//形状(見た目)
	std::unique_ptr<Shape> shape_;
	//CS専用のリソース
	AllResourceForCS allResourceForCS_;

	//名前
	std::string name_;
	//各粒のパラメーター
	json param_;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};