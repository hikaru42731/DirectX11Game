#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Audio.h"
#include "Scene.h"
#include "Fade.h"

ID3D11Buffer* Fade::fadeVertexBuffer=NULL;
ID3D11ShaderResourceView* Fade::fadeTexture=NULL;

ID3D11VertexShader* Fade::fadeVertexShader=NULL;
ID3D11PixelShader* Fade::fadePixelShader=NULL;
ID3D11InputLayout* Fade::fadeVertexLayout=NULL;
bool Fade::extend=false;
bool Fade::shrink = false;
float Fade::alpha=0;
bool Fade::end = false;
class Audio* Fade::fadeSe = NULL;

void Fade::Init()
{
	alpha = 0;
	VERTEX_3D vertex[4];
	// 左上
	vertex[0].position = D3DXVECTOR3(0, 0, 0);    // 頂点のポジション
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // どの面が上なのか(法線)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, alpha); //
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);			  //
	// 右上
	vertex[1].position = D3DXVECTOR3(SCREEN_WIDTH, 0, 0);
	vertex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, alpha);
	vertex[1].texCoord = D3DXVECTOR2(1.0f, 0.0f);
	// 左下
	vertex[2].position = D3DXVECTOR3(0, SCREEN_HEIGHT, 0);
	vertex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, alpha);
	vertex[2].texCoord = D3DXVECTOR2(0.0f, 1.0f);
	// 右下
	vertex[3].position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	vertex[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, alpha);
	vertex[3].texCoord = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファ作成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4; //頂点バッファの数
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{}; // 頂点バッファを入れるデータ
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &fadeVertexBuffer);// 出来上がったものをVertexBufferに入れる

	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/Fadein.png",
		NULL,
		NULL,
		&fadeTexture,
		NULL);

	// テクスチャを読み込めたか確認する
	assert(fadeTexture);

	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&fadeVertexShader, &fadeVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&fadePixelShader, "unlitTexturePS.cso");
	extend = false;
	shrink = false;
	end = false;
}

void Fade::Uninit()
{
	fadeVertexBuffer->Release();
	fadeTexture->Release();

	fadeVertexLayout->Release();
	fadeVertexShader->Release();
	fadePixelShader->Release();
}

void Fade::Update()
{
	
}

void Fade::Draw()
{
	if (shrink == false && extend == false)
	{
		end = false;
	}

	if (extend == true)
	{
		if (fadeSe != NULL) 
		{
			if (alpha == 0)fadeSe->Play(false);
		}
		alpha += 0.025f;
		
		if (alpha >= 1.0f)
		{
			end = true;
			extend = false;
		}
	}
	if (shrink == true)
	{
		alpha -= 0.01f;
		if(alpha <= 0.0f)
		{
			end = true;
			shrink = false;
			alpha = 0;
		}
	}
	
	// 頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(fadeVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// 左上
	vertex[0].position = D3DXVECTOR3(0, 0, 0);    // 頂点のポジション
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // どの面が上なのか(法線)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, alpha); //
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);			  //
	// 右上
	vertex[1].position = D3DXVECTOR3(SCREEN_WIDTH, 0, 0);
	vertex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, alpha);
	vertex[1].texCoord = D3DXVECTOR2(1.0f, 0.0f);
	// 左下
	vertex[2].position = D3DXVECTOR3(0, SCREEN_HEIGHT, 0);
	vertex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, alpha);
	vertex[2].texCoord = D3DXVECTOR2(0.0f, 1.0f);
	// 右下
	vertex[3].position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	vertex[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, alpha);
	vertex[3].texCoord = D3DXVECTOR2(1.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(fadeVertexBuffer, 0);

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(fadeVertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(fadeVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(fadePixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();


	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &fadeVertexBuffer,
		&stride, &offset);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &fadeTexture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void Fade::Load()
{
	fadeSe = Manager::GetScene()->AddGameObject<Audio>();
	fadeSe->Load("asset\\Audio\\Fade.wav");
	fadeSe->SetVolume(3.5f);
}

void Fade::UnLoad()
{
	fadeSe->Uninit();
	delete fadeSe;
}
