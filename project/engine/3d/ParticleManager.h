#pragma once
#include "BaseCamera.h"
#include "MyMath.h"
#include "BlendMode.h"
#include <d3d12.h>
#include <string>
#include <array>
#include <list>
#include <unordered_map>
#include <wrl.h>

class Particle;

/// <summary>
/// 全てのパーティクルを管理するクラス
/// シングルトンパターンで実装
/// </summary>
class ParticleManager {
private://コンストラクタ等の隠蔽
	static ParticleManager* instance;

	ParticleManager() = default;//コンストラクタ隠蔽
	~ParticleManager() = default;//デストラクタ隠蔽
	ParticleManager(ParticleManager&) = delete;//コピーコンストラクタ封印
	ParticleManager& operator=(ParticleManager&) = delete;//コピー代入演算子封印
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static ParticleManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// パーティクルをコンテナに登録
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="particle">パーティクルのポインタ</param>
	void Regist(const std::string& name, Particle* particle);
	/// <summary>
	/// 登録されたパーティクルを削除
	/// </summary>
	/// <param name="name">名前</param>
	void Delete(const std::string& name);

	/// <summary>
	/// 名前を決める関数
	/// </summary>
	/// <param name="name">名前</param>
	/// <returns>重複しない名前</returns>
	std::string GenerateName(const std::string& name);

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetCamera(BaseCamera* camera) { camera_ = camera; }


private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// グラフィックスパイプライン
	/// </summary>
	void GenerateGraphicsPipeline();
	/// <summary>
	/// コンピュートパイプライン
	/// </summary>
	void GenerateComputePipeline();

	/// <summary>
	/// CPSO(コンピュートパイプラインステートオブジェクト)の初期化
	/// </summary>
	void InitCPSOOption();
	/// <summary>
	/// CPSO(コンピュートパイプラインステートオブジェクト)の発行
	/// </summary>
	void EmitCPSOOption();
	/// <summary>
	/// CPSO(コンピュートパイプラインステートオブジェクト)の更新
	/// </summary>
	void UpdateCPSOOption();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	BaseCamera* camera_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//Gルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> gRootSignature = nullptr;
	//グラフィックスパイプライン
	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, (int)BlendMode::kMaxBlendModeNum> graphicsPipelineState;
	//Cルートシグネチャ(init,emit,update分あるので3つ)
	std::array<Microsoft::WRL::ComPtr<ID3D12RootSignature>, 3> cRootSignature;
	//コンピュートパイプライン
	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, 3> computePipelineState;

	//パーティクルのコンテナ
	std::unordered_map<std::string, Particle*> particles;

};

