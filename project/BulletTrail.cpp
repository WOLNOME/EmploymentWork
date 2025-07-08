#include "BulletTrail.h"
#include "BulletTrailManager.h"
#include "DirectXCommon.h"
#include "MainRender.h"
#include "GPUDescriptorManager.h"
#include "BaseCamera.h"
#include <algorithm>

#undef min
#undef max

BulletTrail::BulletTrail() {
}

BulletTrail::~BulletTrail() {
	//マネージャーから削除
	BulletTrailManager::GetInstance()->DeleteBulletTrail(name_);
}

void BulletTrail::Initialize(const std::string& name) {
	//名前
	name_ = name;
	//リソース作成
	resource_ = CreateBulletTrailResource();
	//マネージャーに登録
	BulletTrailManager::GetInstance()->RegisterBulletTrail(name_, this);
}

void BulletTrail::Update() {
	//頂点データとインデックスデータをクリア
	memset(resource_.vertexData, 0, sizeof(VertexData) * kMaxVertexNum_);
	memset(resource_.indexData, 0, sizeof(uint32_t) * kMaxVertexNum_ * 3);
	indexCount_ = 0;
	//入力座標数が1以下なら抜ける
	if (positions_.size() <= 1) return;

	//頂点の生成処理
	std::list<Vector3> vertices;
	int posCount = 0;
	for (auto it = positions_.begin(); it != positions_.end(); ) {
		Vector3 direction = {};
		if (it == positions_.begin()) {
			direction = Vector3(*std::next(it) - *it).Normalized();
		}
		else {
			direction = Vector3(*it - *std::prev(it)).Normalized();
		}

		float verLength = 1.0f - (positions_.size() - (posCount + 1)) * widthDecayValue_;
		if (verLength > 0.0f) {
			Vector3 worldUp = { 0.0f, 1.0f, 0.0f };
			Vector3 right = MyMath::Cross(worldUp, direction).Normalized();
			Vector3 up = MyMath::Cross(direction, right).Normalized();

			Vector3 pos = *it;
			vertices.push_back(pos + right * verLength + up * verLength);
			vertices.push_back(pos - right * verLength + up * verLength);
			vertices.push_back(pos + right * verLength - up * verLength);
			vertices.push_back(pos - right * verLength - up * verLength);

			++it;
			++posCount;
		}
		else {
			it = positions_.erase(it);
		}
	}


	//頂点データの移送
	int index = 0;
	for (const auto& vertex : vertices) {
		resource_.vertexData[index].position = Vector4(vertex.x, vertex.y, vertex.z, 1.0f);
		resource_.vertexData[index].texCoord = Vector2(0.0f, 0.0f);
		index++;
	}

	//インデックスデータの構築
	int sectionCount = positions_.size() - 1;
	indexCount_ = sectionCount * 6 * 4 + 6 * 2;
	int idx = 0;
	for (int i = 0; i < sectionCount; ++i) {
		int vi = i * 4;

		// 上面
		resource_.indexData[idx++] = vi;
		resource_.indexData[idx++] = vi + 4;
		resource_.indexData[idx++] = vi + 1;

		resource_.indexData[idx++] = vi + 1;
		resource_.indexData[idx++] = vi + 4;
		resource_.indexData[idx++] = vi + 5;

		// 右面



		// 下面
		resource_.indexData[idx++] = vi + 2;
		resource_.indexData[idx++] = vi + 3;
		resource_.indexData[idx++] = vi + 6;

		resource_.indexData[idx++] = vi + 3;
		resource_.indexData[idx++] = vi + 7;
		resource_.indexData[idx++] = vi + 6;

		// 左面



	}

}

void BulletTrail::Draw(BaseCamera* _camera) {
	//頂点バッファビューを送信
	MainRender::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &resource_.vertexBufferView);
	//インデックスバッファビューを送信
	MainRender::GetInstance()->GetCommandList()->IASetIndexBuffer(&resource_.indexBufferView);
	//Cameraの情報をCBufferへ送信
	MainRender::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, _camera->GetViewProjectionConstBuffer()->GetGPUVirtualAddress());
	//テクスチャ情報をCBufferへ送信
	MainRender::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, GPUDescriptorManager::GetInstance()->GetGPUDescriptorHandle(textureHandle_));

	//ドローコール
	MainRender::GetInstance()->GetCommandList()->DrawIndexedInstanced(indexCount_, 1, 0, 0, 0);

}

BulletTrail::BulletTrailResource BulletTrail::CreateBulletTrailResource() {
	BulletTrailResource result;
	//リソースの作成
	result.vertexResource = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(VertexData) * kMaxVertexNum_);
	result.indexResource = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(uint32_t) * kMaxVertexNum_ * 3);
	//VBVの作成
	result.vertexBufferView.BufferLocation = result.vertexResource->GetGPUVirtualAddress();
	result.vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * kMaxVertexNum_);
	result.vertexBufferView.StrideInBytes = sizeof(VertexData);
	//IBVの作成
	result.indexBufferView.BufferLocation = result.indexResource->GetGPUVirtualAddress();
	result.indexBufferView.SizeInBytes = UINT(sizeof(uint32_t) * kMaxVertexNum_ * 3);
	result.indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	//リソースとデータを同期させる
	result.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&result.vertexData));
	result.indexResource->Map(0, nullptr, reinterpret_cast<void**>(&result.indexData));

	return result;
}
