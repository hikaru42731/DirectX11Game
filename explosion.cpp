#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Scene.h"
#include "Camera.h"
#include "Explosion.h"

// Explosion:: でExplosionクラスの関数を指定している
ID3D11ShaderResourceView* Explosion::explosionTexture;
ID3D11ShaderResourceView* Bomb::bombTexture;
ID3D11ShaderResourceView* LASTCHARGE::attackTexture;
ID3D11ShaderResourceView* LASTCHARGE::chargeTexture;
void Particle::Init()
{
	num = 30;
	
}
void Particle::Uninit()
{
}
void Particle::Update()
{
	for (int i = 0; i < num; i++)
	{
		Manager::GetScene()->AddGameObject<Explosion>()->SetPosition(objPosition);
		
	}
	SetDestroy();
}
void Particle::Draw()
{

}

void Explosion::Init()
{
	VERTEX_3D vertex[4];
	// 左上
	vertex[0].position = D3DXVECTOR3(-10.0f, 10.0f, 0.0f);    // 頂点のポジション
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // どの面が上なのか(法線)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); 
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);			  
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
	bd.CPUAccessFlags =  D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{}; // 頂点バッファを入れるデータ
	sd.pSysMem = vertex; // さっき作った頂点データで初期化してくれ

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &explosionVertexBuffer);// 出来上がったものをVertexBufferに入れる

	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "unlitTexturePS.cso");

	if (rand() % 10 > 5)
	{
		if (rand() % 4 <= 2)
		{
			moveVec = D3DXVECTOR2(rand() % 5 + 1, rand() % 5 + 1);
		}
		else
		{
			moveVec = D3DXVECTOR2(rand() % 5 + 1, -rand() % 5 + 1);
		}
	}
	else
	{
		if (rand() % 4 <= 2)
		{
			moveVec = D3DXVECTOR2(-rand() % 5 - 1, rand() % 5 + 1);
		}
		else
		{
			moveVec = D3DXVECTOR2(-rand() % 5 - 1, -rand() % 5 + 1);
		}
	}
	D3DXVec2Normalize(&moveVec, &moveVec);
	objRotation = D3DXVECTOR3(moveVec.x,moveVec.y,0);
	color = D3DXVECTOR4(0, 0, 0, 1);
	objScale = D3DXVECTOR3(rand()%3+1.0f, rand() % 3 + 1.0f, 1.0f);
}

void Explosion::Uninit()
{
	explosionVertexBuffer->Release();

	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void Explosion::Update()
{
	cnt++;
	if (cnt % 3 == 0) {
		animeCount++;
	}
	moveVec.y += 0.05f;
	objPosition.x -= moveVec.x * 0.2;
	objPosition.y -= moveVec.y * 0.2;
	color.x += 0.08f;
	color.y += 0.08f;
	if (animeCount >= 16)
	{
		SetDestroy();
		return;
	}
}

void Explosion::Draw()
{
	//テクスチャ座標算出
	float x = animeCount % 4 * (1.0f / 4);
	float y = animeCount / 5 * (1.0f / 1);

	// 頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(explosionVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// 左上
	vertex[0].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);    // 頂点のポジション
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // どの面が上なのか(法線)
	vertex[0].diffuse = color; 
	vertex[0].texCoord = D3DXVECTOR2(x, y);			  
	// 右上
	vertex[1].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = color;
	vertex[1].texCoord = D3DXVECTOR2(x + 0.2f, y);
	// 左下
	vertex[2].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = color;
	vertex[2].texCoord = D3DXVECTOR2(x, y + 0.25f);
	// 右下
	vertex[3].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].diffuse = color;
	vertex[3].texCoord = D3DXVECTOR2(x + 0.2f, y + 0.25f);

	Renderer::GetDeviceContext()->Unmap(explosionVertexBuffer, 0);

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
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &explosionVertexBuffer,
		&stride, &offset);

	// マテリアル設定
	MATERIAL material{};
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &explosionTexture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void Explosion::Load()
{
	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/explosion.png",
		NULL,
		NULL,
		&explosionTexture,
		NULL);

	// テクスチャを読み込めたか確認する
	assert(explosionTexture);
}

void Explosion::UnLoad()
{
	explosionTexture->Release();
}

void Bomb::Init()
{
	VERTEX_3D vertex[4];
	// 左上
	vertex[0].position = D3DXVECTOR3(-10.0f, 10.0f, 0.0f);    // 頂点のポジション
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // どの面が上なのか(法線)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); 
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);			  
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

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &bombVertexBuffer);// 出来上がったものをVertexBufferに入れる



	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "unlitTexturePS.cso");

	objPosition = D3DXVECTOR3(0.0f, 4.0f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(4.0f, 3.0f, 2.0f);
	cnt = 0;
	animeCount = 6;
}


void Bomb::Uninit()
{
	bombVertexBuffer->Release();

	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void Bomb::Update()
{
	cnt++;
	if (cnt % 3 == 0) {
		animeCount++;
	}
	
	
	if (animeCount >= 13)
	{
		SetDestroy();
		return;
	}
}

void Bomb::Draw()
{
	//テクスチャ座標算出
	float x = (animeCount % 5 * (1.0f / 5));
	float y = (animeCount / 5 * (1.0f / 3));

	// 頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(bombVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// 左上
	vertex[0].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);    // 頂点のポジション
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // どの面が上なのか(法線)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); 
	vertex[0].texCoord = D3DXVECTOR2(x, y);			  
	// 右上
	vertex[1].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2(x + 0.2f, y);
	// 左下
	vertex[2].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2(x, y + 0.33f);
	// 右下
	vertex[3].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = D3DXVECTOR2(x + 0.2f, y + 0.33f);

	Renderer::GetDeviceContext()->Unmap(bombVertexBuffer, 0);

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
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &bombVertexBuffer,
		&stride, &offset);

	// マテリアル設定
	MATERIAL material{};
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &bombTexture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void Bomb::Load()
{
	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/Bomb.png",
		NULL,
		NULL,
		&bombTexture,
		NULL);

	// テクスチャを読み込めたか確認する
	assert(bombTexture);
}

void Bomb::UnLoad()
{
	bombTexture->Release();
}

void  LASTCHARGE::Init()
{
	VERTEX_3D vertex[4];
	// 左上
	vertex[0].position = D3DXVECTOR3(-10.0f, 10.0f, 0.0f);    // 頂点のポジション
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // どの面が上なのか(法線)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); 
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);			 
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

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &lastChargeVertexBuffer);// 出来上がったものをVertexBufferに入れる



	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "unlitTexturePS.cso");

	objPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	cnt = 0;
	charge = true;
	animeCount = 0;
}


void  LASTCHARGE::Uninit()
{
	lastChargeVertexBuffer->Release();

	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void LASTCHARGE::Update()
{
	
	if (charge == true)
	{
		if (cnt % 4 == 0) {
			animeCount++;
		}


		if (animeCount >= 13)
		{
			charge = false;
			animeCount = 0;
			
			return;
		}
	}
	else
	{
		if (cnt % 4 == 0) {
			animeCount++;
		}


		if (animeCount >= 13)
		{
			SetDestroy();
			return;
		}
	}
	cnt++;
}

void  LASTCHARGE::Draw()
{
	//テクスチャ座標算出
	float x = (animeCount % 5 * (1.0f / 5));
	float y = (animeCount / 5 * (1.0f / 3));

	// 頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(lastChargeVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// 左上
	vertex[0].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);    // 頂点のポジション
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // どの面が上なのか(法線)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); 
	vertex[0].texCoord = D3DXVECTOR2(x, y);			  
	// 右上
	vertex[1].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2(x + 0.2f, y);
	// 左下
	vertex[2].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2(x, y + 0.33f);
	// 右下
	vertex[3].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = D3DXVECTOR2(x + 0.2f, y + 0.33f);

	Renderer::GetDeviceContext()->Unmap(lastChargeVertexBuffer, 0);

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
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &lastChargeVertexBuffer,
		&stride, &offset);

	// マテリアル設定
	MATERIAL material{};
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//テクスチャ設定
	if (charge == true)
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &chargeTexture);
	}
	else
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &attackTexture);
	}

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void  LASTCHARGE::Load()
{
	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/Charge.png",
		NULL,
		NULL,
		&chargeTexture,
		NULL);

	// テクスチャを読み込めたか確認する
	assert(chargeTexture);
	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/Last.png",
		NULL,
		NULL,
		&attackTexture,
		NULL);

	// テクスチャを読み込めたか確認する
	assert(attackTexture);
}

void  LASTCHARGE::UnLoad()
{
	chargeTexture->Release();
	attackTexture->Release();
}
