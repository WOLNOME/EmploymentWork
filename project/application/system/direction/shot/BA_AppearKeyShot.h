#pragma once
#include "application/system/direction/shot/base/IShotBase.h"
#include <JsonUtil.h>

/// <summary>
/// 鍵を出現させる演出
/// </summary>
class BA_AppearKeyShot : public IShotBase {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BA_AppearKeyShot(int _index, Norm::TransformEuler _startCameraTransform, Norm::TransformEuler _endCameraTransform, float _duration, Norm::BlackBoard* _blackBoard);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~BA_AppearKeyShot();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// デバッグ
	/// </summary>
	void Debug() override;


private:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	//パラメーター
	json param_;


};

