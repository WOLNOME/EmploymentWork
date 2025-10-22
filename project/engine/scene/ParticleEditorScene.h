#pragma once
#include "BaseScene.h"
#include "DevelopCamera.h"
#include "json.hpp"
#include "WorldTransform.h"
#include <vector>
#include <string>

using json = nlohmann::json;

/// <summary>
/// パーティクルエディター専用シーン
/// </summary>
class ParticleEditorScene : public BaseScene {
public:
	/// ============================== ///
	///		メンバ関数
	///	============================== ///

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 終了時
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// デバッグ処理
	/// </summary>
	void DebugWithImGui() override;

private:
	/// ============================== ///
	///		非公開メンバ関数
	///	============================== ///

	/// <summary>
	/// 最初のメニュー画面時処理
	/// </summary>
	void StartWithImGui();
	/// <summary>
	/// 新規作成時処理
	/// </summary>
	void GenerateWithImGui();
	/// <summary>
	/// 編集時処理
	/// </summary>
	void EditWithImGui();
	/// <summary>
	/// リセット処理
	/// </summary>
	void ResetWithImGui();
	/// <summary>
	/// セーブ処理
	/// </summary>
	void SaveWithImGui();
	/// <summary>
	/// エディターのメイン処理
	/// </summary>
	void Editor();

	/// ============================== ///
	///		メンバ変数
	///	============================== ///

	//インプット
	Input* input_ = nullptr;
	//開発用カメラ
	std::unique_ptr<DevelopCamera> camera_ = nullptr;

	//天球と地面
	std::unique_ptr<Object3d> skydome_ = nullptr;
	uint32_t textureHandleGround_ = EOF;
	std::unique_ptr<Object3d> ground_ = nullptr;

	// 編集するパーティクル
	std::unique_ptr<Particle> particle_ = nullptr;
	// 編集するパラメーター
	json editParam_;

	//ImGui操作用変数
	std::vector<std::string> textureFiles_;
	std::vector<std::string> particleFiles_;
	std::string jsonFileName_;
	bool showFileDialog_ = false;
	bool isGenerateMode_ = false;
	bool isEditMode_ = false;
	bool isReset_ = false;
	bool isSave_ = false;
	bool checkContinue_ = false;
	bool checkSameName_ = false;
	bool checkEditName_ = false;
	bool displayLineEmitter_ = true;
};

