#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <list>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

class BaseCamera;
class BulletTrail {
	//弾丸トレールマネージャーに公開
	friend class BulletTrailManager;
private://構造体
	//頂点データ
	struct VertexData {
		Vector4 position;
		Vector2 texCoord;
	};
	//弾丸トレールリソース作成用データ型
	struct BulletTrailResource {
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		VertexData* vertexData;
		Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
		D3D12_INDEX_BUFFER_VIEW indexBufferView;
		uint32_t* indexData;
	};

public://メンバ変数
	BulletTrail();
	~BulletTrail();

	//初期化
	void Initialize(const std::string& name);

private://マネージャーへの委託処理
	//更新処理
	void Update();
	//描画
	void Draw(BaseCamera* _camera);

public://setter
	//座標のセット
	void SetPosition(const Vector3& _position);
	//テクスチャ
	void SetTexture(int32_t _textureHandle) { textureHandle_ = _textureHandle; }
	//表示するか
	void SetIsDisplay(bool _isDisplay) { isDisplay_ = _isDisplay; };

private://非公開メンバ関数
	BulletTrailResource CreateBulletTrailResource();

private://メンバ変数
	//名前
	std::string name_;
	//テクスチャ
	uint32_t textureHandle_ = 0u;
	//弾丸トレールのリソース
	BulletTrailResource resource_;

	//頂点のリスト
	std::list<Vector3> vertices_;
	//最大縦幅
	const float verWidth_ = 1.0f;
	//幅の減少量
	const float widthDecayValue_ = 0.05f;

	//描画するか
	bool isDisplay_ = true;

	//最大頂点数
	const int kMaxVertexNum_
		= 128;
	int indexCount_ = 0;

};

