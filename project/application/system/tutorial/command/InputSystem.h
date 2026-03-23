#pragma once
#include <cstdint>

/// <summary>
/// 入力管理クラス
/// </summary>
class InputSystem {
public:
    /// ============================== ///
    ///		メンバ関数
    /// ============================== ///

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();

    /// ============================== ///
    ///		getter
    /// ============================== ///

    /// <summary>
    /// 操作ロック状態を取得
    /// </summary>
    /// <returns>操作ロック状態</returns>
    bool GetIsLocked() const { return isLocked_; }
    
    /// ============================== ///
    ///		getter
    /// ============================== ///

    /// <summary>
    /// 操作ロック状態をセット
    /// </summary>
    /// <param name="_isLocked">操作ロック状態</param>
    void SetIsLocked(bool _isLocked) { isLocked_ = _isLocked; }

private:
    //操作ロックしているか
    bool isLocked_ = false;
};
