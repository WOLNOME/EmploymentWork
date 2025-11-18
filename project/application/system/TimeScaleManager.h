#pragma once
#include <cstdint>

/// <summary>
/// 演出上またはシステム上の時間経過を管理するクラス
/// </summary>
class TimeScaleManager {
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

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// 再生可能かどうか取得
	/// </summary>
	/// <returns>再生中か</returns>
	bool GetIsPlay() const { return isPlay_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// 時間効果を適用する (新しい命令があったら上書きされる)
	/// </summary>
	/// <param name="_activeTime">稼働時間(秒)</param>
	/// <param name="_playInterval">再生インターバル(フレーム) nフレームにで1回再生</param>
	/// <returns>適用できたか</returns>
	void ApplyTimeEffect(float _activeTime, uint32_t _playInterval);

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///


	/// ============================== ///
	///		インスタンス
	/// ============================== ///


	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//再生中かどうか
	bool isPlay_ ;

	//稼働中かどうか
	bool isActive_;
	//稼働時間
	float activeTime_;
	//稼働タイマー
	float activeTimer_;
	//再生間隔フレーム
	uint32_t playInterval_;
	//再生間隔カウンター
	uint32_t playIntervalCounter_;

};

