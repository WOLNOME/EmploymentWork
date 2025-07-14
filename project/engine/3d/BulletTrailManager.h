#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <array>
#include <unordered_map>
#include "BlendMode.h"

class BaseCamera;
class BulletTrail;
class BulletTrailManager {
private://シングルトン
	static BulletTrailManager* instance;

	BulletTrailManager() = default;//コンストラクタ隠蔽
	~BulletTrailManager() = default;//デストラクタ隠蔽
	BulletTrailManager(BulletTrailManager&) = delete;//コピーコンストラクタ封印
	BulletTrailManager& operator=(BulletTrailManager&) = delete;//コピー代入演算子封印
public://シングルトン
	static BulletTrailManager* GetInstance();
public://メンバ関数
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//終了
	void Finalize();

	//オブジェクトをコンテナに登録
	void RegisterBulletTrail(const std::string& name, BulletTrail* bulletTrail);
	//登録されたオブジェクトを削除
	void DeleteBulletTrail(const std::string& name);

	//名前を決める関数
	std::string GenerateName(const std::string& name);

public://setter
	void SetCamera(BaseCamera* _camera) { camera_ = _camera; }

private://非公開メンバ関数
	//グラフィックスパイプライン
	void GenerateGraphicsPipeline();

private://借用インスタンス
	BaseCamera* camera_ = nullptr;

private://メンバ変数
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	//グラフィックスパイプライン
	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, (int)BlendMode::kMaxBlendModeNum> graphicsPipelineState_;

	//弾丸トレールのコンテナ
	std::unordered_map<std::string, BulletTrail*> bulletTrails_;
};

