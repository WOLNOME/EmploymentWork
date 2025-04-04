#include "ParticleManager.h"
#include "DirectXCommon.h"
#include "MainRender.h"
#include "SrvManager.h"
#include "TextureManager.h"
#include "Logger.h"
#include <random>
#undef min
#undef max
#include <algorithm>

ParticleManager* ParticleManager::instance = nullptr;

ParticleManager* ParticleManager::GetInstance() {
	if (instance == nullptr) {
		instance = new ParticleManager;
	}
	return instance;
}

void ParticleManager::Initialize() {
	//グラフィックスパイプラインの設定
	GenerateGraphicsPipeline();
}

void ParticleManager::Update() {
	//各パーティクルの更新
	for (const auto& particle : particles) {
		//空いているエフェクトの中から確率で生成
		if (particle.second->emitter_.isPlay) {
			int max = particle.second->GetParam()["MaxEffects"];
			int rate = particle.second->GetParam()["EmitRate"];
			float ratePerFrame = rate * kDeltaTime;
			int genNum = 0;
			for (int i = 0; i < 60; i++) {
				//ランダム
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<float> dist(0.0f, 100.0f);
				//確率でこのフレームの生成数をインクリメント
				if (dist(gen) < ratePerFrame) {
					genNum++;
				}
			}
			//エフェクトの生成
			if (genNum > 0 && particle.second->effects_.size() + genNum < max) {
				particle.second->effects_.splice(particle.second->effects_.end(), GenerateEffect(particle.second, genNum));
			}
		}

		//インスタンスの番号
		uint32_t instanceNum = 0;
		//パーティクル内各エフェクトの更新
		for (std::list<Particle::EffectData>::iterator effectIterator = particle.second->effects_.begin(); effectIterator != particle.second->effects_.end();) {
			//各エフェクト(生きている)の寿命更新
			(*effectIterator).currentTime += kDeltaTime;
			//各エフェクトの生存チェック(寿命を迎えたら削除)
			if ((*effectIterator).currentTime > (*effectIterator).lifeTime) {
				effectIterator = particle.second->effects_.erase(effectIterator);
				continue;
			}
			//各エフェクトとエミッターとの処理
			if (particle.second->emitter_.isGravity) {
				//重力処理
				(*effectIterator).velocity.y += particle.second->emitter_.gravity * kDeltaTime;
			}
			if (particle.second->emitter_.isBound) {
				//エフェクトの足
				float leg = (*effectIterator).transform.translate.y - MyMath::Lerp((*effectIterator).startSize, (*effectIterator).endSize, (*effectIterator).currentTime);
				//床の反発処理
				if (leg > particle.second->emitter_.floorHeight && leg + (kDeltaTime * (*effectIterator).velocity.y) < particle.second->emitter_.floorHeight) {
					(*effectIterator).velocity.y *= (-1.0f) * particle.second->emitter_.repulsion;
				}
			}
			//各エフェクトとフィールドとの処理
			if (field_) {
				if (field_->isActive && particle.second->emitter_.isAffectedField) {
					if (MyMath::IsCollision(field_->area, (*effectIterator).transform.translate)) {
						(*effectIterator).velocity = (*effectIterator).velocity + (kDeltaTime * field_->acceleration);
					}
				}
			}
			//各エフェクトの速度加算
			(*effectIterator).transform.translate = (*effectIterator).transform.translate + (kDeltaTime * (*effectIterator).velocity);
			//各エフェクトの色更新
			Vector4 currentColor = MyMath::Lerp((*effectIterator).startColor, (*effectIterator).endColor, (*effectIterator).currentTime / (*effectIterator).lifeTime);
			//各エフェクトのサイズ更新
			(*effectIterator).transform.scale = Vector3(MyMath::Lerp((*effectIterator).startSize, (*effectIterator).endSize, (*effectIterator).currentTime / (*effectIterator).lifeTime), MyMath::Lerp((*effectIterator).startSize, (*effectIterator).endSize, (*effectIterator).currentTime / (*effectIterator).lifeTime), MyMath::Lerp((*effectIterator).startSize, (*effectIterator).endSize, (*effectIterator).currentTime / (*effectIterator).lifeTime));
			//座標情報からワールド行列を作成(ビルボード行列の計算もここで)
			Matrix4x4 backToFrontMatrix = MyMath::MakeRotateYMatrix(std::numbers::pi_v<float>);
			Matrix4x4 billboardMatrix = MyMath::Multiply(backToFrontMatrix, camera_->GetWorldMatrix());
			billboardMatrix.m[3][0] = 0.0f;
			billboardMatrix.m[3][1] = 0.0f;
			billboardMatrix.m[3][2] = 0.0f;
			Matrix4x4 worldMatrix = MyMath::Multiply(MyMath::Multiply(MyMath::MakeScaleMatrix((*effectIterator).transform.scale), billboardMatrix), MyMath::MakeTranslateMatrix((*effectIterator).transform.translate));
			if (!particle.second->emitter_.isBillboard) {
				worldMatrix = MyMath::MakeAffineMatrix((*effectIterator).transform.scale, (*effectIterator).transform.rotate, (*effectIterator).transform.translate);
			}
			//各エフェクトのワールド行列と色情報をパーティクルリソースに書き込む
			particle.second->particleResource_.instancingData[instanceNum].World = worldMatrix;
			particle.second->particleResource_.instancingData[instanceNum].color = currentColor;

			//次のエフェクトへ
			++effectIterator;
			//インスタンスの番号をインクリメント
			++instanceNum;
		}
		//モデル更新
		particle.second->model_->Update();
	}
}

void ParticleManager::Draw() {
	//パーティクルが一つもセットされてなかったら抜ける
	if (particles.empty()) return;

	auto mainRender = MainRender::GetInstance();
	//カメラの有無チェック
	if (!camera_) {
		assert(0 && "カメラがセットされていません。");
	}
	//ルートシグネチャをセットするコマンド
	MainRender::GetInstance()->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	//プリミティブトポロジーをセットするコマンド
	MainRender::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//カメラ情報をVSに送信(一括)
	MainRender::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, camera_->GetViewProjectionConstBuffer()->GetGPUVirtualAddress());
	//パーティクル個別の設定
	for (const auto& particle : particles) {
		//各パーティクルのブレンドモード情報からパイプラインステートを選択
		MainRender::GetInstance()->GetCommandList()->SetPipelineState(graphicsPipelineState[particle.second->GetParam()["BlendMode"]].Get());
		//各パーティクルのインスタンシングデータをVSに送信
		MainRender::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, SrvManager::GetInstance()->GetGPUDescriptorHandle(particle.second->particleResource_.srvIndex));
		//各パーティクルモデルの描画
		std::string textureName = particle.second->GetParam()["Texture"];
		int textureHandle = TextureManager::GetInstance()->LoadTexture(textureName);
		particle.second->model_->Draw(0, 3, (uint32_t)particle.second->effects_.size(), textureHandle);
	}
}

void ParticleManager::Finalize() {
	delete instance;
	instance = nullptr;
}

void ParticleManager::RegisterParticle(const std::string& name, Particle* particle) {
	//重複チェック
	if (particles.find(name) != particles.end()) {
		return;
	}
	//登録
	particles[name] = particle;
}

void ParticleManager::DeleteParticle(const std::string& name) {
	// 名前がコンテナ内に存在するかチェック
	auto it = particles.find(name);
	if (it != particles.end()) {
		particles.erase(it);  // コンテナから削除
	}
}

void ParticleManager::GenerateGraphicsPipeline() {
	HRESULT hr;
	auto dxCommon = DirectXCommon::GetInstance();

	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//DescriptorRange作成
	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;
	descriptorRangeForInstancing[0].NumDescriptors = 1;
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//RootParameter作成。複数設定できるので配列。今回は結果1つだけなので長さ1の配列
	D3D12_ROOT_PARAMETER rootParameters[4] = {};
	//マテリアルの設定
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;//レジスタ番号0とバインド
	//オブジェクト関連の設定
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//Tableを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);
	//ViewProjection関連の設定
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//PixelShaderで使う
	rootParameters[2].Descriptor.ShaderRegister = 0;//レジスタ番号0とバインド
	//テクスチャの設定
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//Tableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;//Tableの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);

	//Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//Signatureに反映
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);
	descriptionRootSignature.pParameters = rootParameters;//ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);//配列の長さ

	//シリアライズしてバイナリにする
	Microsoft::WRL::ComPtr<ID3D10Blob> signatireBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatireBlob, &errorBlob);
	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	//バイナリをもとに生成
	hr = dxCommon->GetDevice()->CreateRootSignature(0, signatireBlob->GetBufferPointer(),
		signatireBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));

	//InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	//BlendStateの設定
	std::array<D3D12_BLEND_DESC, (int)BlendMode::kMaxBlendModeNum> blendDesc{};
	for (int i = 0; i < (int)BlendMode::kMaxBlendModeNum; i++) {
		//共通部分の設定
		blendDesc[i].RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		blendDesc[i].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc[i].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc[i].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

		switch (i) {
		case (int)BlendMode::None:
			blendDesc[i].RenderTarget[0].BlendEnable = FALSE;
			break;
		case (int)BlendMode::Normal:
			blendDesc[i].RenderTarget[0].BlendEnable = TRUE;
			blendDesc[i].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc[i].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc[i].RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			break;
		case (int)BlendMode::Add:
			blendDesc[i].RenderTarget[0].BlendEnable = TRUE;
			blendDesc[i].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc[i].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc[i].RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			break;
		case (int)BlendMode::Subtract:
			blendDesc[i].RenderTarget[0].BlendEnable = TRUE;
			blendDesc[i].RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
			blendDesc[i].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			blendDesc[i].RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			break;
		case (int)BlendMode::Multiply:
			blendDesc[i].RenderTarget[0].BlendEnable = TRUE;
			blendDesc[i].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc[i].RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
			blendDesc[i].RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
			break;
		case (int)BlendMode::Screen:
			blendDesc[i].RenderTarget[0].BlendEnable = TRUE;
			blendDesc[i].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc[i].RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
			blendDesc[i].RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
			break;
		case (int)BlendMode::Execlution:
			blendDesc[i].RenderTarget[0].BlendEnable = TRUE;
			blendDesc[i].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendDesc[i].RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
			blendDesc[i].RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_COLOR;
			break;
		default:
			break;
		}
	}


	//RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏面を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	//Shaderをコンパイルする
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = dxCommon->CompileShader(L"Resources/shaders/Particle.VS.hlsl",
		L"vs_6_0");
	assert(vertexShaderBlob != nullptr);

	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = dxCommon->CompileShader(L"Resources/shaders/Particle.PS.hlsl",
		L"ps_6_0");
	assert(pixelShaderBlob != nullptr);

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	//書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	//比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	std::array<D3D12_GRAPHICS_PIPELINE_STATE_DESC, (int)BlendMode::kMaxBlendModeNum> graphicsPipelineStateDesc{};
	for (int i = 0; i < (int)BlendMode::kMaxBlendModeNum; i++) {
		graphicsPipelineStateDesc[i].pRootSignature = rootSignature.Get();
		graphicsPipelineStateDesc[i].InputLayout = inputLayoutDesc;
		graphicsPipelineStateDesc[i].VS = { vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize() };
		graphicsPipelineStateDesc[i].PS = { pixelShaderBlob->GetBufferPointer(),
		pixelShaderBlob->GetBufferSize() };
		graphicsPipelineStateDesc[i].BlendState = blendDesc[i];
		graphicsPipelineStateDesc[i].RasterizerState = rasterizerDesc;
		//書き込むRTVの情報
		graphicsPipelineStateDesc[i].NumRenderTargets = 1;
		graphicsPipelineStateDesc[i].RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		//利用するトポロジのタイプ。三角形
		graphicsPipelineStateDesc[i].PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		//どのように画面に色を打ち込むかの設定
		graphicsPipelineStateDesc[i].SampleDesc.Count = 1;
		graphicsPipelineStateDesc[i].SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		//DepthStencilの設定
		graphicsPipelineStateDesc[i].DepthStencilState = depthStencilDesc;
		graphicsPipelineStateDesc[i].DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		//実際に生成
		hr = dxCommon->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc[i],
			IID_PPV_ARGS(&graphicsPipelineState[i]));
		assert(SUCCEEDED(hr));
	}
}

std::list<Particle::EffectData> ParticleManager::GenerateEffect(Particle* particle, int genNum) {

	std::list<Particle::EffectData> effects;
	for (int i = 0; i < genNum; i++) {
		Particle::EffectData effect;
		//ランダム
		std::random_device rd;
		std::mt19937 gen(rd());
		//一様分布
		std::uniform_real_distribution<float> distTranslateX(particle->emitter_.transform.translate.x - particle->emitter_.transform.scale.x, particle->emitter_.transform.translate.x + particle->emitter_.transform.scale.x);
		std::uniform_real_distribution<float> distTranslateY(particle->emitter_.transform.translate.y - particle->emitter_.transform.scale.y, particle->emitter_.transform.translate.y + particle->emitter_.transform.scale.y);
		std::uniform_real_distribution<float> distTranslateZ(particle->emitter_.transform.translate.z - particle->emitter_.transform.scale.z, particle->emitter_.transform.translate.z + particle->emitter_.transform.scale.z);
		std::uniform_real_distribution<float> distStartSize(particle->GetParam()["StartSize"]["Min"], particle->GetParam()["StartSize"]["Max"]);
		std::uniform_real_distribution<float> distEndSize(particle->GetParam()["EndSize"]["Min"], particle->GetParam()["EndSize"]["Max"]);
		Vector4 startColorMin = {
			particle->GetParam()["StartColor"]["Min"]["x"],
			particle->GetParam()["StartColor"]["Min"]["y"],
			particle->GetParam()["StartColor"]["Min"]["z"],
			particle->GetParam()["StartColor"]["Min"]["w"]
		};
		Vector4 startColorMax = {
			particle->GetParam()["StartColor"]["Max"]["x"],
			particle->GetParam()["StartColor"]["Max"]["y"],
			particle->GetParam()["StartColor"]["Max"]["z"],
			particle->GetParam()["StartColor"]["Max"]["w"]
		};
		std::uniform_real_distribution<float> distStartColorX(std::min(startColorMin.x, startColorMax.x), std::max(startColorMin.x, startColorMax.x));
		std::uniform_real_distribution<float> distStartColorY(std::min(startColorMin.y, startColorMax.y), std::max(startColorMin.y, startColorMax.y));
		std::uniform_real_distribution<float> distStartColorZ(std::min(startColorMin.z, startColorMax.z), std::max(startColorMin.z, startColorMax.z));
		std::uniform_real_distribution<float> distStartColorW(std::min(startColorMin.w, startColorMax.w), std::max(startColorMin.w, startColorMax.w));
		Vector4 endColorMin = {
			particle->GetParam()["EndColor"]["Min"]["x"] ,
			particle->GetParam()["EndColor"]["Min"]["y"] ,
			particle->GetParam()["EndColor"]["Min"]["z"] ,
			particle->GetParam()["EndColor"]["Min"]["w"]
		};
		Vector4 endColorMax = {
			particle->GetParam()["EndColor"]["Max"]["x"] ,
			particle->GetParam()["EndColor"]["Max"]["y"] ,
			particle->GetParam()["EndColor"]["Max"]["z"] ,
			particle->GetParam()["EndColor"]["Max"]["w"]
		};
		std::uniform_real_distribution<float> distEndColorX(std::min(endColorMin.x, endColorMax.x), std::max(endColorMin.x, endColorMax.x));
		std::uniform_real_distribution<float> distEndColorY(std::min(endColorMin.y, endColorMax.y), std::max(endColorMin.y, endColorMax.y));
		std::uniform_real_distribution<float> distEndColorZ(std::min(endColorMin.z, endColorMax.z), std::max(endColorMin.z, endColorMax.z));
		std::uniform_real_distribution<float> distEndColorW(std::min(endColorMin.w, endColorMax.w), std::max(endColorMin.w, endColorMax.w));
		Vector3 velocityMax = {
			particle->GetParam()["Velocity"]["Max"]["x"],
			particle->GetParam()["Velocity"]["Max"]["y"],
			particle->GetParam()["Velocity"]["Max"]["z"],
		};
		Vector3 velocityMin = {
			particle->GetParam()["Velocity"]["Min"]["x"],
			particle->GetParam()["Velocity"]["Min"]["y"],
			particle->GetParam()["Velocity"]["Min"]["z"],
		};
		std::uniform_real_distribution<float> distVelocityX(std::min(velocityMin.x, velocityMax.x), std::max(velocityMin.x, velocityMax.x));
		std::uniform_real_distribution<float> distVelocityY(std::min(velocityMin.y, velocityMax.y), std::max(velocityMin.y, velocityMax.y));
		std::uniform_real_distribution<float> distVelocityZ(std::min(velocityMin.z, velocityMax.z), std::max(velocityMin.z, velocityMax.z));
		std::uniform_real_distribution<float> distLifeTime(particle->GetParam()["LifeTime"]["Min"], particle->GetParam()["LifeTime"]["Max"]);
		//パラメータの初期化
		effect.transform.translate = Vector3(distTranslateX(gen), distTranslateY(gen), distTranslateZ(gen));
		effect.transform.rotate = Vector3(0.0f, 0.0f, 0.0f);
		effect.startSize = distStartSize(gen);
		effect.endSize = distEndSize(gen);
		effect.transform.scale = Vector3(effect.startSize, effect.startSize, effect.startSize);
		effect.startColor = Vector4(distStartColorX(gen), distStartColorY(gen), distStartColorZ(gen), distStartColorW(gen));
		effect.endColor = Vector4(distEndColorX(gen), distEndColorY(gen), distEndColorZ(gen), distEndColorW(gen));
		effect.velocity = Vector3(distVelocityX(gen), distVelocityY(gen), distVelocityZ(gen));
		effect.lifeTime = distLifeTime(gen);
		effect.currentTime = 0.0f;
		//プッシュバック
		effects.push_back(effect);
	}
	return effects;
}
