#pragma once
#include <application/object/character/base/BaseCharacter.h>
#include <string>
#include <Vector3.h>
#include <JsonUtil.h>

class Item : public BaseCharacter {
public:
	// デストラクタ
	~Item() override = default;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& _initPos);
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui() override;

private:
	//当たり判定処理
	void OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) override;

private:
	//死ぬまでの処理
	void UntilDeathProcess();

private:
	//パラメーター
	json param_;

	float swingTimer_ = 0.0f; // アイテムの振り子のタイマー
	bool isUp_ = true; // アイテムの上下移動フラグ
};

