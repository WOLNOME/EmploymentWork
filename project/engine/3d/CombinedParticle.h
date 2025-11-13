#pragma once
#include "Particle.h"
#include <string>
#include <cstdint>
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
		bool isDisplay = true; //表示フラグ
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
	/// <returns>ハンドル</returns>
	uint32_t AddParticle(const std::string& _fileName, float _startTime, float _endTime);
	/// <summary>
	/// パーティクルを削除(パーティクルエディター用)
	/// </summary>
	/// <param name="_id">ハンドル</param>
	void RemoveParticle(uint32_t _id);

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//名前
	std::string name_;

	//パーティクルのコンテナ
	std::unordered_map<uint32_t, SingleParticleInfo> particles_;

	//全体の尺
	float totalDuration_ = 0.0f;

	//複合最大数
	const uint32_t kMaxCombinedParticles = 8;

	//最新のID
	uint32_t useId_ = 0;
};

