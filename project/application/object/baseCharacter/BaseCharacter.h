#pragma once
#include "Collider.h"
#include "BaseCamera.h"
#include "SceneLight.h"

// キャラクター基底クラス
class BaseCharacter : public Collider {
public:
	// コンストラクタ
	BaseCharacter() = default;
	// デストラクタ
	virtual ~BaseCharacter() = default;
	// 初期化
	virtual void Initialize() = 0;
	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Draw(BaseCamera* _camera, SceneLight* _light) = 0;
	// デバッグ用パラメーター調整
	virtual void DebugWithImGui() = 0;


};

