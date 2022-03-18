#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "ResultTexture.h"

ID3D11ShaderResourceView* R_Texture::resultTexture=NULL;
bool R_Texture::win = false;
void R_Texture::Init()
{
	VERTEX_3D vertex[4];
	// 左上
	vertex[0].position = D3DXVECTOR3(400.0f, 0.0f, 0.0f);    // 頂点のポジション
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // どの面が上なのか(法線)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); 
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);			  
	// 右上
	vertex[1].position = D3DXVECTOR3(920, 0.0f, 0.0f);
	vertex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2(1.0f, 0.0f);
	// 左下
	vertex[2].position = D3DXVECTOR3(400.0f, SCREEN_HEIGHT, 0.0f);
	vertex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2(0.0f, 1.0f);
	// 右下
	vertex[3].position = D3DXVECTOR3(920, SCREEN_HEIGHT, 0.0f);
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

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &resultTextureVertexBuffer);// 出来上がったものをVertexBufferに入れる

	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "unlitTexturePS.cso");

	objPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void R_Texture::Uninit()
{
}

void R_Texture::Update()
{
}
void R_Texture::Draw()
{
	Renderer::GetDeviceContext()->Unmap(resultTextureVertexBuffer, 0);

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
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &resultTextureVertexBuffer,
		&stride, &offset);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &resultTexture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void R_Texture::Load(bool W)
{
	win = W;
	if (win == true)
	{
		// テクスチャ読み込み
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
			"asset/texture/Win.png",
			NULL,
			NULL,
			&resultTexture,
			NULL);
	}
	else
	{
		// テクスチャ読み込み
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
			"asset/texture/Lose.png",
			NULL,
			NULL,
			&resultTexture,
			NULL);
	}
	// テクスチャを読み込めたか確認する
	assert(m_Texture);
}
