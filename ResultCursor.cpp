#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Scene.h"
#include "Input.h"
#include "Audio.h"
#include "GameObject.h"
#include "ResultTexture.h"
#include "ResultCursor.h"

void ResultCursor::Init()
{
	VERTEX_3D vertex[4];
	objPosition = D3DXVECTOR3(500, 580, 0);
	objScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	sizeY = 50;
	sizeX = 360;
	// 左上
	vertex[0].position = objPosition;
	vertex[0].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f);
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);
	// 右上
	vertex[1].position = D3DXVECTOR3(objPosition.x + sizeX, objPosition.y, objPosition.z);
	vertex[1].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f);
	vertex[1].texCoord = D3DXVECTOR2(1.0f, 0.0f);
	// 左下
	vertex[2].position = D3DXVECTOR3(objPosition.x, objPosition.y + sizeY, objPosition.z);
	vertex[2].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f);
	vertex[2].texCoord = D3DXVECTOR2(0.0f, 1.0f);
	// 右下
	vertex[3].position = D3DXVECTOR3(objPosition.x + sizeX, objPosition.y + sizeY, objPosition.z);
	vertex[3].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f);
	vertex[3].texCoord = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファ作成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4; //頂点バッファの数
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{}; // 頂点バッファを入れるデータ
	sd.pSysMem = vertex; // さっき作った頂点データで初期化してくれ

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &resultCurVertexBuffer);// 出来上がったものをVertexBufferに入れる
	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/Red.png",
		NULL,
		NULL,
		&resultCurTexture,
		NULL);
	// テクスチャを読み込めたか確認する
	assert(resultCurTexture);

	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "unlitTexturePS.cso");

	moveSe = Manager::GetScene()->AddGameObject<Audio>();
	moveSe->Load("asset\\Audio\\Cursor.wav");
	moveSe->SetVolume(1.5f);
	pressSe = Manager::GetScene()->AddGameObject<Audio>();
	pressSe->Load("asset\\Audio\\HitSword.wav");
	pressSe->SetVolume(1.5f);
}

void ResultCursor::Uninit()
{
	resultCurVertexBuffer->Release();
	resultCurTexture->Release();
	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void ResultCursor::Update()
{
	class R_Texture* tex = Manager::GetScene()->Get2DGameObject<R_Texture>();
	if (tex->GetWin() == true)
	{
		if (objPosition.y == 580)
		{
			if (Input::GetKeyTrigger('W'))	//上入力
			{
				moveSe->Play(false);
				objPosition.y -= 65;
				return;
			}
		}
		if (objPosition.y == 515)
		{
			if (Input::GetKeyTrigger('S'))	//上入力
			{
				moveSe->Play(false);
				objPosition.y += 65;
				return;
			}
		}
	}
	if (objPosition.y == 645)
	{
		if (Input::GetKeyTrigger('W'))	//上入力
		{
			moveSe->Play(false);
			objPosition.y -= 65;
		}
	}
	if (objPosition.y == 580)
	{
		if (Input::GetKeyTrigger('S'))	//下入力
		{
			moveSe->Play(false);
			objPosition.y += 65;
		}
	}

	if(Input::GetKeyTrigger('L'))pressSe->Play(false);
}

void ResultCursor::Draw()
{
	// 頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(resultCurVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
	// 左上
	vertex[0].position = objPosition;
	vertex[0].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 0.5f);
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);
	// 右上
	vertex[1].position = D3DXVECTOR3(objPosition.x + sizeX, objPosition.y, objPosition.z);
	vertex[1].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 0.5f);
	vertex[1].texCoord = D3DXVECTOR2(1.0f, 0.0f);
	// 左下
	vertex[2].position = D3DXVECTOR3(objPosition.x, objPosition.y + sizeY, objPosition.z);
	vertex[2].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 0.5f);
	vertex[2].texCoord = D3DXVECTOR2(0.0f, 1.0f);
	// 右下
	vertex[3].position = D3DXVECTOR3(objPosition.x + sizeX, objPosition.y + sizeY, objPosition.z);
	vertex[3].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 0.5f);
	vertex[3].texCoord = D3DXVECTOR2(1.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(resultCurVertexBuffer, 0);
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
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &resultCurVertexBuffer,
		&stride, &offset);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &resultCurTexture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}
