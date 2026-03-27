#include "InputSystem.h"

//アプリケーション
#include <application/object/character/player/Player.h>

void InputSystem::Initialize() {
	//変数の初期化
	isLocked_ = false;
}

void InputSystem::SetIsLocked(bool _isLocked) {
	isLocked_ = _isLocked;
	player_->SetIsInput(!isLocked_);
}
