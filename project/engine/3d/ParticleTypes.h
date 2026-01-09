#pragma once
#include <cstdint>
#include "Vector4.h"

/// <summary>
/// GPU用トランスフォーム情報
/// </summary>
struct TransformForGPU {
	Vector4 scale;
	Vector4 rotate;
	Vector4 translate;
};
/// <summary>
/// CS用粒情報
/// </summary>
struct GrainForCS {
	TransformForGPU initialTransform;
	TransformForGPU transform;
	Vector4 velocity;
	Vector4 angularVelocity;
	float sizeVelocity;
	float lifeTime = 0.0f;
	float currentTime = 0.0f;
	uint32_t startColor;
	uint32_t endColor;
	uint32_t emitterID;
};
/// <summary>
/// CS用エミッター情報
/// </summary>
struct EmitterForCS {
	TransformForGPU worldTransform;
	uint32_t isAffectedField = 0u;
	uint32_t isPlay = 0u;
	uint32_t isAlive = 0u;
};
/// <summary>
/// CS用JSON情報
/// </summary>
struct JsonInfoForCS {
	TransformForGPU localTransform;
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
/// CS用総合情報
/// </summary>
struct GeneralInfoForCS {
	float time = 0.0f;
	float deltaTime = 0.0f;
	uint32_t maxGrains = 0u;
	uint32_t maxEmitters = 0u;
};