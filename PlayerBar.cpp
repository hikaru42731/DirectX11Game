#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Scene.h"
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "PlayerBar.h"

void Player_Bar::Init()
{
	sizeX = 450.0f;
	VERTEX_3D vertex[4];
	// 左上
	vertex[0].position = D3DXVECTOR3(756.5f, 587.0f, 0.0f);    // 頂点のポジション
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // どの面が上なのか(法線)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); //
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);			  //
	// 右上
	vertex[1].position = D3DXVECTOR3(756.5f + sizeX, 587.0f, 0.0f);
	vertex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2(1.0f, 0.0f);
	// 左下
	vertex[2].position = D3DXVECTOR3(756.5f, 619.0f, 0.0f);
	vertex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2(0.0f, 1.0f);
	// 右下
	vertex[3].position = D3DXVECTOR3(756.5f + sizeX, 619.0f, 0.0f);
	vertex[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファ作成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4; //頂点バッファの数
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{}; // 頂点バッファを入れるデータ
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &playerBarVertexBuffer);// 出来上がったものをVertexBufferに入れる

	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/PlayerHP.png",
		NULL,
		NULL,
		&playerBarTexture,
		NULL);

	// テクスチャを読み込めたか確認する
	assert(playerBarTexture);

	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "unlitTexturePS.cso");

	objPosition = D3DXVECTOR3(0.0f, 1000.0f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(5.0f, 5.0f, 1.0f);

	num = 0;
}

void Player_Bar::Uninit()
{
	playerBarVertexBuffer->Release();
	playerBarTexture->Release();

	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void Player_Bar::Update()
{
	barPlayer = Manager::GetScene()->GetGameObject<Player>();
	sizeX = barPlayer->GetHP()/2.225;
}

void Player_Bar::Draw()
{
	// 頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(playerBarVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// 左上
	vertex[0].position = D3DXVECTOR3(756.5f, 587.0f, 0.0f);    // 頂点のポジション
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // どの面が上なのか(法線)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); //
	vertex[0].texCoord = D3DXVECTOR2(0, 0);			  //
	// 右上
	vertex[1].position = D3DXVECTOR3(756.5 + sizeX, 587.0f, 0.0f);
	vertex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2(1, 0);
	// 左下
	vertex[2].position = D3DXVECTOR3(756.5f, 619.0f, 0.0f);
	vertex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2(0, 1);
	// 右下
	vertex[3].position = D3DXVECTOR3(756.5 + sizeX, 619.0f, 0.0f);
	vertex[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = D3DXVECTOR2(1, 1);

	Renderer::GetDeviceContext()->Unmap(playerBarVertexBuffer, 0);

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
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &playerBarVertexBuffer,
		&stride, &offset);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &playerBarTexture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}
