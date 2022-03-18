#include "Sprite.h"
#include <d3dx9.h>
#include "MyDirect3D.h"
#include "Texture.h"

/*-----------------------------------------------------------------------------
   構造体
------------------------------------------------------------------------------*/
// 頂点構造体
typedef struct Vertex2D_tag {
	D3DXVECTOR4 Posision;
	D3DCOLOR Color;
	D3DXVECTOR2 TexCord;

}Vertex2D;
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

/*-----------------------------------------------------------------------------
   グローバル変数宣言
------------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;// 頂点バッファインターフェース
LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;  // インデックスバッファーインターフェース
D3DCOLOR g_Color=0xffffffff;

// スプライトの初期化
void Init_Sprite(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) {
		return;
	}

	g_Color = (255, 255, 255, 255);

	pDevice->CreateVertexBuffer(
		sizeof(Vertex2D) * 4, // 頂点バッファの量（バイト）
		D3DUSAGE_WRITEONLY, // 使い方
		FVF_VERTEX2D,       // FVF
		D3DPOOL_MANAGED,    // メモリの管理方法
		&g_pVertexBuffer,   // 取得したインターフェースのアドレスを記録するためのポインタへのアドレス
		NULL
	);

	pDevice->CreateIndexBuffer(
		sizeof(WORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIndexBuffer,
		NULL
	);
}

// スプライトの終了
void Fin_Sprite(void)
{
	if (g_pIndexBuffer) {
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}

	if (g_pVertexBuffer) {
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}

// スプライトの描画
// *テクスチャサイズのポリゴンで指定座標に描画
//
// 引数: textureId...テクスチャ管理番号
//               dx...座標X
//               dy...座標Y
//               dw...ポリゴンサイズ幅
//               dh...ポリゴンサイズ高さ
//              tcx...テクスチャ切り取り座標X
//              tcy...テクスチャ切り取り座標Y
//               
void Draw_SpriteTestIndex(int textureId, float dx, float dy)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) {
		MessageBox(NULL, "ダイレクト３Dの読み込みに失敗しました", "エラー", MB_OK);
	}

	// デバイスにFVFの設定をする
	pDevice->SetFVF(FVF_VERTEX2D);

	// デバイスにテクスチャの設定をする
	pDevice->SetTexture(0, Texture_GetTexture(textureId));
	//pDevice->SetTexture(0, NULL);


	// ポリゴンのサイズはテクスチャサイズ
	unsigned long w = Texture_GetTextureWidth(textureId);
	unsigned long h = Texture_GetTextureHight(textureId);

	// 頂点データ
	Vertex2D v[] = {
		{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f)},
		{ D3DXVECTOR4(dx + w - 0.5f, dy - 0.5f,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f)},
		{ D3DXVECTOR4(dx - 0.5f, dy + h - 0.5f,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f)},
		{ D3DXVECTOR4(dx + w - 0.5f, dy + h - 0.5f,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,1.0f)},
	};

	// 頂点バッファをロックしてデータを書き込む
	Vertex2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);// 0を入れると全体をロック

	// データを書き込む
	memcpy(pV, v, sizeof(v));

	g_pVertexBuffer->Unlock();

	// デバイスに利用する頂点バッファを指定する
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2D));

	// インデックスバッファをロックしてデータを書き込む
	WORD* pI;
	g_pIndexBuffer->Lock(0, 0, (void**)&pI, 0);
	pI[0] = 0;
	pI[1] = 1;
	pI[2] = 2;
	pI[3] = 1;
	pI[4] = 3;
	pI[5] = 2;
	g_pIndexBuffer->Unlock();
	
	// デバイスに利用する頂点バッファを指定する
	pDevice->SetIndices(g_pIndexBuffer);

	// ポリゴン描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

// スプライトの描画
// *テクスチャサイズのポリゴンで指定座標に描画
//
// 引数: textureId...テクスチャ管理番号
//               dx...座標X
//               dy...座標Y
//               dw...ポリゴンサイズ幅
//               dh...ポリゴンサイズ高さ
//              tcx...テクスチャ切り取り座標X
//              tcy...テクスチャ切り取り座標Y
//               
void Draw_SpriteTest(int textureId, float dx, float dy)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) {
		MessageBox(NULL, "ダイレクト３Dの読み込みに失敗しました", "エラー", MB_OK);
	}

	// デバイスにFVFの設定をする
	pDevice->SetFVF(FVF_VERTEX2D);

	// デバイスにテクスチャの設定をする
	pDevice->SetTexture(0, Texture_GetTexture(textureId));
	//pDevice->SetTexture(0, NULL);


	// ポリゴンのサイズはテクスチャサイズ
	unsigned long w = Texture_GetTextureWidth(textureId);
	unsigned long h = Texture_GetTextureHight(textureId);

	// 頂点データ
	Vertex2D v[] = {
		{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f,1.0f,1.0f),g_Color,D3DXVECTOR2(0.0f,0.0f)},
		{ D3DXVECTOR4(dx + w - 0.5f, dy - 0.5f,1.0f,1.0f),g_Color,D3DXVECTOR2(1.0f,0.0f)},
		{ D3DXVECTOR4(dx - 0.5f, dy + h - 0.5f,1.0f,1.0f),g_Color,D3DXVECTOR2(0.0f,1.0f)},
		{ D3DXVECTOR4(dx + w - 0.5f, dy + h - 0.5f,1.0f,1.0f),g_Color,D3DXVECTOR2(1.0f,1.0f)},
	};

	
	Vertex2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);// 0を入れると全体をロック

	// データを書き込む
	memcpy(pV, v, sizeof(v));

	g_pVertexBuffer->Unlock();

	// デバイスに利用する頂点バッファを指定する
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2D));

	// ポリゴン描画（簡易版）
	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);	
}


// スプライトの描画
// *テクスチャサイズのポリゴンで指定座標に描画
//
// 引数: textureId...テクスチャ管理番号
//               dx...座標X
//               dy...座標Y
//               dw...ポリゴンサイズ幅
//               dh...ポリゴンサイズ高さ
//              tcx...テクスチャ切り取り座標X
//              tcy...テクスチャ切り取り座標Y
//               
void Draw_Sprite(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) {
		MessageBox(NULL, "ダイレクト３Dの読み込みに失敗しました", "エラー", MB_OK);
	}

	// デバイスにFVFの設定をする
	pDevice->SetFVF(FVF_VERTEX2D);

	// デバイスにテクスチャの設定をする
	pDevice->SetTexture(0, Texture_GetTexture(textureId));
	//pDevice->SetTexture(0, NULL);


	// ポリゴンのサイズはテクスチャサイズ
	unsigned long w = Texture_GetTextureWidth(textureId);
	unsigned long h = Texture_GetTextureHight(textureId);

	// テクスチャ切り取りUV座標
	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;

	// 頂点データ
	Vertex2D v[] = {
		{ D3DXVECTOR4(dx -      0.5f,dy -      0.5f,1.0f,1.0f),g_Color,D3DXVECTOR2(u0,v0)},
		{ D3DXVECTOR4(dw + dx - 0.5f,dy -      0.5f,1.0f,1.0f),g_Color,D3DXVECTOR2(u1,v0)},
		{ D3DXVECTOR4(dx -      0.5f,dh + dy - 0.5f,1.0f,1.0f),g_Color,D3DXVECTOR2(u0,v1)},
		{ D3DXVECTOR4(dw + dx - 0.5f,dh + dy - 0.5f,1.0f,1.0f),g_Color,D3DXVECTOR2(u1,v1)},
	};



	// ポリゴン描画（簡易版）
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));

	g_Color = D3DCOLOR_RGBA(255, 255, 255, 255);
}

// スプライトの描画
// *テクスチャサイズのポリゴンで指定座標に描画
//
// 引数: textureId...テクスチャ管理番号
//               dx...座標X
//               dy...座標Y
//               dw...ポリゴンサイズ幅
//               dh...ポリゴンサイズ高さ
//              tcx...テクスチャ切り取り座標X
//              tcy...テクスチャ切り取り座標Y
//               cx...回転の中心座標
//               cy...回転の中心座標
//            angle...回転角度
//        
void Draw_SpriteR(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch,float cx,float cy,float angle)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) {
		MessageBox(NULL, "ダイレクト３Dの読み込みに失敗しました", "エラー", MB_OK);
	}

	// デバイスにFVFの設定をする
	pDevice->SetFVF(FVF_VERTEX2D);

	// デバイスにテクスチャの設定をする
	pDevice->SetTexture(0, Texture_GetTexture(textureId));
	//pDevice->SetTexture(0, NULL);


	// ポリゴンのサイズはテクスチャサイズ
	unsigned long w = Texture_GetTextureWidth(textureId);
	unsigned long h = Texture_GetTextureHight(textureId);

	// テクスチャ切り取りUV座標
	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;	

	// 頂点データ
	Vertex2D v[] = {
		{ D3DXVECTOR4(   - 0.5f,     - 0.5f ,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(u0,v0)},
		{ D3DXVECTOR4(dw - 0.5f,     - 0.5f ,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(u1,v0)},
		{ D3DXVECTOR4(   - 0.5f,  dh - 0.5f ,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(u0 ,v1)},
		{ D3DXVECTOR4(dw - 0.5f,  dh - 0.5f ,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(u1,v1)},
	};

	
	// 並行移動行列
	D3DXMATRIX mtxTranslationC;
	D3DXMatrixTranslation(&mtxTranslationC, -cx, -cy, 0.0f);

	D3DXMATRIX mtxTranslationI;
	D3DXMatrixTranslation(&mtxTranslationI, cx + dx, cy + dy, 0.0f);

	// 回転
	D3DXMATRIX mtxRotation;
	D3DXMatrixRotationZ(&mtxRotation, angle);

	// 拡大・縮小
	D3DXMATRIX mtxScale;
	D3DXMatrixScaling(&mtxScale, 1.0f, 1.0f, 1.0f);

	// 行列の合成
	D3DXMATRIX mtxWorld;
	mtxWorld = mtxTranslationC * mtxScale * mtxRotation * mtxTranslationI;

	// 座標変換
	for (int i = 0; i < 4; i++) {
		D3DXVec4Transform(&v[i].Posision, &v[i].Posision, &mtxWorld);
	}

	// ポリゴン描画（簡易版）
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}


void Sprite_SetColor(D3DCOLOR color)
{
	g_Color = color;
}