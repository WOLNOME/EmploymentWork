#include "ParticleEditorScene.h"
#include "SceneManager.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "JsonUtil.h"
#include <filesystem>
#include <iostream>

void ParticleEditorScene::Initialize() {
	//シーン共通の初期化
	BaseScene::Initialize();

	//インプット
	input_ = Input::GetInstance();

	//カメラの生成と初期化
	camera_ = std::make_unique<DevelopCamera>();
	camera_->Initialize();
	camera_->worldTransform.translate = { 0.0f,50.0f,0.0f };
	camera_->worldTransform.rotate = { 0.2f,0.0f,0.0f };
	//カメラをセット
	Object3dManager::GetInstance()->SetCamera(camera_.get());
	LineManager::GetInstance()->SetCamera(camera_.get());
	ParticleManager::GetInstance()->SetCamera(camera_.get());
	//シーンライトをセット
	Object3dManager::GetInstance()->SetSceneLight(sceneLight_.get());

	//天球と地面の生成と初期化
	skydome_ = std::make_unique<Object3d>();
	skydome_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Skydome"), "skydome");
	textureHandleGround_ = TextureManager::GetInstance()->LoadTexture("grid.png");
	ground_ = std::make_unique<Object3d>();
	ground_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Ground"), "ground");
	ground_->SetTexture(textureHandleGround_);

}

void ParticleEditorScene::Finalize() {
}

void ParticleEditorScene::Update() {
	//シーン共通の更新
	BaseScene::Update();
	//カメラの更新
	camera_->Update();

	//リセットコマンド
	if (state_.check == Check::kNone) {
		if (input_->TriggerKey(DIK_ESCAPE)) {
			state_.option = Option::kReset;
		}
	}
}

void ParticleEditorScene::DebugWithImGui() {
	//ImGui操作
#ifdef _DEBUG
	//最初のメニュー画面時操作
	StartWithImGui();

	//エディター画面時の操作
	EditWithImGui();

	//オプション処理
	OptionWithImGui();
	//確認処理
	CheckWithImGui();

#endif //_DEBUG
}

void ParticleEditorScene::StartWithImGui() {
#ifdef _DEBUG
	//最初の操作
	if (state_.mode == Mode::kNone && state_.check == Check::kNone) {
		ImGui::SetNextWindowPos(ImVec2(450, 300), ImGuiCond_FirstUseEver);
		ImGui::Begin("メニュー");
		ImGui::Text("パーティクルエディターへようこそ！\n以下の項目から希望のオプションを選択して下さい。\n");

		if (ImGui::Button("パーティクルを新しく作成する")) {
			state_.mode = Mode::kEdit;
			//パーティクルの生成
			cParticle_ = std::make_unique<CombinedParticle>();
			//パーティクルの初期化
			cParticle_->Initialize(ParticleManager::GetInstance()->GenerateName("Sample"), "Basic");
			//エミッターの位置を調整
			cParticle_->SetBaseTransform(TransformEuler({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }));
			//jsonデータをロード
			cEditParam_ = cParticle_->GetParams();
			//カメラの位置をセット
			camera_->worldTransform.translate = { 0.0f,4.0f,-20.0f };
			camera_->worldTransform.rotate = { 0.03f,0.0f,0.0f };
		}
		if (ImGui::Button("パーティクルを編集する")) {
			state_.check = Check::kEditName;
			//パーティクルの生成
			cParticle_ = std::make_unique<CombinedParticle>();
		}
		ImGui::End();
	}

#endif //_DEBUG
}

void ParticleEditorScene::EditWithImGui() {
#ifdef _DEBUG
	if (state_.mode == Mode::kEdit) {
		Editor();
	}
#endif //_DEBUG
}

void ParticleEditorScene::OptionWithImGui() {
#ifdef _DEBUG
	//オプションごとの処理
	switch (state_.option) {
	case ParticleEditorScene::Option::kNone:
	{
		//何もしない
		break;
	}
	case ParticleEditorScene::Option::kReset:
	{
		//リセット(Escapeキーによる)
		ImGui::OpenPopup("確認");
		ImGui::SetNextWindowPos(ImVec2(510, 30));
		if (ImGui::BeginPopupModal("確認", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
			ImGui::Text("リセットしますか？\n[注意] 編集中のパラメーターは反映されません\n ");
			if (ImGui::Button("はい", ImVec2(120, 0))) {
				sceneManager_->SetNextScene("PARTICLEEDITOR");
				state_.option = Option::kNone;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("いいえ", ImVec2(120, 0))) {
				state_.option = Option::kNone;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		break;
	}
	case ParticleEditorScene::Option::kSave:
	{
		//セーブ
		ImGui::SetNextWindowPos(ImVec2(470, 280));
		ImGui::Begin("パーティクルのセーブ");

		ImGui::Text("作成したパーティクルの名前を入力してください");
		char buffer[256];
		strncpy_s(buffer, sizeof(buffer), particleFileName_.c_str(), _TRUNCATE);
		buffer[sizeof(buffer) - 1] = '\0';
		if (ImGui::InputText("入力欄", buffer, sizeof(buffer))) {
			particleFileName_ = buffer;
		}
		if (particleFileName_.size() != 0) {
			if (ImGui::Button("名前を確定する")) {
				//該当ファイル内全てのJsonファイルを削除
				std::string targetDir = "Resources/particles/" + particleFileName_;
				for (const auto& entry : std::filesystem::directory_iterator(targetDir)) {
					//jsonファイルなら
					if (entry.is_regular_file() && entry.path().extension() == ".json") {
						std::filesystem::remove(entry.path());
					}
				}
				//保持しているJsonファイル名を走査
				for (const auto& name : jsonFileNames_) {
					//JsonUtilを使ってパーティクルを保存
					JsonUtil::CreateJson(name, "Resources/particles/" + particleFileName_, cEditParam_[name]);
				}
				//編集をさらに続けるかの確認へ
				state_.check = Check::kContinue;
			}
		}

		//表示を消す
		if (ImGui::Button("Back")) {
			state_.option = Option::kNone;
		}
		ImGui::End();

		break;
	}
	case ParticleEditorScene::Option::kShowParticleFileDialog:
	{
		//検索済みのパーティクル一覧を表示
		ImGui::SetNextWindowFocus();
		ImGui::SetNextWindowPos(ImVec2(860, 280));
		ImGui::Begin("パーティクル一覧");
		for (const auto& file : particleFiles_) {
			if (ImGui::Selectable(file.c_str())) {
				//パーティクルファイル名をクリア
				particleFileName_.clear();
				//jsonファイル名コンテナをクリア
				jsonFileNames_.clear();

				//パーティクルファイル名を保存
				particleFileName_ = file;

				//更にそのファイルの中を走査
				for (const auto& entry : std::filesystem::directory_iterator("Resources/particles/" + file)) {
					//jsonファイルなら
					if (entry.is_regular_file() && entry.path().extension() == ".json") {
						//entryから.jsonをカット
						std::string cutJson = std::filesystem::path(entry.path().string()).stem().string();
						jsonFileNames_.push_back(cutJson);  //ファイル名を保存
					}
				}
				state_.option = Option::kNone;	  //ウィンドウを閉じる
			}
		}
		ImGui::End();

		break;
	}
	case ParticleEditorScene::Option::kShowTextureFileDialog:
	{
		//検索済みのテクスチャ一覧を表示
		if (state_.option == Option::kShowTextureFileDialog) {
			ImGui::SetNextWindowPos(ImVec2(360, 80));
			ImGui::Begin("テクスチャ一覧");
			for (const auto& file : textureFiles_) {
				if (ImGui::Selectable(file.c_str())) {
					selectedTexture_ = file;  //選択したファイル名を保存
					state_.option = Option::kNone;  //ウィンドウを閉じる
					isChangeTexture_ = true;  //テクスチャが変更された
				}
			}
			ImGui::End();
		}
		break;
	}
	default:
		break;
	}
#endif // _DEBUG
}

void ParticleEditorScene::CheckWithImGui() {
#ifdef _DEBUG
	//確認ごとの処理
	switch (state_.check) {
	case ParticleEditorScene::Check::kNone:
		//何もしない
		break;
	case ParticleEditorScene::Check::kContinue:
	{
		//続けるかの確認
		ImGui::OpenPopup("パーティクルの作成に成功しました");
		ImGui::SetNextWindowPos(ImVec2(510, 30));
		if (ImGui::BeginPopupModal("パーティクルの作成に成功しました", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
			ImGui::Text("作業を終了しますか？\n ");
			if (ImGui::Button("はい", ImVec2(120, 0))) {
				sceneManager_->SetNextScene("PARTICLEEDITOR");
				state_.option = Option::kNone;
				state_.check = Check::kNone;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("いいえ", ImVec2(120, 0))) {
				state_.option = Option::kNone;
				state_.check = Check::kNone;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		break;
	}
	case ParticleEditorScene::Check::kEditName:
	{
		//編集対象名の確認
		ImGui::OpenPopup("パーティクル名の入力");
		ImGui::SetWindowSize(ImVec2(510, 120));
		if (ImGui::BeginPopupModal("パーティクル名の入力", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
			ImGui::Text("{particles}フォルダ内のパーティクルファイル名を入力してください\n ");
			//particlesから参照
			if (ImGui::Button("{particles}フォルダから参照する")) {
				state_.option = Option::kShowParticleFileDialog;
				particleFiles_.clear();
				//"Resources/particles"フォルダを走査
				for (const auto& entry : std::filesystem::directory_iterator("Resources/particles")) {
					//フォルダ内の全てのファイルを保存
					particleFiles_.push_back(entry.path().filename().string());
				}
			}
			//確認欄
			ImGui::Text("選択中のファイル : %s", particleFileName_.c_str());

			//確定ボタン(該当ファイル内の全てのJSONファイルを読み込む)
			if (ImGui::Button("このファイルを編集する")) {
				//ファイル内全てのJSONファイルを保持
				for (const auto& entry : std::filesystem::directory_iterator("Resources/particles/" + particleFileName_)) {
					//jsonファイルなら
					if (entry.is_regular_file() && entry.path().extension() == ".json") {
						//entryから.jsonをカット
						std::string cutJson = std::filesystem::path(entry.path().string()).stem().string();
						//ファイル名を保存
						jsonFileNames_.push_back(cutJson);
					}
				}
				//編集モードへ移行
				state_.mode = Mode::kEdit;
				//パーティクルの初期化
				cParticle_->Initialize(ParticleManager::GetInstance()->GenerateName("Sample"), particleFileName_);
				//エミッターの位置を調整
				cParticle_->SetBaseTransform(TransformEuler({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }));
				//編集用jsonデータをロード
				cEditParam_ = cParticle_->GetParams();
				//カメラの位置をセット
				camera_->worldTransform.translate = { 0.0f,4.0f,-20.0f };
				camera_->worldTransform.rotate = { 0.03f,0.0f,0.0f };

				//ウィンドウを閉じる
				ImGui::CloseCurrentPopup();
			}

			//戻る
			if (ImGui::Button("Back")) {
				//オプションをクリア
				state_.option = Option::kNone;
				//確認をクリア
				state_.check = Check::kNone;
				//パーティクルファイル名をクリア
				particleFileName_ = std::string();
				//jsonファイル名コンテナをクリア
				jsonFileNames_.clear();
				//パーティクルを破棄
				cParticle_.release();

				//ウィンドウを閉じる
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		break;
	}
	default:
		break;
	}

#endif // _DEBUG
}

void ParticleEditorScene::Editor() {
#ifdef _DEBUG
	//パラメーター編集処理のラムダ式
	auto paramProcess = [this]() {
		//パラメーター
		ImGui::SetNextWindowPos(ImVec2(10, 80), ImGuiCond_FirstUseEver);
		ImGui::Begin("パーティクルのパラメーター");
		//テクスチャを写す
		isChangeTexture_ = false;
		selectedTexture_ = particle_->GetParam()["Texture"];
		if (ImGui::CollapsingHeader("テクスチャの設定")) {
			ImGui::Text("選択中のテクスチャ : %s", selectedTexture_.c_str());
			//フォルダ内のテクスチャを検索
			if (ImGui::Button("{textures}フォルダ内のテクスチャを検索")) {
				state_.option = Option::kShowTextureFileDialog;
				textureFiles_.clear();
				for (const auto& entry : std::filesystem::directory_iterator("Resources/textures")) {
					if (entry.is_regular_file() && entry.path().extension() == ".png") {
						textureFiles_.push_back(entry.path().filename().string());
					}
				}
			}
		}

		//色を写す
		Vector4 startColorMax = { editParam_["StartColor"]["Max"]["x"],editParam_["StartColor"]["Max"]["y"],editParam_["StartColor"]["Max"]["z"],editParam_["StartColor"]["Max"]["w"] };
		Vector4 startColorMin = { editParam_["StartColor"]["Min"]["x"],editParam_["StartColor"]["Min"]["y"],editParam_["StartColor"]["Min"]["z"],editParam_["StartColor"]["Min"]["w"] };
		Vector4 endColorMax = { editParam_["EndColor"]["Max"]["x"],editParam_["EndColor"]["Max"]["y"],editParam_["EndColor"]["Max"]["z"],editParam_["EndColor"]["Max"]["w"] };
		Vector4 endColorMin = { editParam_["EndColor"]["Min"]["x"],editParam_["EndColor"]["Min"]["y"],editParam_["EndColor"]["Min"]["z"],editParam_["EndColor"]["Min"]["w"] };
		if (ImGui::CollapsingHeader("色の設定")) {
			ImGui::ColorEdit4("StartColorMax", &startColorMax.x);
			ImGui::ColorEdit4("StartColorMin", &startColorMin.x);
			ImGui::ColorEdit4("EndColorMax", &endColorMax.x);
			ImGui::ColorEdit4("EndColorMin", &endColorMin.x);
		}
		//初期トランスフォームを写す
		Vector3 rotateMax = { editParam_["GrainTransform"]["Rotate"]["Max"]["x"],editParam_["GrainTransform"]["Rotate"]["Max"]["y"] ,editParam_["GrainTransform"]["Rotate"]["Max"]["z"] };
		Vector3 rotateMin = { editParam_["GrainTransform"]["Rotate"]["Min"]["x"],editParam_["GrainTransform"]["Rotate"]["Min"]["y"] ,editParam_["GrainTransform"]["Rotate"]["Min"]["z"] };
		Vector3 scaleMax = { editParam_["GrainTransform"]["Scale"]["Max"]["x"],editParam_["GrainTransform"]["Scale"]["Max"]["y"],editParam_["GrainTransform"]["Scale"]["Max"]["z"] };
		Vector3 scaleMin = { editParam_["GrainTransform"]["Scale"]["Min"]["x"],editParam_["GrainTransform"]["Scale"]["Min"]["y"],editParam_["GrainTransform"]["Scale"]["Min"]["z"] };
		if (ImGui::CollapsingHeader("初期トランスフォームの設定")) {
			if (ImGui::TreeNode("Rotate")) {
				ImGui::DragFloat3("RotateMax", &rotateMax.x, 0.1f);
				ImGui::DragFloat3("RotateMin", &rotateMin.x, 0.1f);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Scale")) {
				ImGui::DragFloat3("ScaleMax", &scaleMax.x, 0.1f, 0.0f, 10.0f);
				ImGui::DragFloat3("ScaleMin", &scaleMin.x, 0.1f, 0.0f, 10.0f);
				ImGui::TreePop();
			}
		}
		//回転を写す
		Vector3 startRotateMax = { editParam_["StartRotate"]["Max"]["x"],editParam_["StartRotate"]["Max"]["y"],editParam_["StartRotate"]["Max"]["z"] };
		Vector3 startRotateMin = { editParam_["StartRotate"]["Min"]["x"],editParam_["StartRotate"]["Min"]["y"],editParam_["StartRotate"]["Min"]["z"] };
		Vector3 endRotateMax = { editParam_["EndRotate"]["Max"]["x"],editParam_["EndRotate"]["Max"]["y"],editParam_["EndRotate"]["Max"]["z"] };
		Vector3 endRotateMin = { editParam_["EndRotate"]["Min"]["x"],editParam_["EndRotate"]["Min"]["y"],editParam_["EndRotate"]["Min"]["z"] };
		if (ImGui::CollapsingHeader("更新回転の設定")) {
			ImGui::DragFloat3("StartRotateMax", &startRotateMax.x, 0.1f);
			ImGui::DragFloat3("StartRotateMin", &startRotateMin.x, 0.1f);
			ImGui::DragFloat3("EndRotateMax", &endRotateMax.x, 0.1f);
			ImGui::DragFloat3("EndRotateMin", &endRotateMin.x, 0.1f);
		}
		//サイズを写す
		float startSizeMax = editParam_["StartSize"]["Max"];
		float startSizeMin = editParam_["StartSize"]["Min"];
		float endSizeMax = editParam_["EndSize"]["Max"];
		float endSizeMin = editParam_["EndSize"]["Min"];
		if (ImGui::CollapsingHeader("更新サイズの設定")) {
			ImGui::DragFloat("StartSizeMax", &startSizeMax, 0.1f, startSizeMin);
			ImGui::DragFloat("StartSizeMin", &startSizeMin, 0.1f, 0.0f, startSizeMax);
			ImGui::DragFloat("EndSizeMax", &endSizeMax, 0.1f, endSizeMin);
			ImGui::DragFloat("EndSizeMin", &endSizeMin, 0.1f, 0.0f, endSizeMax);
		}
		//速度を写す
		Vector3 velocityMax = { editParam_["Velocity"]["Max"]["x"],editParam_["Velocity"]["Max"]["y"],editParam_["Velocity"]["Max"]["z"] };
		Vector3 velocityMin = { editParam_["Velocity"]["Min"]["x"],editParam_["Velocity"]["Min"]["y"],editParam_["Velocity"]["Min"]["z"] };
		if (ImGui::CollapsingHeader("速度の設定")) {
			ImGui::DragFloat3("VelocityMax", &velocityMax.x, 0.1f);
			ImGui::DragFloat3("VelocityMin", &velocityMin.x, 0.1f);
		}
		//寿命を写す
		float lifeTimeMax = editParam_["LifeTime"]["Max"];
		float lifeTimeMin = editParam_["LifeTime"]["Min"];
		if (ImGui::CollapsingHeader("寿命の設定")) {
			ImGui::DragFloat("LifeTimeMax", &lifeTimeMax, 0.1f, lifeTimeMin);
			ImGui::DragFloat("LifeTimeMin", &lifeTimeMin, 0.1f, 0.0f, lifeTimeMax);
		}
		//粒の最大数を写す
		int maxGrains = editParam_["MaxGrains"];
		if (ImGui::CollapsingHeader("粒の最大数")) {
			//推奨値の計算
			int RecommendValue;
			switch (particle_->emitter_.generateMethod) {
			case Particle::GenerateMethod::Random:
				RecommendValue = int(lifeTimeMax * editParam_["EmitRate"]);
				break;
			case Particle::GenerateMethod::Clump:
				RecommendValue = int(lifeTimeMax * editParam_["EmitRate"] * particle_->emitter_.clumpNum);
				break;
			default:
				break;
			}
			ImGui::Text("推奨値 : %d (最低限の数で回せる値)", RecommendValue);
			if (ImGui::Button("推奨値を適用")) {
				maxGrains = RecommendValue;
			}
			ImGui::DragInt("MaxGrains", &maxGrains, 1);
		}
		//1秒あたりの粒の生成量を写す
		int emitRate = editParam_["EmitRate"];
		if (ImGui::CollapsingHeader("1秒あたりの粒の生成量")) {
			ImGui::DragInt("EmitRate", &emitRate, 1);
		}
		//ブレンドモードを写す
		int blendMode = editParam_["BlendMode"];
		const char* blendModeList[] = { "None","Normal","Add","Subtract","Multiply","Screen","Exclusion" };
		if (ImGui::CollapsingHeader("ブレンドモード")) {
			ImGui::Combo("BlendMode", &blendMode, blendModeList, (int)BlendMode::kMaxBlendModeNum);
		}
		//形状を写す
		int primitive = editParam_["Primitive"];
		bool isShapeChange = false;
		const char* primitiveList[] = { "Shere","Cube","SkyBox","Plane","Ring","Tube" };
		if (ImGui::CollapsingHeader("形状")) {
			if (ImGui::Combo("Primitive", &primitive, primitiveList, (int)Shape::ShapeKind::kMaxShapeKindNum)) {
				//形状の変更通知
				isShapeChange = true;
			}
		}

		//editParamに変更を反映
		editParam_["Texture"] = selectedTexture_;
		editParam_["StartColor"]["Max"]["x"] = startColorMax.x;
		editParam_["StartColor"]["Max"]["y"] = startColorMax.y;
		editParam_["StartColor"]["Max"]["z"] = startColorMax.z;
		editParam_["StartColor"]["Max"]["w"] = startColorMax.w;
		editParam_["StartColor"]["Min"]["x"] = startColorMin.x;
		editParam_["StartColor"]["Min"]["y"] = startColorMin.y;
		editParam_["StartColor"]["Min"]["z"] = startColorMin.z;
		editParam_["StartColor"]["Min"]["w"] = startColorMin.w;
		editParam_["EndColor"]["Max"]["x"] = endColorMax.x;
		editParam_["EndColor"]["Max"]["y"] = endColorMax.y;
		editParam_["EndColor"]["Max"]["z"] = endColorMax.z;
		editParam_["EndColor"]["Max"]["w"] = endColorMax.w;
		editParam_["EndColor"]["Min"]["x"] = endColorMin.x;
		editParam_["EndColor"]["Min"]["y"] = endColorMin.y;
		editParam_["EndColor"]["Min"]["z"] = endColorMin.z;
		editParam_["EndColor"]["Min"]["w"] = endColorMin.w;
		editParam_["GrainTransform"]["Rotate"]["Max"]["x"] = rotateMax.x;
		editParam_["GrainTransform"]["Rotate"]["Max"]["y"] = rotateMax.y;
		editParam_["GrainTransform"]["Rotate"]["Max"]["z"] = rotateMax.z;
		editParam_["GrainTransform"]["Rotate"]["Min"]["x"] = rotateMin.x;
		editParam_["GrainTransform"]["Rotate"]["Min"]["y"] = rotateMin.y;
		editParam_["GrainTransform"]["Rotate"]["Min"]["z"] = rotateMin.z;
		editParam_["GrainTransform"]["Scale"]["Max"]["x"] = scaleMax.x;
		editParam_["GrainTransform"]["Scale"]["Max"]["y"] = scaleMax.y;
		editParam_["GrainTransform"]["Scale"]["Max"]["z"] = scaleMax.z;
		editParam_["GrainTransform"]["Scale"]["Min"]["x"] = scaleMin.x;
		editParam_["GrainTransform"]["Scale"]["Min"]["y"] = scaleMin.y;
		editParam_["GrainTransform"]["Scale"]["Min"]["z"] = scaleMin.z;
		editParam_["StartRotate"]["Max"]["x"] = startRotateMax.x;
		editParam_["StartRotate"]["Max"]["y"] = startRotateMax.y;
		editParam_["StartRotate"]["Max"]["z"] = startRotateMax.z;
		editParam_["StartRotate"]["Min"]["x"] = startRotateMin.x;
		editParam_["StartRotate"]["Min"]["y"] = startRotateMin.y;
		editParam_["StartRotate"]["Min"]["z"] = startRotateMin.z;
		editParam_["EndRotate"]["Max"]["x"] = endRotateMax.x;
		editParam_["EndRotate"]["Max"]["y"] = endRotateMax.y;
		editParam_["EndRotate"]["Max"]["z"] = endRotateMax.z;
		editParam_["EndRotate"]["Min"]["x"] = endRotateMin.x;
		editParam_["EndRotate"]["Min"]["y"] = endRotateMin.y;
		editParam_["EndRotate"]["Min"]["z"] = endRotateMin.z;
		editParam_["StartSize"]["Max"] = startSizeMax;
		editParam_["StartSize"]["Min"] = startSizeMin;
		editParam_["EndSize"]["Max"] = endSizeMax;
		editParam_["EndSize"]["Min"] = endSizeMin;
		editParam_["Velocity"]["Max"]["x"] = velocityMax.x;
		editParam_["Velocity"]["Max"]["y"] = velocityMax.y;
		editParam_["Velocity"]["Max"]["z"] = velocityMax.z;
		editParam_["Velocity"]["Min"]["x"] = velocityMin.x;
		editParam_["Velocity"]["Min"]["y"] = velocityMin.y;
		editParam_["Velocity"]["Min"]["z"] = velocityMin.z;
		editParam_["LifeTime"]["Max"] = lifeTimeMax;
		editParam_["LifeTime"]["Min"] = lifeTimeMin;
		editParam_["MaxGrains"] = maxGrains;
		editParam_["EmitRate"] = emitRate;
		editParam_["BlendMode"] = blendMode;
		editParam_["Primitive"] = primitive;
		//パーティクルに反映
		particle_->SetParam(editParam_);
		//形状の変更通知を受け取ったら
		if (isShapeChange) {
			//パーティクルの形状を変更
			particle_->ShapeChange();
		}
		//テクスチャの変更通知を受け取ったら
		if (isChangeTexture_) {
			//パーティクルのテクスチャを変更
			particle_->TextureChange();
		}

		//セーブボタン
		if (ImGui::Button("セーブ")) {
			state_.option = Option::kSave;
		}
		ImGui::End();
		};

	//エミッター編集処理のラムダ式
	auto emitterProcess = [this]() {
		//エミッター
		ImGui::SetNextWindowPos(ImVec2(1000, 80), ImGuiCond_FirstUseEver);
		ImGui::Begin("エミッター");
		//エミッター可視化用ライン登録処理
		ImGui::Checkbox("エミッターの枠を表示する", &displayLineEmitter_);
		if (displayLineEmitter_) {
			//AABBを作成
			AABB aabb;
			aabb.max = particle_->emitter_.transform.translate + particle_->emitter_.transform.scale;
			aabb.min = particle_->emitter_.transform.translate - particle_->emitter_.transform.scale;

			Vector4 color = { 1,0,0,1 };

			MyMath::CreateLineAABB(aabb, color);
		}
		//エミッターのトランスフォーム
		if (ImGui::CollapsingHeader("エミッターのトランスフォーム")) {
			ImGui::DragFloat3("平行移動", &particle_->emitter_.transform.translate.x, 0.1f);
			ImGui::DragFloat3("拡縮", &particle_->emitter_.transform.scale.x, 0.1f, 0.0f);
		}
		//生成アルゴリズム
		if (ImGui::CollapsingHeader("生成アルゴリズム")) {
			ImGui::Checkbox("生成するか(isPlay)", &particle_->emitter_.isPlay);
			if (ImGui::TreeNode("生成方法")) {
				//現在の生成方法を表示
				const char* methods[] = { "Random","Clump" };
				const char* currentMethod = "";
				switch (particle_->emitter_.generateMethod) {
				case Particle::GenerateMethod::Random:
					currentMethod = methods[0];
					break;
				case Particle::GenerateMethod::Clump:
					currentMethod = methods[1];
					break;
				default:
					break;
				}
				ImGui::Text("現在の生成方法 : %s", currentMethod);
				//生成方法の選択
				ImGui::Combo("生成方法(generateMethod)", (int*)&particle_->emitter_.generateMethod, methods, IM_ARRAYSIZE(methods));
				//生成方法ごとの設定
				switch (particle_->emitter_.generateMethod) {
				case Particle::GenerateMethod::Random:
					break;
				case Particle::GenerateMethod::Clump:
					ImGui::DragInt("一塊の粒の数(clumpNum)", &particle_->emitter_.clumpNum, 1, 1, 20);
					break;
				default:
					break;
				}
				ImGui::TreePop();
			}
			//エフェクトの発生スタイル
			if (ImGui::TreeNode("スタイル")) {
				const char* styles[] = { "Loop","OneShot" };
				const char* currentStyle = "";
				switch (particle_->emitter_.effectStyle) {
				case Particle::EffectStyle::Loop:
					currentStyle = styles[0];
					break;
				case Particle::EffectStyle::OneShot:
					currentStyle = styles[1];
					break;
				default:
					break;
				}
				ImGui::Text("現在のスタイル : %s", currentStyle);
				ImGui::Combo("スタイル(effectStyle)", (int*)&particle_->emitter_.effectStyle, styles, IM_ARRAYSIZE(styles));
				ImGui::TreePop();
			}

		}

		//重力関係
		if (ImGui::CollapsingHeader("重力")) {
			ImGui::Checkbox("重力の処理をするか(isGravity)", &particle_->emitter_.isGravity);
			ImGui::DragFloat("重力値", &particle_->emitter_.gravity, 0.1f);
		}
		//床関係
		if (ImGui::CollapsingHeader("床")) {
			ImGui::Checkbox("床の処理をするか(isBound)", &particle_->emitter_.isBound);
			ImGui::DragFloat("床の反発値", &particle_->emitter_.repulsion, 0.1f);
			ImGui::DragFloat("床の高さ", &particle_->emitter_.floorHeight, 0.1f);
		}
		//ビルボードを適用するか
		if (ImGui::CollapsingHeader("ビルボード")) {
			ImGui::Checkbox("ビルボードの処理をするか(isBillboard)", &particle_->emitter_.isBillboard);
		}
		ImGui::End();
		};

	//パラメーター処理の呼び出し
	paramProcess();

	//エミッター処理の呼び出し
	emitterProcess();


#endif //_DEBUG
}
