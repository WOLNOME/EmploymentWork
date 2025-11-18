#include "TimeScaleManager.h"
#include <MyMath.h>

void TimeScaleManager::Initialize() {
	//メンバ変数の初期化
	isPlay_ = true;
	isActive_ = false;
	activeTime_ = 0.0f;
	activeTimer_ = 0.0f;
	playInterval_ = 0u;
	playIntervalCounter_ = 0u;
}

void TimeScaleManager::Update() {
    //稼働中なら
    if (isActive_) {
        //稼働タイマーを更新
        activeTimer_ += kDeltaTime;
        //もし稼働時間を超えたら
        if (activeTimer_ >= activeTime_) {
            //稼働終了
            isActive_ = false;
            isPlay_ = true;
        }
        else {
            //再生間隔カウンターを更新
            playIntervalCounter_++;
            //もし再生間隔フレームに達したら
            if (playIntervalCounter_ >= playInterval_) {
                //再生
                isPlay_ = true;
                //カウンターリセット
                playIntervalCounter_ = 0u;
            }
            else {
                //停止
                isPlay_ = false;
            }
		}
	}
}

void TimeScaleManager::ApplyTimeEffect(float _activeTime, uint32_t _playInterval) {
    //メンバ変数にセット
    activeTime_ = _activeTime;
    playInterval_ = _playInterval;
    //稼働タイマーリセット
    activeTimer_ = 0.0f;
    //再生間隔カウンターリセット
    playIntervalCounter_ = 0u;
    //稼働中フラグオン
	isActive_ = true;
}
