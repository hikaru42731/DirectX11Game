//======================================================
//
//             テクスチャ管理[Texture.h]
//
//
//======================================================

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <d3d9.h>

/*------------------------------------
  定数定義
-------------------------------------*/
#define TEXTURE_FILENAME_MAX (64) // テクスチャファイル名最大文字数
#define TEXTURE_MAX (1024)        // テクスチャ管理最大数
#define TEXTURE_INVLIT_ID (-1)    // 無効なテクスチャ管理番号
#define TEXTURE_INVLIT_ID1 (-1)

// テクスチャ管理モジュールの初期化
void Texture_Init(void);

// テクスチャ管理モジュールの終了
void Texture_Fin(void);

// テクスチャファイルの読み込み予約
// 
// 引数  pFilname・・・ファイル名
//
// 戻り値　テクスチャ管理番号
//            最大管理数を超えていた場合はINVALIT_TEXTURE_ID
//
int Texture_SetTextureLoadFile(const char* pFileName);

// テクスチャの読み込み
//
// 予約されているテクスチャを読み込む
//
// 戻り値　　読み込まれなかったファイル数
//              または他の不具合が発生した場合は -1
//
int Texture_Load(void);

// テクスチャの部分解放
//
// 引数　textureids[]・・・解放したいtextureidが入ったint型配列の先頭アドレス
// 　　　　　count　 ・・・解放したいテクスチャの数
//
void Texture_Release(int textureids[], int count);

// テクスチャの全解放
void Texture_AllRelease(void);

// テクスチャの幅の取得
//
// 引数　textureid・・・テクスチャ管理番号
//
// 戻り値　テクスチャの幅
//
unsigned long Texture_GetTextureWidth(int textureid);

// テクスチャの高さの取得
//
// 引数　textureid・・・テクスチャ管理番号
//
// 戻り値　テクスチャの高さ
//
unsigned long Texture_GetTextureHight(int textureid);

// テクスチャインターフェースポインタの取得
//
// 引数　テクスチャ管理番号
//
// 戻り値　テクスチャインターフェースポインタ
//            読み込めてないまたは不正な管理番号だった場合はNULL
//
LPDIRECT3DTEXTURE9 Texture_GetTexture(int textureid);

#endif // TEXTURE_H_