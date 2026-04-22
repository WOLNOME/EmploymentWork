#pragma once
#include <Sprite.h>
#include <Vector3.h>
#include <memory>
#include <array>

//前方宣言（アプリケーション）
class CameraManager;
class Player;

/// <summary>
/// ヒットインジケーターを管理するクラス
/// </summary>
class HitIndicator {
private:
	/// ============================== ///
	///		構造体
	/// ============================== ///

	/// <summary>
	/// インジケーター情報
	/// </summary>
	struct IndicatorInfo {
		uint32_t textureHandle = 0u;
		std::unique_ptr<Norm::Sprite> sprite = nullptr;
		float currentTime;
		Norm::Vector3 hitPosition;
		bool isActive;
	};

public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ヒットインジケーター登録
	/// </summary>
	/// <param name="_hitPosition">衝突点</param>
	void RegistIndicator(const Norm::Vector3& _hitPosition);

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// プレイヤーの設定
	/// </summary>
	/// <param name="_player">プレイヤー</param>
	void SetPlayer(Player* _player) { player_ = _player; }
	/// <summary>
	/// カメラマネージャーの設定
	/// </summary>
	/// <param name="_cameraManager">カメラマネージャー</param>
	void SetCameraManager(CameraManager* _cameraManager) { cameraManager_ = _cameraManager; }

	/// <summary>
	/// 表示するかを設定
	/// </summary>
	/// <param name="_isDisplay">表示するか</param>
	void SetIsDisplay(bool _isDisplay);

private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	Player* player_ = nullptr;
	CameraManager* cameraManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	static const int kNumIndicators_ = 10;
	std::array<IndicatorInfo, kNumIndicators_> indicators_;

	const float kMaxActiveTime_ = 1.0f;
};

