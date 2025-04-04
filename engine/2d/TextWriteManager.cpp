#include "TextWriteManager.h"
#include "TextWrite.h"
#include <filesystem>
#include <cassert>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwrite.lib")

namespace fs = std::filesystem;

TextWriteManager* TextWriteManager::instance = nullptr;

TextWriteManager* TextWriteManager::GetInstance() {
	if (instance == nullptr) {
		instance = new TextWriteManager;
	}
	return instance;
}

void TextWriteManager::Initialize() {
	//DWriteFactoryの生成
	CreateIDWriteFactory();
	//D3D11On12Deviceの生成
	CreateD3D11On12Device();
	//D2DDeviceContextの生成
	CreateDirect2DDeviceContext();
	//D2DRenderTargetの生成
	CreateD2DRenderTarget();

	//フォントファイルの生成
	CreateFontFile();
}

void TextWriteManager::Finalize() {
	delete instance;
	instance = nullptr;
}

void TextWriteManager::Registration(TextWrite* piece) {
	std::string key = piece->GetName();

	//コンテナに同じ名前が登録されていたらエラー
	if (textWriteMap.find(key) != textWriteMap.end()) {
		assert(0 && "TextWriteで既に同じ名前が使われています。");
	}

	//コンテナに登録
	textWriteMap[key] = piece;
}

void TextWriteManager::CancelRegistration(const std::string& key) {
	// 指定したキーが存在するか確認
	auto it = textWriteMap.find(key);
	if (it != textWriteMap.end()) {
		// キーと関連する情報を削除
		textWriteMap.erase(it);
	}
	else {
		// キーが見つからなかった場合の処理
		return;
	}

	//ブラシとフォントのコンテナの情報も削除
	if (solidColorBrushMap.find(key) != solidColorBrushMap.end()) {
		// キーと関連する情報を削除
		solidColorBrushMap.erase(solidColorBrushMap.find(key));
	}
	else {
		// キーが見つからなかった場合の処理
		assert(0 && "発生するはずのないエラーです。システムに問題がある可能性が高いです。");
		return;
	}
	if (textFormatMap.find(key) != textFormatMap.end()) {
		// キーと関連する情報を削除
		textFormatMap.erase(textFormatMap.find(key));
	}
	else {
		// キーが見つからなかった場合の処理
		assert(0 && "発生するはずのないエラーです。システムに問題がある可能性が高いです。");
		return;
	}

}

std::string TextWriteManager::GenerateFontKey(const std::wstring& fontName, const FontStyle& style) {
	std::string key(fontName.begin(), fontName.end()); // wstring → string 変換

	switch (style) {
	case FontStyle::Normal:  key += "_Normal"; break;
	case FontStyle::Oblique: key += "_Oblique"; break;
	case FontStyle::Italic:  key += "_Italic"; break;
	}
	return key;
}

void TextWriteManager::CreateIDWriteFactory() {
	HRESULT hr;
	//IDWriteFactoryの生成
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &directWriteFactory);
	assert(SUCCEEDED(hr));
}

void TextWriteManager::CreateD3D11On12Device() {
	HRESULT hr;
	//ID3D11On12Deviceの生成
	ComPtr<ID3D11Device
	> d3d11Device = nullptr;
	UINT d3d11DeviceFlags = 0U;
#ifdef _DEBUG
	d3d11DeviceFlags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#else
	d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif // _DEBUG
	hr = D3D11On12CreateDevice(
		dxcommon->GetDevice(),
		d3d11DeviceFlags,
		nullptr,
		0,
		reinterpret_cast<IUnknown**>(dxcommon->GetAddressOfCommandQueue()),
		1,
		0,
		&d3d11Device,
		&d3d11On12DeviceContext,
		nullptr
	);
	assert(SUCCEEDED(hr));
	//D3D11->D3D11On12
	hr = d3d11Device.As(&d3d11On12Device);
	assert(SUCCEEDED(hr));
}

void TextWriteManager::CreateDirect2DDeviceContext() {
	HRESULT hr;
	//ID2D1Factory3の生成
	D2D1_FACTORY_OPTIONS factoryOptions{};
	factoryOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &factoryOptions, &d2dFactory);
	assert(SUCCEEDED(hr));
	//IDXGIDeviceの生成
	ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	hr = d3d11On12Device.As(&dxgiDevice);
	assert(SUCCEEDED(hr));
	//ID2D1Device2の生成
	ComPtr<ID2D1Device2> d2dDevice = nullptr;
	hr = d2dFactory->CreateDevice(dxgiDevice.Get(), d2dDevice.ReleaseAndGetAddressOf());
	//d2dDeviceContextの生成
	hr = d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, d2dDeviceContext.ReleaseAndGetAddressOf());
	assert(SUCCEEDED(hr));
}

void TextWriteManager::CreateD2DRenderTarget() {
	HRESULT hr;
	//DirectWriteの描画先の生成
	D3D11_RESOURCE_FLAGS resourceFlags = { D3D11_BIND_RENDER_TARGET };
	const UINT dpi = GetDpiForWindow(winapp->GetHwnd());
	D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), static_cast<float>(dpi), static_cast<float>(dpi));

	for (UINT i = 0U; i < mainrender->GetBackBufferCount(); ++i) {
		ComPtr<ID3D11Resource> wrappedBackBuffer = nullptr;
		//ID3D11Resourceの生成
		hr = d3d11On12Device->CreateWrappedResource(mainrender->GetSwapChainResource(i), &resourceFlags, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT, IID_PPV_ARGS(wrappedBackBuffer.ReleaseAndGetAddressOf()));
		assert(SUCCEEDED(hr));
		//IDXGISurfaceの生成
		ComPtr<IDXGISurface> dxgiSurface = nullptr;
		hr = wrappedBackBuffer.As(&dxgiSurface);
		assert(SUCCEEDED(hr));
		//ID2D1Bitmap1の生成
		ComPtr<ID2D1Bitmap1> d2dRenderTarget = nullptr;
		hr = d2dDeviceContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), &bitmapProperties, &d2dRenderTarget);
		assert(SUCCEEDED(hr));

		wrappedBackBuffers.emplace_back(wrappedBackBuffer);
		d2dRenderTargets.emplace_back(d2dRenderTarget);
	}
}

void TextWriteManager::CreateFontFile() {
	HRESULT hr;
	// IDWriteFontSetBuilder2 の生成
	ComPtr<IDWriteFontSetBuilder2> dwriteFontSetBuilder = nullptr;
	hr = directWriteFactory->CreateFontSetBuilder(&dwriteFontSetBuilder);
	assert(SUCCEEDED(hr));
	// フォントファイルとフォントフェイスを作る
	std::vector<ComPtr<IDWriteFontFile>> fontFiles;
	std::wstring fontDirectory = L"Resources/fonts"; // フォントフォルダのパス

	// fontsフォルダ内の .ttf 及び .ttc ファイルを探索
	for (const auto& entry : fs::directory_iterator(fontDirectory)) {
		if (entry.is_regular_file() && (entry.path().extension() == L".ttf" || entry.path().extension() == L".ttc")) {
			// IDWriteFontFile の生成
			ComPtr<IDWriteFontFile> dwriteFontFile;
			hr = directWriteFactory->CreateFontFileReference(entry.path().c_str(), nullptr, &dwriteFontFile);
			if (FAILED(hr)) continue;
			// vectorに保存
			fontFiles.push_back(dwriteFontFile);
			// フォントセットビルダーに追加
			hr = dwriteFontSetBuilder->AddFontFile(dwriteFontFile.Get());
			assert(SUCCEEDED(hr));

			//フォントファイルの種類を取得
			BOOL isSupported;
			DWRITE_FONT_FILE_TYPE fileType;
			DWRITE_FONT_FACE_TYPE faceType;
			UINT32 numFaces;
			hr = dwriteFontFile->Analyze(&isSupported, &fileType, &faceType, &numFaces);
			if (FAILED(hr) || !isSupported) continue;

			// IDWriteFontFace の作成 (ttcの場合はフォントごとに作る)
			for (UINT32 i = 0; i < numFaces; i++) {
				//IDWriteFontReferenceの作成
				ComPtr<IDWriteFontFaceReference> dwriteFontFaceRef;
				hr = directWriteFactory->CreateFontFaceReference(
					dwriteFontFile.Get(), i, DWRITE_FONT_SIMULATIONS_NONE, &dwriteFontFaceRef
				);
				if (FAILED(hr)) continue;
				//IDWriteFontFaceの作成
				ComPtr<IDWriteFontFace3> dwriteFontFace;
				hr = dwriteFontFaceRef->CreateFontFace(dwriteFontFace.ReleaseAndGetAddressOf());
				if (FAILED(hr)) continue;
				//フォントファミリー名を取得
				ComPtr<IDWriteLocalizedStrings> fontNames;
				hr = dwriteFontFace->GetFamilyNames(&fontNames);
				if (FAILED(hr)) continue;
				// 最初のフォント名を取得
				UINT32 length = 0;
				hr = fontNames->GetStringLength(0, &length);
				if (FAILED(hr)) continue;
				std::vector<wchar_t> nameBuffer(length + 1);
				hr = fontNames->GetString(0, nameBuffer.data(), length + 1);
				if (FAILED(hr)) continue;
				std::wstring fontName = nameBuffer.data();

				// フォントのスタイルを取得
				DWRITE_FONT_STYLE fontStyle = dwriteFontFace->GetStyle();
				FontStyle style = static_cast<FontStyle>(fontStyle);
				std::string fontKey = GenerateFontKey(fontName, style);

				// unordered_map に格納
				fontFaceMap[fontKey] = dwriteFontFace;
			}
		}
	}

	// IDWriteFontSet の生成
	ComPtr<IDWriteFontSet> dwriteFontSet = nullptr;
	hr = dwriteFontSetBuilder->CreateFontSet(&dwriteFontSet);
	assert(SUCCEEDED(hr));

	// フォントコレクションの生成
	hr = directWriteFactory->CreateFontCollectionFromFontSet(dwriteFontSet.Get(), &dwriteFontCollection);
	assert(SUCCEEDED(hr));
}

void TextWriteManager::EditSolidColorBrash(const std::string& key, const Vector4& color) noexcept {
	HRESULT hr;
	//色と透明度を分離
	D2D1::ColorF rgb(color.x, color.y, color.z);
	FLOAT alpha = static_cast<FLOAT>(color.w);
	//ブラシを作って登録(すでに作っていたら編集)
	ComPtr<ID2D1SolidColorBrush> brush = nullptr;
	hr = d2dDeviceContext->CreateSolidColorBrush(rgb, &brush);
	assert(SUCCEEDED(hr));
	brush->SetOpacity(alpha);
	solidColorBrushMap[key] = brush;
}

void TextWriteManager::EditTextFormat(const std::string& key, const std::wstring& fontName, const float fontSize) noexcept {
	HRESULT hr;
	//該当キーのテキストライトマップが存在するか確認
	if (!textWriteMap[key]) {
		assert(0 && "入力されたキーのTextWriteMapがありませんでした。初期化してください。");
	}
	//テキストフォーマットを作って登録(すでに作ってあったら編集)
	ComPtr<IDWriteTextFormat> textFormat = nullptr;
	DWRITE_FONT_STYLE style = static_cast<DWRITE_FONT_STYLE>(textWriteMap[key]->GetFontStyle());
	hr = directWriteFactory->CreateTextFormat(
		fontName.c_str(),
		dwriteFontCollection.Get(),
		DWRITE_FONT_WEIGHT_NORMAL,
		style,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"ja-jp",
		&textFormat
	);
	assert(SUCCEEDED(hr));

	textFormatMap[key] = textFormat;
}

void TextWriteManager::BeginDrawWithD2D() const noexcept {
	const auto backBufferIndex = mainrender->GetSwapChain()->GetCurrentBackBufferIndex();
	const auto wrappedBackBuffer = wrappedBackBuffers[backBufferIndex];
	const auto backBufferForD2D = d2dRenderTargets[backBufferIndex];

	d3d11On12Device->AcquireWrappedResources(wrappedBackBuffer.GetAddressOf(), 1);
	d2dDeviceContext->SetTarget(backBufferForD2D.Get());
	d2dDeviceContext->BeginDraw();
	d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
}

void TextWriteManager::WriteText(const std::string& key) {
	const auto textFormat = textFormatMap.at(key);
	const auto solidColorBrush = solidColorBrushMap[key];
	const auto textWrite = textWriteMap[key];

	//アウトライン描画
	if (textWriteMap[key]->GetIsEdgeDisplay()) {
		DrawOutline(key);
	}
	//描画範囲
	D2D1_RECT_F rect;
	rect = {
		textWrite->GetPosition().x,
		textWrite->GetPosition().y,
		textWrite->GetPosition().x + textWrite->GetWidth(),
		textWrite->GetPosition().y + textWrite->GetHeight()
	};
	//テキスト描画処理
	d2dDeviceContext->SetTransform(
		D2D1::Matrix3x2F::Identity()
	);
	d2dDeviceContext->DrawTextW(
		textWrite->GetText().c_str(),
		static_cast<UINT32>(textWrite->GetText().length()),
		textFormat.Get(),
		&rect,
		solidColorBrush.Get()
	);
}

void TextWriteManager::EndDrawWithD2D() const noexcept {
	const auto backBufferIndex = mainrender->GetSwapChain()->GetCurrentBackBufferIndex();
	const auto wrappedBackBuffer = wrappedBackBuffers[backBufferIndex];

	d2dDeviceContext->EndDraw();
	d3d11On12Device->ReleaseWrappedResources(wrappedBackBuffer.GetAddressOf(), 1);
	//描画内容の確定(スワップ可能状態に移行)
	d3d11On12DeviceContext->Flush();
}

void TextWriteManager::DrawOutline(const std::string& key) {
	HRESULT hr;
	//グリフインデックス列を取得する
	std::vector<UINT> codePoints;
	auto glyphIndices = new UINT16[textWriteMap[key]->GetText().length()];
	ZeroMemory(glyphIndices, sizeof(UINT16) * textWriteMap[key]->GetText().length());
	for (auto character : textWriteMap[key]->GetText()) {
		codePoints.emplace_back(character);
	}
	const auto fontFace = fontFaceMap[textWriteMap[key]->GetFontFaceKey()];
	hr = fontFace->GetGlyphIndicesW(codePoints.data(), static_cast<UINT32>(codePoints.size()), glyphIndices);
	if (FAILED(hr)) return;
	ComPtr<ID2D1PathGeometry> pathGeometry = nullptr;
	hr = d2dFactory->CreatePathGeometry(pathGeometry.ReleaseAndGetAddressOf());
	if (FAILED(hr)) return;
	ComPtr<ID2D1GeometrySink> geometrySink = nullptr;
	hr = pathGeometry->Open(geometrySink.ReleaseAndGetAddressOf());
	if (FAILED(hr)) return;
	//アウトライン情報を取得する
	hr = fontFace->GetGlyphRunOutline(textWriteMap[key]->GetSize(), glyphIndices, nullptr, nullptr, static_cast<UINT32>(textWriteMap[key]->GetText().length()), FALSE, FALSE, geometrySink.Get());
	if (FAILED(hr)) return;
	hr = geometrySink->Close();
	assert(SUCCEEDED(hr));
	codePoints.clear();
	delete[] glyphIndices;
	std::pair<ComPtr<ID2D1PathGeometry>, ComPtr<ID2D1GeometrySink>> geometryInfo;
	geometryInfo = { pathGeometry, geometrySink };

	const auto textPathGeometry = geometryInfo.first;
	const auto solidColorBrushOfEdge = solidColorBrushMap[textWriteMap[key]->GetEdgeName()];
	Vector2 position = textWriteMap[key]->GetPosition();
	Vector2 slideRate = textWriteMap[key]->GetEdgeSlideRate();
	float strokeWidth = textWriteMap[key]->GetEdgeStrokeWidth();
	if (slideRate.x != 0.0f || slideRate.y != 0.0f) {
		D2D1_RECT_F rect{};
		hr = textPathGeometry->GetBounds(nullptr, &rect);
		assert(SUCCEEDED(hr));
		position.x += (rect.right - rect.left) * slideRate.x;
		position.y += (rect.right - rect.left) * slideRate.y;
	}

	//文字サイズに合わせて位置調整(フォントごと)
	switch (textWriteMap[key]->GetFont()) {
	case Font::Meiryo:
		position.y += textWriteMap[key]->GetSize() * (17.0f / 16.0f);
		break;
	case Font::YuGothic:
		position.y += textWriteMap[key]->GetSize() * (21.0f / 16.0f);
		break;
	case Font::YuMincho:
		position.y += textWriteMap[key]->GetSize() * (21.0f / 16.0f);
		break;
	case Font::UDDegitalN_B:
		position.y += textWriteMap[key]->GetSize() * (14.0f / 16.0f);
		break;
	case Font::UDDegitalN_R:
		position.y += textWriteMap[key]->GetSize() * (14.0f / 16.0f);
		break;
	case Font::UDDegitalNK_B:
		position.y += textWriteMap[key]->GetSize() * (14.0f / 16.0f);
		break;
	case Font::UDDegitalNK_R:
		position.y += textWriteMap[key]->GetSize() * (14.0f / 16.0f);
		break;
	case Font::UDDegitalNP_B:
		position.y += textWriteMap[key]->GetSize() * (14.0f / 16.0f);
		break;
	case Font::UDDegitalNP_R:
		position.y += textWriteMap[key]->GetSize() * (14.0f / 16.0f);
		break;
	case Font::OnionScript:
		position.y += textWriteMap[key]->GetSize() * (15.0f / 16.0f);
		break;
	default:
		assert(0 && "不明なフォントです。ポジションの計算をしてください。");
		break;
	}

	//描画
	d2dDeviceContext->SetTransform(
		D2D1::Matrix3x2F::Translation(position.x, position.y)
	);
	d2dDeviceContext->DrawGeometry(
		textPathGeometry.Get(),
		solidColorBrushOfEdge.Get(),
		strokeWidth
	);
}
