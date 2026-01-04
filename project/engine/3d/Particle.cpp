#include "Particle.h"
#include "DirectXCommon.h"
#include "GPUDescriptorManager.h"
#include "ModelManager.h"
#include "ParticleManager.h"
#include "TextureManager.h"
#include "JsonUtil.h"

inline UINT Align256(UINT size) {
	return (size + 255) & ~255;
}

Particle::~Particle() {
	//確保したSRV・UAVデスクリプタヒープの解放
	GPUDescriptorManager::GetInstance()->Free(allResourceForCS_.grainsSrvIndex);
	GPUDescriptorManager::GetInstance()->Free(allResourceForCS_.grainsUavIndex);
	GPUDescriptorManager::GetInstance()->Free(allResourceForCS_.freeListIndexUavIndex);
	GPUDescriptorManager::GetInstance()->Free(allResourceForCS_.freeListUavIndex);

	//マネージャーから削除
	ParticleManager::GetInstance()->Delete(name_);
}

void Particle::Initialize(const std::string& name, const std::string& fileName) {
	//名前を登録
	name_ = name;
	//パラメータをセット
	auto data = JsonUtil::GetJsonData("Resources/particles/" + fileName);
	//JSONファイルの読み込み
	if (data) param_ = data;
	else assert(0 && "JSONファイルが存在しません");
	//エミッターの初期化
	emitter_.worldTransform.translate = { 0,0,0 };
	emitter_.worldTransform.rotate = { 0,0,0 };
	emitter_.worldTransform.scale = { 1,1,1 };
	emitter_.isAffectedField = false;
	emitter_.isPlay = false;
	//テクスチャハンドルの取得
	textureHandle_ = TextureManager::GetInstance()->LoadTexture(param_["Texture"]);

	//形状を生成
	shape_ = std::make_unique<Shape>();
	//形状の初期化
	Shape::ShapeKind shapeKind = (Shape::ShapeKind)param_["Primitive"];
	shape_->Initialize(shapeKind);

	//CS専用リソースの作成
	allResourceForCS_ = CreateAllResourceForCS();
	//エミッターの情報を写す
	TraceEmitterForCS();
	//JSONの情報を写す
	TraceJsonDataForCS();

	//最後にマネージャーに登録
	ParticleManager::GetInstance()->Regist(name_, this);
}

Particle::AllResourceForCS Particle::CreateAllResourceForCS() {
	AllResourceForCS result;
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	int maxNumGrains = param_["MaxGrains"];

	{
		//粒の情報用のResorceを確保
		result.grainsResource = dxCommon->CreateUAVBufferResource(sizeof(GrainForCS) * maxNumGrains);
		//粒情報用のuavを作成。RBStructuredBufferでアクセスできるようにする
		result.grainsUavIndex = GPUDescriptorManager::GetInstance()->Allocate();
		GPUDescriptorManager::GetInstance()->CreateUAVforRWStructuredBuffer(result.grainsUavIndex, result.grainsResource.Get(), UINT(maxNumGrains), sizeof(GrainForCS));
		//粒情報用のsrvを作成。StructuredBufferでアクセスできるようにする
		result.grainsSrvIndex = GPUDescriptorManager::GetInstance()->Allocate();
		GPUDescriptorManager::GetInstance()->CreateSRVforStructuredBuffer(result.grainsSrvIndex, result.grainsResource.Get(), UINT(maxNumGrains), sizeof(GrainForCS));
	}
	{
		//フリーリストインデックス用のResourceを確保
		result.freeListIndexResource = dxCommon->CreateUAVBufferResource(sizeof(int32_t));
		//フリーリストインデックス用のuavを作成。RWStructuredBufferでアクセスできるようにする
		result.freeListIndexUavIndex = GPUDescriptorManager::GetInstance()->Allocate();
		GPUDescriptorManager::GetInstance()->CreateUAVforRWStructuredBuffer(result.freeListIndexUavIndex, result.freeListIndexResource.Get(), 1, sizeof(int32_t));
	}
	{
		//フリーリスト用のResourceを確保
		result.freeListResource = dxCommon->CreateUAVBufferResource(sizeof(uint32_t) * maxNumGrains);
		//フリーリスト用のuavを作成。RWStructuredBufferでアクセスできるようにする
		result.freeListUavIndex = GPUDescriptorManager::GetInstance()->Allocate();
		GPUDescriptorManager::GetInstance()->CreateUAVforRWStructuredBuffer(result.freeListUavIndex, result.freeListResource.Get(),
			UINT(maxNumGrains), sizeof(uint32_t));
	}
	{
		//エミッター情報用のResorceを確保
		result.emitterResource = dxCommon->CreateBufferResource(sizeof(EmitterForCS));
		EmitterForCS* mappedEmitter = nullptr;
		result.emitterResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedEmitter));
		std::memset(mappedEmitter, 0, sizeof(EmitterForCS));
		result.mappedEmitter = { mappedEmitter,1 };
	}
	{
		//JSON情報用のResorceを確保
		result.jsonInfoResource = dxCommon->CreateBufferResource(sizeof(JsonInfoForCS));
		JsonInfoForCS* mappedJsonInfo = nullptr;
		result.jsonInfoResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedJsonInfo));
		std::memset(mappedJsonInfo, 0, sizeof(JsonInfoForCS));
		result.mappedJsonInfo = { mappedJsonInfo,1 };
	}
	{
		//時間の情報用のResorceを確保
		result.perFrameResource = dxCommon->CreateBufferResource(sizeof(PerFrameForCS));
		PerFrameForCS* mappedPerFrame = nullptr;
		result.perFrameResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPerFrame));
		std::memset(mappedPerFrame, 0, sizeof(PerFrameForCS));
		result.mappedPerFrame = { mappedPerFrame,1 };
		//データ入力
		result.mappedPerFrame[0].time = 0.0f;
		result.mappedPerFrame[0].deltaTime = kDeltaTime;
	}

	return result;
}

void Particle::ShapeChange() {
	//JSONをもとにShapeを作り直す
	shape_ = std::make_unique<Shape>();
	//形状の初期化
	Shape::ShapeKind shapeKind = (Shape::ShapeKind)param_["Primitive"];
	shape_->Initialize(shapeKind);
}

void Particle::TextureChange() {
	//テクスチャハンドルの取得
	textureHandle_ = TextureManager::GetInstance()->LoadTexture(param_["Texture"]);
}

void Particle::TraceEmitterForCS() {
	auto Vec3ToVec4 = [](const Vector3& j) -> Vector4 {
		return { j.x,j.y,j.z,0.0f };
		};
	allResourceForCS_.mappedEmitter[0].worldTransform.scale = Vec3ToVec4(emitter_.worldTransform.scale);
	allResourceForCS_.mappedEmitter[0].worldTransform.rotate = Vec3ToVec4(emitter_.worldTransform.rotate);
	allResourceForCS_.mappedEmitter[0].worldTransform.translate = Vec3ToVec4(emitter_.worldTransform.translate);
	allResourceForCS_.mappedEmitter[0].isAffectedField = emitter_.isAffectedField;
	allResourceForCS_.mappedEmitter[0].isPlay = emitter_.isPlay;
}

void Particle::TraceJsonDataForCS() {
	//データ入力
	auto Vec3ToVec4 = [](const auto& j) -> Vector4 {
		return { (float)j["x"], (float)j["y"], (float)j["z"], 0.0f };
		};
	auto Vec4ToVec4 = [](const auto& j) -> Vector4 {
		return { (float)j["x"], (float)j["y"],(float)j["z"],(float)j["w"] };
		};
	allResourceForCS_.mappedJsonInfo[0].localTransform.scale = Vec4ToVec4(param_["LocalTransform"]["Scale"]);
	allResourceForCS_.mappedJsonInfo[0].localTransform.rotate = Vec4ToVec4(param_["LocalTransform"]["Rotate"]);
	allResourceForCS_.mappedJsonInfo[0].localTransform.translate = Vec4ToVec4(param_["LocalTransform"]["Translate"]);
	allResourceForCS_.mappedJsonInfo[0].velocityMax = Vec3ToVec4(param_["Velocity"]["Max"]);
	allResourceForCS_.mappedJsonInfo[0].velocityMin = Vec3ToVec4(param_["Velocity"]["Min"]);
	allResourceForCS_.mappedJsonInfo[0].initRotateMax = Vec3ToVec4(param_["GrainTransform"]["Rotate"]["Max"]);
	allResourceForCS_.mappedJsonInfo[0].initRotateMin = Vec3ToVec4(param_["GrainTransform"]["Rotate"]["Min"]);
	allResourceForCS_.mappedJsonInfo[0].initScaleMax = Vec3ToVec4(param_["GrainTransform"]["Scale"]["Max"]);
	allResourceForCS_.mappedJsonInfo[0].initScaleMin = Vec3ToVec4(param_["GrainTransform"]["Scale"]["Min"]);
	allResourceForCS_.mappedJsonInfo[0].startColorMax = Vec4ToVec4(param_["StartColor"]["Max"]);
	allResourceForCS_.mappedJsonInfo[0].startColorMin = Vec4ToVec4(param_["StartColor"]["Min"]);
	allResourceForCS_.mappedJsonInfo[0].endColorMax = Vec4ToVec4(param_["EndColor"]["Max"]);
	allResourceForCS_.mappedJsonInfo[0].endColorMin = Vec4ToVec4(param_["EndColor"]["Min"]);
	allResourceForCS_.mappedJsonInfo[0].angularVelocityMax = Vec3ToVec4(param_["AngularVelocity"]["Max"]);
	allResourceForCS_.mappedJsonInfo[0].angularVelocityMin = Vec3ToVec4(param_["AngularVelocity"]["Min"]);
	allResourceForCS_.mappedJsonInfo[0].sizeVelocityMax = param_["SizeVelocity"]["Max"];
	allResourceForCS_.mappedJsonInfo[0].sizeVelocityMin = param_["SizeVelocity"]["Min"];
	allResourceForCS_.mappedJsonInfo[0].lifeTimeMax = param_["LifeTime"]["Max"];
	allResourceForCS_.mappedJsonInfo[0].lifeTimeMin = param_["LifeTime"]["Min"];
	allResourceForCS_.mappedJsonInfo[0].gravity = param_["Gravity"];
	allResourceForCS_.mappedJsonInfo[0].repulsion = param_["Repulsion"];
	allResourceForCS_.mappedJsonInfo[0].floorHeight = param_["FloorHeight"];
	allResourceForCS_.mappedJsonInfo[0].emitRate = param_["EmitRate"];
	allResourceForCS_.mappedJsonInfo[0].maxGrains = param_["MaxGrains"];
	allResourceForCS_.mappedJsonInfo[0].generateMethod = param_["GenerateMethod"];
	allResourceForCS_.mappedJsonInfo[0].clumpNum = param_["ClumpNum"];
	allResourceForCS_.mappedJsonInfo[0].effectStyle = param_["EffectStyle"];
	allResourceForCS_.mappedJsonInfo[0].isGravity = param_["IsGravity"];
	allResourceForCS_.mappedJsonInfo[0].isBound = param_["IsBound"];
	allResourceForCS_.mappedJsonInfo[0].isBillboard = param_["IsBillboard"];
}
