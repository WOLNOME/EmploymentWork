#pragma once
#include <cstdint>

/// 前方宣言
class CameraManager;
class UISystem;

/// <summary>
/// カメラ制御システム
/// </summary>
class CameraSystem {
public:
    /// ============================== ///
    ///		メンバ関数
    /// ============================== ///

    /// <summary>
    /// CameraManagerの設定
    /// </summary>
    /// <param name="manager">借用するCameraManager</param>
    void SetCameraManager(CameraManager* manager);

    /// <summary>
    /// UIシステムの設定
    /// </summary>
    /// <param name="ui">暗転制御用UI</param>
    void SetUISystem(UISystem* ui);

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update(float deltaTime);

    /// <summary>
    /// カメラトランスフォームの直接設定
    /// </summary>
    void SetTransform(
        float posX, float posY, float posZ,
        float rotX, float rotY, float rotZ);

    /// <summary>
    /// カメラ演出の開始
    /// </summary>
    void StartCameraMove(
        float targetPosX, float targetPosY, float targetPosZ,
        float targetRotX, float targetRotY, float targetRotZ,
        float duration);

    /// <summary>
    /// 演出中かの取得
    /// </summary>
    bool IsMoving() const;

private:
    /// ============================== ///
    ///		内部処理
    /// ============================== ///

    void BeginFadeOut();
    void BeginFadeIn();

private:
    /// ============================== ///
    ///		メンバ変数
    /// ============================== ///

    CameraManager* cameraManager_ = nullptr;
    UISystem* ui_ = nullptr;

    bool isMoving_ = false;
    float moveTimer_ = 0.0f;
    float moveDuration_ = 0.0f;

    float startPos_[3] = {};
    float startRot_[3] = {};
    float targetPos_[3] = {};
    float targetRot_[3] = {};
};
