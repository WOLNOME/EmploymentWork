#pragma once
#include <JsonUtil.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

//アプリケーション
#include <application/system/tutorial/command/executor/CommandExecutor.h>

// 前方宣言
class CameraManager;
class Player;
class PlayerWeaponManager;
class PlayerUI;
class ItemManager;
class EnemyManager;

/// <summary>
/// チュートリアルマネージャー
/// </summary>
class TutorialManager {
private:
    /// ============================== ///
    ///		構造体
    /// ============================== ///

    /// <summary>
    /// コマンド1つ分
    /// </summary>
    struct CommandData {
        std::string name;
        std::unordered_map<std::string, json> params;
    };

    /// <summary>
    /// ステップ
    /// </summary>
    struct Step {
        std::vector<CommandData> commands;
        std::string waitCondition;
    };

    /// <summary>
    /// シーケンス
    /// </summary>
    struct Sequence {
        std::vector<Step> steps;
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
    /// スクリプトファイルをロード
    /// </summary>
    /// <param name="path">ファイルパス</param>
    void LoadFromFile(const std::string& path);
    /// <summary>
    /// チュートリアル開始
    /// </summary>
    /// <param name="sequenceName">シーケンス名</param>
    void Start(const std::string& sequenceName);
    /// <summary>
    /// チュートリアル実行中か
    /// </summary>
    bool IsRunning() const { return currentSequence_; }

    /// ============================== ///
    ///		setter
    /// ============================== ///

    /// <summary>
    /// カメラマネージャーを設定します
    /// </summary>
    /// <param name="cameraManager">カメラマネージャーのインスタンス</param>
    void SetCameraManager(CameraManager* cameraManager);

    /// <summary>
    /// プレイヤーを設定します
    /// </summary>
    /// <param name="player">プレイヤーのインスタンス</param>
    void SetPlayer(Player* player);

    /// <summary>
    /// プレイヤー武器マネージャーを設定します
    /// </summary>
    /// <param name="playerWeaponManager">プレイヤー武器マネージャー</param>
    void SetPlayerWeaponManager(PlayerWeaponManager* playerWeaponManager);

    /// <summary>
    /// プレイヤーUIを設定します
    /// </summary>
    /// <param name="playerUI">プレイヤーUIのインスタンス</param>
    void SetPlayerUI(PlayerUI* playerUI);

    /// <summary>
    /// アイテムマネージャーを設定します
    /// </summary>
    /// <param name="itemManager">アイテムマネージャーのインスタンス</param>
    void SetItemManager(ItemManager* itemManager);

    /// <summary>
    /// エネミーマネージャーを設定します
    /// </summary>
    /// <param name="enemyManager">エネミーマネージャー</param>
    void SetEnemyManager(EnemyManager* enemyManager);

private:
    /// ============================== ///
    ///		メンバ関数(private)
    /// ============================== ///

    /// <summary>
    /// ステップ実行
    /// </summary>
    /// <param name="step">現在のステップ</param>
    void ExecuteStep(const Step& step);

    /// <summary>
    /// 待機条件チェック
    /// </summary>
    /// <param name="condition">現在の待機条件</param>
    /// <returns></returns>
    bool CheckWaitCondition(const std::string& condition) const;

private:
    /// ============================== ///
    ///		メンバ変数
    /// ============================== ///

    //コマンドエグゼキューター
    std::unique_ptr<CommandExecutor> commandExecutor_ = nullptr;

    //全シーケンス
    std::unordered_map<std::string, Sequence> sequences_;

    //現在の状態
    Sequence* currentSequence_ = nullptr;
    std::string currentSequenceName_;
    size_t currentStepIndex_ = 0;

    //待機フラグ
    bool isWaiting_ = false;
    std::string currentWaitCondition_;
};
