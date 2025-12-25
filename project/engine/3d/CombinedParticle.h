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
	//複合パーティクルマネージャーに公開
	friend class CombinedParticleManager;
	//パーティクルエディターシーンに公開
	friend class ParticleEditorScene;
public:
	/// ============================== ///
	///		構造体(public)
	/// ============================== ///

	/// <summary>
	/// 単パーティクル情報(これをつなぎ合わせて複合にする)
	/// </summary>
	struct SingleParticleInfo {
		std::unique_ptr<Particle> particle; // パーティクル本体
		float startTime = 0.0f; // 発生開始時間
		float endTime = 0.0f;   // 発生終了時間
	};

private:
	/// ============================== ///
	///		構造体(private)
	/// ============================== ///

	struct PlayInfo {
		bool isPlay = false;		//再生フラグ
		float currentTime = 0.0f;	//現在の時間
		float duration = 0.0f;		//尺
	};

public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CombinedParticle();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_name">名前</param>
	/// <param name="_comParticleFileName">複合パーティクルのファイル名</param>
	void Initialize(const std::string& _name, const std::string& _comParticleFileName);
	/// <summary>
	/// デバッグ処理
	/// </summary>
	void Debug();
	
	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// 基準のトランスフォームを取得する
	/// </summary>
	/// <returns>基準のトランスフォーム</returns>
	const TransformEuler& GetBaseTransform() { return baseTransform_; }
	/// <summary>
	/// 全てのハンドル名をコンテナとして渡す
	/// </summary>
	/// <returns>全てのハンドル名</returns>
	std::vector<std::string> GetAllHandleName();
	/// <summary>
	/// 再生フラグの取得
	/// </summary>
	/// <returns>再生するか</returns>
	bool GetIsPlay() const { return playInfo_.isPlay; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// 基準のトランスフォームをセットする
	/// </summary>
	/// <param name="transform">基準トランスフォーム</param>
	void SetBaseTransform(const TransformEuler& transform) { baseTransform_ = transform; }
	/// <summary>
	/// 再生フラグのセット
	/// </summary>
	/// <param name="isPlay">再生するか</param>
	void SetIsPlay(bool isPlay) { playInfo_.isPlay = isPlay; }

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// 更新処理(複合パーティクルマネージャー用)
	/// </summary>
	void Update();

	/// <summary>
	/// パーティクルを追加(パーティクルエディター用)
	/// </summary>
	/// <param name="_fileName">ファイル名</param>
	/// <param name="_startTime">開始時間</param>
	/// <param name="_endTime">終了時間</param>
	/// <returns>成功したか</returns>
	bool AddParticle(const std::string& _fileName, float _startTime, float _endTime);
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
	/// 再生情報の取得
	/// </summary>
	/// <returns>再生情報</returns>
	const PlayInfo& GetPlayInfo() { return playInfo_; }

	/// ============================== ///
	///		setter(エディター専用)
	/// ============================== ///

	/// <summary>
	/// パラメーターのセット
	/// </summary>
	/// <param name="_params">パラメーター</param>
	void SetParams(const std::unordered_map<std::string, json>& _params);

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//名前
	std::string name_;

	//パーティクルのコンテナ
	std::vector<SingleParticleInfo> particles_;

	//再生情報
	PlayInfo playInfo_;

	//基準のトランスフォーム
	TransformEuler baseTransform_;

	//複合最大数
	const uint32_t kMaxCombinedParticles = 8;
};

