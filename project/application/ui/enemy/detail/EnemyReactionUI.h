#pragma once
#include <JsonUtil.h>
#include <Object3d.h>
#include <MyMath.h>
#include <array>
#include <memory>

//前方宣言（エンジン）
namespace Norm {
	class GameCamera;
}

/// <summary>
/// 敵のリアクションUIを管理するクラス
/// </summary>
class EnemyReactionUI {
private:
	/// ============================== ///
	///		構造体
	/// ============================== ///

	/// <summary>
	/// リアクション
	/// </summary>
	struct Reaction {
		std::unique_ptr<Norm::Object3d> object;
		Norm::Vector3 basePosition;
		float dirTimer;
		bool isActive = false;
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
	/// 感知リアクションのスポーン
	/// </summary>
	/// <param name="_subjectPos">対象の座標</param>
	/// <param name="_height">高さ</param>
	/// <param name="_front">前</param>
	void SensingSpawn(const Norm::Vector3& _subjectPos, float _height, float _front);

	/// <summary>
	/// 見失うリアクションのスポーン
	/// </summary>
	/// <param name="_subjectPos">対象の座標</param>
	/// <param name="_height">高さ</param>
	/// <param name="_front">前</param>
	void MissingSpawn(const Norm::Vector3& _subjectPos, float _height, float _front);

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// ゲームカメラのセット
	/// </summary>
	/// <param name="_camera">ゲームカメラ</param>
	void SetGameCamera(Norm::GameCamera* _camera) { camera_ = _camera; }
private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//カメラ
	Norm::GameCamera* camera_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメーター
	json param_;

	//感知リアクション
	static const int kNumSensing = 10;	//一度に表示可能な感知オブジェクトの数
	const float kSensingDirTime = 2.0f;	//感知演出の時間
	uint32_t textureHandleSensing_ = 0u;
	std::array<Reaction, kNumSensing> sensing_;

	//見失うリアクション
	static const int kNumMissing = 10;	//一度に表示可能な見失うオブジェクトの数
	const float kMissingDirTime = 2.0f;	//見失う演出の時間
	uint32_t textureHandleMissing_ = 0u;
	std::array<Reaction, kNumMissing> missing_;



};

