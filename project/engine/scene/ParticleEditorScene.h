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
private:
	/// ============================== ///
	///		列挙体
	///	============================== ///
	
	/// <summary>
	/// モード
	/// </summary>
	enum class Mode {
		None,			//未選択
		pGenerate,		//単体パーティクルの生成
		cpGenerate,		//複合パーティクルの生成
		pEdit,			//単体パーティクルの編集
		cpEdit,			//複合パーティクルの編集
	};
	/// <summary>
	/// オプション
	/// </summary>
	enum class Option {
		None,			//未選択
		Reset,			//リセット
		Save,			//セーブ
		ShowFileDialog,	//ファイルのダイアログの表示
	};
	/// <summary>
	/// 確認
	/// </summary>
	enum class Check {
		None,		//未選択
		Continue,	//続けるか
		SameName,	//同名か
		EditName,	//編集対象名
	};

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
	std::unique_ptr<CombinedParticle> cParticle_ = nullptr;
	// 編集するパラメーター
	json editParam_;
	std::unordered_map<std::string, json> cEditParam_;

	//ImGui操作用変数
	std::vector<std::string> textureFiles_;
	std::vector<std::string> particleFiles_;
	std::string jsonFileName_;

	Mode mode_ = Mode::None;
	Option option_ = Option::None;
	Check check_ = Check::None;
	bool displayLineEmitter_ = true;
};

