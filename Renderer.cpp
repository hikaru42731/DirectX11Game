#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#include "Main.h"
#include "Renderer.h"
#include <d2d1.h>/
#include <dwrite.h>
#include <io.h>


D3D_FEATURE_LEVEL       Renderer::featureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*           Renderer::renderDevice = NULL;
ID3D11DeviceContext*    Renderer::renderDeviceContext = NULL;
IDXGISwapChain*         Renderer::renderSwapChain = NULL;
ID3D11RenderTargetView* Renderer::renderTargetView = NULL;
ID3D11DepthStencilView* Renderer::renderDepthStencilView = NULL;

ID3D11Buffer*			Renderer::worldBuffer = NULL;
ID3D11Buffer*			Renderer::viewBuffer = NULL;
ID3D11Buffer*			Renderer::projectionBuffer = NULL;
ID3D11Buffer*			Renderer::materialBuffer = NULL;
ID3D11Buffer*			Renderer::lightBuffer = NULL;


ID3D11DepthStencilState* Renderer::depthStateEnable = NULL;
ID3D11DepthStencilState* Renderer::depthStateDisable = NULL;



void Renderer::Init()
{
	HRESULT hr = S_OK;

	// Directxのこと  バッファの切り替えを行う
	// デバイス、スワップチェーン作成
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;
	swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = GetWindow();
	swapChainDesc.SampleDesc.Count = 8;	//ハードウェアによって最大値が異なる
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&renderSwapChain,
		&renderDevice,
		&featureLevel,
		&renderDeviceContext);


	// レンダーターゲットビュー作成
	ID3D11Texture2D* renderTarget = NULL;
	renderSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
	renderDevice->CreateRenderTargetView(renderTarget, NULL, &renderTargetView);
	renderTarget->Release();


	// デプスステンシルバッファ作成　Zバッファと呼ばれるもの
	ID3D11Texture2D* depthStencile = NULL;
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = swapChainDesc.BufferDesc.Width;
	textureDesc.Height = swapChainDesc.BufferDesc.Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D16_UNORM;
	textureDesc.SampleDesc = swapChainDesc.SampleDesc;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	renderDevice->CreateTexture2D(&textureDesc, NULL, &depthStencile);

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;	//MSを付け加えるだけ
	depthStencilViewDesc.Flags = 0;
	renderDevice->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &renderDepthStencilView);
	depthStencile->Release();


	renderDeviceContext->OMSetRenderTargets(1, &renderTargetView, renderDepthStencilView);





	// ビューポート設定 画面のどこに描画するか　画面分割とかに使える
	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)SCREEN_WIDTH;
	viewport.Height = (FLOAT)SCREEN_HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	renderDeviceContext->RSSetViewports(1, &viewport);



	// ラスタライザステート設定  描画の全体設定
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;

	ID3D11RasterizerState* rs;
	renderDevice->CreateRasterizerState(&rasterizerDesc, &rs);

	renderDeviceContext->RSSetState(rs);




	// ブレンドステート設定　αブレンドの設定　加算合成とかの
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = NULL;
	renderDevice->CreateBlendState(&blendDesc, &blendState);
	renderDeviceContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);



	// デプスステンシルステート設定　Zバッファの設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	renderDevice->CreateDepthStencilState(&depthStencilDesc, &depthStateEnable);//深度有効ステート

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	renderDevice->CreateDepthStencilState(&depthStencilDesc, &depthStateDisable);//深度無効ステート

	renderDeviceContext->OMSetDepthStencilState(depthStateEnable, NULL);


	// サンプラーステート設定　テクスチャをどう貼り付けるかの設定
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;//みっぷマップ
	samplerDesc.MaxAnisotropy = 16;	//ぼやけすぎ防止

	ID3D11SamplerState* samplerState = NULL;
	renderDevice->CreateSamplerState(&samplerDesc, &samplerState);

	renderDeviceContext->PSSetSamplers(0, 1, &samplerState);
	

	// 定数バッファ生成　シェーダーに定数を送る
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(D3DXMATRIX);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	renderDevice->CreateBuffer(&bufferDesc, NULL, &worldBuffer);
	renderDeviceContext->VSSetConstantBuffers(0, 1, &worldBuffer);

	renderDevice->CreateBuffer(&bufferDesc, NULL, &viewBuffer);
	renderDeviceContext->VSSetConstantBuffers(1, 1, &viewBuffer);

	renderDevice->CreateBuffer(&bufferDesc, NULL, &projectionBuffer);
	renderDeviceContext->VSSetConstantBuffers(2, 1, &projectionBuffer);


	bufferDesc.ByteWidth = sizeof(MATERIAL);

	renderDevice->CreateBuffer(&bufferDesc, NULL, &materialBuffer);
	renderDeviceContext->VSSetConstantBuffers(3, 1, &materialBuffer);


	bufferDesc.ByteWidth = sizeof(LIGHT);

	renderDevice->CreateBuffer(&bufferDesc, NULL, &lightBuffer);
	renderDeviceContext->VSSetConstantBuffers(4, 1, &lightBuffer);
	renderDeviceContext->PSSetConstantBuffers(4, 1, &lightBuffer);




	// ライト初期化
	LIGHT light{};
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f); //環境光
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); //直接光
	SetLight(light);



	// マテリアル初期化
	MATERIAL material{};
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);
}



void Renderer::Uninit()
{
	worldBuffer->Release();
	viewBuffer->Release();
	projectionBuffer->Release();
	lightBuffer->Release();
	materialBuffer->Release();


	renderDeviceContext->ClearState();
	renderTargetView->Release();
	renderSwapChain->Release();
	renderDeviceContext->Release();
	renderDevice->Release();
}




void Renderer::Begin()
{
	// カラー指定
	float clearColor[4] = { 0.4f, 0.9f, 1.0f, 1.0f };
	renderDeviceContext->ClearRenderTargetView( renderTargetView, clearColor );
	renderDeviceContext->ClearDepthStencilView( renderDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}



void Renderer::End()
{
	renderSwapChain->Present( 1, 0 );
}




void Renderer::SetDepthEnable( bool Enable )
{
	if( Enable )
		renderDeviceContext->OMSetDepthStencilState( depthStateEnable, NULL );
	else
		renderDeviceContext->OMSetDepthStencilState( depthStateDisable, NULL );

}

void Renderer::SetWorldViewProjection2D()
{
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranspose(&world, &world);

	renderDeviceContext->UpdateSubresource(worldBuffer, 0, NULL, &world, 0, 0);

	D3DXMATRIX view;
	D3DXMatrixIdentity(&view);
	D3DXMatrixTranspose(&view, &view);
	renderDeviceContext->UpdateSubresource(viewBuffer, 0, NULL, &view, 0, 0);

	D3DXMATRIX projection;
	D3DXMatrixOrthoOffCenterLH(&projection, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	D3DXMatrixTranspose(&projection, &projection);
	renderDeviceContext->UpdateSubresource( projectionBuffer, 0, NULL, &projection, 0, 0 );

}


void Renderer::SetWorldMatrix( D3DXMATRIX* WorldMatrix )
{
	D3DXMATRIX world;
	D3DXMatrixTranspose(&world, WorldMatrix);
	renderDeviceContext->UpdateSubresource(worldBuffer, 0, NULL, &world, 0, 0);
}

void Renderer::SetViewMatrix( D3DXMATRIX* ViewMatrix )
{
	D3DXMATRIX view;
	D3DXMatrixTranspose(&view, ViewMatrix);
	renderDeviceContext->UpdateSubresource(viewBuffer, 0, NULL, &view, 0, 0);
}

void Renderer::SetProjectionMatrix( D3DXMATRIX* ProjectionMatrix )
{
	D3DXMATRIX projection;
	D3DXMatrixTranspose(&projection, ProjectionMatrix);
	renderDeviceContext->UpdateSubresource(projectionBuffer, 0, NULL, &projection, 0, 0);
}



void Renderer::SetMaterial( MATERIAL Material )
{
	renderDeviceContext->UpdateSubresource( materialBuffer, 0, NULL, &Material, 0, 0 );
}

void Renderer::SetLight( LIGHT Light )
{
	renderDeviceContext->UpdateSubresource(lightBuffer, 0, NULL, &Light, 0, 0);
}





void Renderer::CreateVertexShader( ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName )
{

	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	renderDevice->CreateVertexShader(buffer, fsize, NULL, VertexShader);


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	renderDevice->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	delete[] buffer;
}



void Renderer::CreatePixelShader( ID3D11PixelShader** PixelShader, const char* FileName )
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	renderDevice->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	delete[] buffer;
}


