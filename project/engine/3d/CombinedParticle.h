#pragma once
#include "Particle.h"
#include "JsonUtil.h"
#include <MyMath.h>
#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <memory>
#include <queue>

/// <summary>
/// 複合パーティクルを管理するクラス
/// </summary>
class CombinedParticle {
	//パーティクルマネージャーに公開
	friend class ParticleManager;
	//パーティクルエディターシーンに公開
	friend class ParticleEditorScene;
public:
	/// ============================== ///
	///		構造体
	/// ============================== ///

	/// <summary>
	/// 単パーティクル情報(これをつなぎ合わせて複合にする)
	/// </summary>
	struct SingleParticleInfo {
		std::unique_ptr<Particle> particle; //パーティクル本体
		float startTime = 0.0f; //発生開始時間(0~1)
		float endTime = 0.0f; //発生終了時間(0~1)
	};

	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_name">名前</param>
	/// <param name="_comParticleFileName">複合パーティクルのファイル名</param>
	void Initialize(const std::string& _name, const std::string& _comParticleFileName);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// 全てのハンドル名をコンテナとして渡す
	/// </summary>
	/// <returns>全てのハンドル名</returns>
	std::vector<std::string> GetAllHandleName();

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// 基準のトランスフォームをセットする
	/// </summary>
	/// <param name="transform">基準トランスフォーム</param>
	void SetBaseTransform(const TransformEuler& transform);
	/// <summary>
	/// 再生フラグのセット
	/// </summary>
	/// <param name="isPlay">再生するか</param>
	void SetIsPlay(bool isPlay) { isPlay_ = isPlay; }


private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// パーティクルを追加(パーティクルエディター用)
	/// </summary>
	/// <param name="_fileName">ファイル名</param>
	/// <param name="_startTime">開始時間</param>
	/// <param name="_endTime">終了時間</param>
	/// <returns>ハンドル名(エラーなら"error"を返す)</returns>
	std::string AddParticle(const std::string& _fileName, float _startTime, float _endTime);
	/// <summary>
	/// パーティクルを削除(パーティクルエディター用)
	/// </summary>
	/// <param name="_id">ハンドル名</param>
	void RemoveParticle(const std::string& _handleName);

	/// ============================== ///
	///		getter(エディター専用)
	/// ============================== ///

	/// <summary>
	/// パラメーターの取得
	/// </summary>
	/// <returns>パラメーター</returns>
	const std::unordered_map<std::string, json> GetParams();
	/// <summary>
	/// パーティクルコンテナの取得
	/// </summary>
	/// <returns>パーティクルコンテナ</returns>
	const std::vector<SingleParticleInfo>& GetParticles() { return particles_; }

	/// ============================== ///
	///		setter(エディター専用)
	/// ============================== ///

	/// <summary>
	/// パラメーターのセット
	/// </summary>
	/// <param name="_params">パラメーター</param>
	void SetParams(const std::unordered_map<std::string, json>& _params);
	/// <summary>
	/// パーティクルコンテナのセット
	/// </summary>
	/// <param name="_particles">パーティクルコンテナ(std::move()を使用すること)</param>
	void SetParticles(std::vector<SingleParticleInfo>&& _particles) { particles_ = std::move(_particles); }

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//名前
	std::string name_;

	//パーティクルのコンテナ
	std::vector<SingleParticleInfo> particles_;

	//再生フラグ
	bool isPlay_ = false;

	//全体の尺
	float totalDuration_ = 0.0f;
	//タイマー
	float timer_ = 0.0f;

	//複合最大数
	const uint32_t kMaxCombinedParticles = 8;
};

