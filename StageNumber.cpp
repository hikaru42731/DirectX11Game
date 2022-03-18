#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Scene.h"
#include "AbilitySelect.h"
#include "StageNumber.h"

void StageNumber::Init()
{
	VERTEX_3D vertex[4];
	objPosition = D3DXVECTOR3(800, 950, 0);
	sizeY = 87;
	sizeX = 157;
	// 左上
	vertex[0].position = objPosition;
	vertex[0].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);
	// 右上
	vertex[1].position = D3DXVECTOR3(objPosition.x + sizeX, objPosition.y, objPosition.z);
	vertex[1].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2(1.0f, 0.0f);
	// 左下
	vertex[2].position = D3DXVECTOR3(objPosition.x, objPosition.y + sizeY, objPosition.z);
	vertex[2].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2(0.0f, 1.0f);
	// 右下
	vertex[3].position = D3DXVECTOR3(objPosition.x + sizeX, objPosition.y + sizeY, objPosition.z);
	vertex[3].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファ作成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4; //頂点バッファの数
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{}; // 頂点バッファを入れるデータ
	sd.pSysMem = vertex; // さっき作った頂点データで初期化してくれ

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &abilityOnVertexBuffer);// 出来上がったものをVertexBufferに入れる
	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/StageNumber/Stage1.png",
		NULL,
		NULL,
		&numberTexture[0],
		NULL);
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/StageNumber/Stage2.png",
		NULL,
		NULL,
		&numberTexture[1],
		NULL);
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/StageNumber/Stage3.png",
		NULL,
		NULL,
		&numberTexture[2],
		NULL);
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/StageNumber/Stage4.png",
		NULL,
		NULL,
		&numberTexture[3],
		NULL);
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/StageNumber/Stage5.png",
		NULL,
		NULL,
		&numberTexture[4],
		NULL);

	
	number = AbilitySelect::GameStageNumber();
	drawTexture = numberTexture[number];
	// テクスチャを読み込めたか確認する
	assert(abilityTexture);

	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "unlitTexturePS.cso");

	objScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void  StageNumber::Uninit()
{
	abilityOnVertexBuffer->Release();
	drawTexture->Release();
	for (int i = 0; i < 5; i++)
	{
		numberTexture[i]->Release();
	}
	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void   StageNumber::Update()
{
	if (number > Manager::GetUnLockStage())number = 0;
	drawTexture = numberTexture[number];
	AbilitySelect::SetStageNumber(number);
}

void   StageNumber::Draw()
{
	// 頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(abilityOnVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
	// 左上
	vertex[0].position = objPosition;
	vertex[0].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);
	// 右上
	vertex[1].position = D3DXVECTOR3(objPosition.x + sizeX, objPosition.y, objPosition.z);
	vertex[1].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2(1.0f, 0.0f);
	// 左下
	vertex[2].position = D3DXVECTOR3(objPosition.x, objPosition.y + sizeY, objPosition.z);
	vertex[2].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2(0.0f, 1.0f);
	// 右下
	vertex[3].position = D3DXVECTOR3(objPosition.x + sizeX, objPosition.y + sizeY, objPosition.z);
	vertex[3].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = D3DXVECTOR2(1.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(abilityOnVertexBuffer, 0);
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(objVertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(objVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(objPixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &abilityOnVertexBuffer,
		&stride, &offset);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &drawTexture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}
