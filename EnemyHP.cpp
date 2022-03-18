#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "EnemyHP.h"

float draw_num;
void EnemyHP::Init()
{
	objPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	VERTEX_3D vertex[4];
	// 左上
	vertex[0].position = D3DXVECTOR3( 750.0f, 10.0f, 0.0f );
    vertex[0].normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vertex[0].diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[0].texCoord = D3DXVECTOR2( 0.0f, 0.0f);
	// 右上
	vertex[1].position = D3DXVECTOR3( 1300.0f, 10.0f, 0.0f);
	vertex[1].normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	vertex[1].diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2( 1.0f, 0.0f);
	// 左下
	vertex[2].position = D3DXVECTOR3( 750.0f, 90.0f, 0.0f);
	vertex[2].normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	vertex[2].diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2( 0.0f, 1.0f);
	// 右下
	vertex[3].position = D3DXVECTOR3( 1300.0f, 90.0f, 0.0f);
	vertex[3].normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	vertex[3].diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = D3DXVECTOR2( 1.0f, 1.0f);

	// 頂点バッファ作成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4; //頂点バッファの数
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{}; // 頂点バッファを入れるデータ
	sd.pSysMem = vertex; // さっき作った頂点データで初期化

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &enemyHpVertexBuffer);// 出来上がったものをVertexBufferに入れる

	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
											"asset/texture/Enemy_bar.png",
											NULL,
											NULL,
											&enemyHpTexture,
											NULL);
	// テクスチャを読み込めたか確認する
	assert(enemyHpTexture);
	
	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "unlitTexturePS.cso");

	draw_num = 0;
}

void EnemyHP::Uninit()
{
	enemyHpVertexBuffer->Release();
	enemyHpTexture->Release();

	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader ->Release();
}

void EnemyHP::Update()
{
	cam = Manager::GetScene()->GetGameObject<Camera>();
	if (cam->TargetON() == true) 
	{
		draw_num = 4;
	}
	else if (cam->TargetON() == false)
	{
		draw_num = 0;
	}
}

void EnemyHP::Draw()
{
	
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
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &enemyHpVertexBuffer,
			&stride, &offset);

		//テクスチャ設定
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &enemyHpTexture);

		// プリミティブトポロジ設定
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


		// ポリゴン描画
		Renderer::GetDeviceContext()->Draw(draw_num, 0);
	
}