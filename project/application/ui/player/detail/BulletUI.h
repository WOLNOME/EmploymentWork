#pragma once
#include <Sprite.h>
#include <array>
#include <memory>
#include <JsonUtil.h>

class Player;

/// <summary>
/// 機関銃弾の装填状況UIを管理するクラス
/// </summary>
class BulletUI {
public:
	//初期化
	void Initialize();
	//更新
	void Update();

	//シェイクの適用
	void AttachShake(const Vector2& _shakeOffset);
	//点滅の適用
	void AttachBlinking(const Vector4& _color);

public://setter
	void SetPlayer(Player* _player) { player_ = _player; }

public://借用インスタンス
	Player* player_ = nullptr;

private:
	//プレイヤーのパラメーター
	json param_;

	std::array<uint32_t, 2> textureHandles_;
	std::array<std::unique_ptr<Sprite>, 2> sprites_;
};

