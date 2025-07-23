#pragma once
#include <Sprite.h>
#include <array>
#include <memory>
#include <JsonUtil.h>

class Player;
class PlayerHPUI {
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

private://借用インスタンス
	Player* player_ = nullptr;

private:
	//プレイヤーのパラメーター
	json param_;

	std::array<uint32_t, 2> textureHandles_;
	std::array<std::unique_ptr<Sprite>, 2> sprites_;
};

