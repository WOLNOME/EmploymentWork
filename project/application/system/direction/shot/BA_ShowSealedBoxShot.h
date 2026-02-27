#pragma once
#include "application/system/direction/shot/base/IShotBase.h"

/// <summary>
/// 封印ボックスを見せる演出
/// </summary>
class BA_ShowSealedBoxShot : public IShotBase {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BA_ShowSealedBoxShot(int _index, Norm::TransformEuler _startCameraTransform, Norm::TransformEuler _endCameraTransform, float _duration, Norm::BlackBoard* _blackBoard);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~BA_ShowSealedBoxShot();

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


};

