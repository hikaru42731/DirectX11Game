#define _CRT_SECURE_NO_WARNINGS
#include "Texture.h"

#include <d3dx9.h>
#include <string.h>
#include "MyDirect3D.h"
#include"debugPrintf.h"


/*--------------------------------
  構造体宣言
---------------------------------*/
typedef struct Texture_tag {
	LPDIRECT3DTEXTURE9 pTexture;
	unsigned long width;         // テクスチャの幅
	unsigned long hight;         // テクスチャの高さ
	char FileName[TEXTURE_FILENAME_MAX];
}Texture;

/*--------------------------------
  グローバル変数宣言
---------------------------------*/
static Texture g_Texture[TEXTURE_MAX];

// テクスチャ管理モジュールの初期化
void Texture_Init(void)
{
	// テクスチャ管理テーブルの初期化
	for (int i = 0; i < TEXTURE_MAX; i++) {
		g_Texture[i].pTexture = NULL;
		g_Texture[i].width = 0;
		g_Texture[i].width = 0;
		g_Texture[i].FileName[0] = 0;
	}
}

// テクスチャ管理モジュールの終了
void Texture_Fin(void)
{
	for (int i = TEXTURE_MAX; i >= 0; i--) {

	}
}


// テクスチャファイルの読み込み予約
// 
// 引数  pFilname・・・ファイル名
//
// 戻り値　テクスチャ管理番号
//            最大管理数を超えていた場合はINVALIT_TEXTURE_ID
//
int Texture_SetTextureLoadFile(const char* pFileName)
{
	// ファイル名検索
	for (int i = 0; i < TEXTURE_MAX; i++) {
		// テーブルが空いていたらスキップ
		if (g_Texture[i].FileName[0] == 0) {
			continue;
		}

		// 既に指定のファイルが予約されているか?
		if (strcmp(pFileName, g_Texture[i].FileName) == 0) {
			// 予約済み
			return i;
		}
	}

	// 新規予約
	for (int i = 0; i < TEXTURE_MAX; i++) {
		// テーブルが使用中だったらスキップ
		if (g_Texture[i].FileName[0] != 0) {
			continue;
		}
		strcpy(g_Texture[i].FileName, pFileName);

		return i;
	}

	return TEXTURE_INVLIT_ID;
}

// テクスチャの読み込み
//
// 予約されているテクスチャを読み込む
//
// 戻り値　　読み込まれなかったファイル数
//
int Texture_Load(void)
{
	int err_count = 0;
	
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "デバイスの取得に失敗しました", "エラー", MB_OK);
	}

	for (int i = 0; i < TEXTURE_MAX; i++) {
		if (g_Texture[i].FileName[0] == 0) {
			// 空のテーブルなのでスキップ
			continue;
		}

		if (g_Texture[i].pTexture != NULL) {
			// 既に読み込まれている
			continue;
		}

		// テクスチャの読み込み
		HRESULT hr = D3DXCreateTextureFromFile(pDevice, g_Texture[i].FileName, &g_Texture[i].pTexture);
		if (FAILED(hr)) {
			Debug_Printf("読み込め無かったファイル:[%s]\n", g_Texture[i].FileName);
			err_count++;
		}
		else {
			// テクスチャの読み込み
			D3DXIMAGE_INFO info;
			D3DXGetImageInfoFromFile(g_Texture[i].FileName, &info);
			g_Texture[i].width = info.Width;
			g_Texture[i].hight = info.Height;
		}
	}

	return err_count;
}

// テクスチャの部分解放
//
// 引数　textureids[]・・・解放したいtextureidが入ったint型配列の先頭アドレス
// 　　　　　count　 ・・・解放したいテクスチャの数
//
void Texture_Release(int textureids[], int count)
{
	for (int i = 0; i < count; i++) {
		if (g_Texture[textureids[i]].pTexture) {
			g_Texture[textureids[i]].pTexture->Release();
			g_Texture[textureids[i]].pTexture = NULL;
		}
		g_Texture[textureids[i]].FileName[0] = 0;
		g_Texture[textureids[i]].width = 0;
		g_Texture[textureids[i]].hight = 0;
	}
}

// テクスチャの全解放
void Texture_AllRelease(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++) {
		if (g_Texture[i].pTexture) {
			g_Texture[i].pTexture->Release();
			g_Texture[i].pTexture = NULL;
		}
		g_Texture[i].FileName[0] = 0;
		g_Texture[i].width = 0;
		g_Texture[i].hight = 0;
	}
}

// テクスチャの幅の取得
//
// 引数　textureid・・・テクスチャ管理番号
//
// 戻り値　テクスチャの幅
//
unsigned long Texture_GetTextureWidth(int textureid)
{
	return g_Texture[textureid].width;
}

// テクスチャの高さの取得
//
// 引数　textureid・・・テクスチャ管理番号
//
// 戻り値　テクスチャの高さ
//
unsigned long Texture_GetTextureHight(int textureid)
{
	return g_Texture[textureid].hight;
}

// テクスチャインターフェースポインタの取得
//
// 引数　テクスチャ管理番号
//
// 戻り値　テクスチャインターフェースポインタ
//            読み込めてないまたは不正な管理番号だった場合はNULL
//
LPDIRECT3DTEXTURE9 Texture_GetTexture(int textureid)
{
	return g_Texture[textureid].pTexture;
}