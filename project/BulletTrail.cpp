#include "BulletTrail.h"
#include "BulletTrailManager.h"
#include "DirectXCommon.h"

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
	//リストが2個以下なら抜ける
	if (positions_.size() <= 2) return;

	int index = 0;

	//リストのポジションから頂点を作っていく
	for (const auto& position : positions_) {

	}


}

void BulletTrail::Draw(BaseCamera* _camera) {
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
