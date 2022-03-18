#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Scene.h"
#include "Camera.h"
#include "Enemy.h"
#include "EnemyWarp.h"
#include "Audio.h"

std::vector<Enemy*> w_enemyList;
class Audio* EnemyWarp::seWarp;
ID3D11ShaderResourceView* EnemyWarp::enemyWarpTexture;
void EnemyWarp::Init()
{
	VERTEX_3D vertex[4];
	// 左上
	vertex[0].position = D3DXVECTOR3(-10.0f, 10.0f, 0.0f);    // 頂点のポジション
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // どの面が上なのか(法線)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); //
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);			  //
	// 右上
	vertex[1].position = D3DXVECTOR3(10.0f, 10.0f, 0.0f);
	vertex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2(1.0f, 0.0f);
	// 左下
	vertex[2].position = D3DXVECTOR3(-10.0f, -10.0f, 0.0f);
	vertex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2(0.0f, 1.0f);
	// 右下
	vertex[3].position = D3DXVECTOR3(10.0f, -10.0f, 0.0f);
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
	sd.pSysMem = vertex; // さっき作った頂点データで初期化してくれ

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &enemyWarpVertexBuffer);// 出来上がったものをVertexBufferに入れる

	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "unlitTexturePS.cso");

	objPosition = D3DXVECTOR3(0.0f, 4.0f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(1.0f, 0.5f, 2.0f);
	extend = true;
	audioPlay = false;
	w_enemyList.clear();
}

void EnemyWarp::Uninit()
{
	enemyWarpVertexBuffer->Release();

	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void EnemyWarp::Update()
{
	if (warp == true)
	{
		if (audioPlay == false)
		{
			seWarp->Play(false);
			audioPlay = true;
		}
		if (extend == true)
		{
			telescopicSpeed += 0.001f;
			objScale.x += telescopicSpeed;;
			if (objScale.x > 2.0f)extend = false;
		}

	}
	else
	{
		audioPlay = false;
		objScale.x = 1.0f;
		extend = true;
	}
}

void EnemyWarp::Draw()
{
	// 頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(enemyWarpVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// 左上
	vertex[0].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);    // 頂点のポジション
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // どの面が上なのか(法線)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); //
	vertex[0].texCoord = D3DXVECTOR2(0, 0);			  //
	// 右上
	vertex[1].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2(1, 0);
	// 左下
	vertex[2].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2(0, 1);
	// 右下
	vertex[3].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = D3DXVECTOR2(1,1);

	Renderer::GetDeviceContext()->Unmap(enemyWarpVertexBuffer, 0);

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(objVertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(objVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(objPixelShader, NULL, 0);

	// カメラのビューマトリクス取得
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>(); // 単体で読み込む時はGetGameObjectを使う。複数読み込む時、敵や玉などはGetGameObjetsを使ってループ処理で読み込む。
	D3DXMATRIX view = camera->GetViewMatrix();

	//　ビューの逆行列 逆の計算をする行列。カメラに対して逆計算をすることでビルボードになる
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view); // 逆行列
	invView._41 = 0.0f; // 移動成分を0に設定して移動出来ないようにする　floatで値を管理しているから0に設定している
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	//マトリクス設定
	D3DXMATRIX world, scale, trans;
	D3DXMatrixScaling(&scale, objScale.x, objScale.y, objScale.z);
	D3DXMatrixTranslation(&trans, objPosition.x, objPosition.y, objPosition.z);
	world = scale * invView * trans;
	Renderer::SetWorldMatrix(&world);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &enemyWarpVertexBuffer,
		&stride, &offset);

	// マテリアル設定
	MATERIAL material{};
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &enemyWarpTexture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		if (warp == true) {
			// ポリゴン描画
			Renderer::GetDeviceContext()->Draw(4, 0);
		}
}


void EnemyWarp::Load()
{
	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/Warp.png",
		NULL,
		NULL,
		&enemyWarpTexture,
		NULL);

	// テクスチャを読み込めたか確認する
	assert(enemyWarpTexture);
	seWarp = new Audio;
	seWarp->Load("asset\\Audio\\Warp.wav");
}

void EnemyWarp::UnLoad()
{
	enemyWarpTexture->Release();
}
