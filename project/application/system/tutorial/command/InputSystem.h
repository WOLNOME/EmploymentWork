#pragma once
#include <cstdint>

//前方宣言
class Player;

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
    ///		setter
    /// ============================== ///

    /// <summary>
    /// 操作ロック状態をセット
    /// </summary>
    /// <param name="_isLocked">操作ロック状態</param>
    void SetIsLocked(bool _isLocked);
    /// <summary>
    /// プレイヤーをセット
    /// </summary>
    /// <param name="_player">プレイヤー</param>
    void SetPlayer(Player* _player) { player_ = _player; }

private:
    /// ============================== ///
    ///		インスタンス
    /// ============================== ///

    Player* player_ = nullptr;

    /// ============================== ///
    ///		メンバ変数
    /// ============================== ///

    //操作ロックしているか
    bool isLocked_ = false;
};
