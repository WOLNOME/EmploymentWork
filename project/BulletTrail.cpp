#include "BulletTrail.h"
#include "BulletTrailManager.h"
#include "DirectXCommon.h"
#include "MainRender.h"
#include "GPUDescriptorManager.h"
#include "BaseCamera.h"

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
	//頂点数が0なら抜ける
	if (vertices_.empty()) return;

	//頂点を狭める処理
	for (auto it = vertices_.begin(); it != vertices_.end(); ) {
		auto upper = it;
		auto lower = std::next(it);

		if (lower == vertices_.end()) break;

		// 上下を狭める
		upper->y += widthDecayValue_;
		lower->y -= widthDecayValue_;

		// もし上下が逆転したら削除
		if (upper->y <= lower->y) {
			it = vertices_.erase(it);     // upperを削除
			if (it != vertices_.end()) {
				it = vertices_.erase(it); // lowerを削除
			}

		}
		else {
			std::advance(it, 2); // 次のペアへ
		}
	}

	//頂点データの移送
	int index = 0;
	for (const auto& vertex : vertices_) {
		resource_.vertexData[index].position = Vector4(vertex.x, vertex.y, vertex.z, 1.0f);
		resource_.vertexData[index].texCoord = Vector2(0.0f, 0.0f);
		index++;
	}

	//インデックスデータの構築
	int quadCount = vertices_.size() / 2 - 1;
	indexCount_ = quadCount * 6;
	for (int i = 0; i < quadCount; i++) {
		int vi = i * 2;
		resource_.indexData[i * 6 + 0] = vi;
		resource_.indexData[i * 6 + 1] = vi + 2;
		resource_.indexData[i * 6 + 2] = vi + 1;

		resource_.indexData[i * 6 + 3] = vi + 1;
		resource_.indexData[i * 6 + 4] = vi + 2;
		resource_.indexData[i * 6 + 5] = vi + 3;
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

void BulletTrail::SetPosition(const Vector3& _position) {
	//頂点の生成
	Vector3 ver1 = _position, ver2 = _position;
	ver1.y += verWidth_ / 2.0f;
	ver2.y -= verWidth_ / 2.0f;
	vertices_.push_back(ver1);
	vertices_.push_back(ver2);
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
