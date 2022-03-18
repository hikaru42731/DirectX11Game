#include "Debug_font.h"
#include "Texture.h"
#include "Sprite.h"
#include<string.h>

#define DEBUGFONT_WIDTH 32
#define DEBUGFONT_HIGHT 64
#define DEBUGFONT_ONELINE_CHARCOUNT 16

static int g_TextureId = TEXTURE_INVLIT_ID;

// デバッグフォントモジュールの初期化
void DebugFont_Init(void)
{
	g_TextureId = Texture_SetTextureLoadFile("Asset/debug_font_32x64.png");

	// テクスチャの読み込み
	/*if (Texture_Load() > 0) {
		MessageBox(NULL, "テクスチャの読み込みに失敗しました", "エラー", MB_OK);
	}*/
}

// デバッグフォントの表示
// 
// 引数  pString...デバッグ文字列へのポインタ
//         dx,dy...デバッグ文字列の描画座標
void DebugFont_Draw(const char* pString, float dx, float dy)
{
	for (int i = 0; i < strlen(pString); i++) {

		int offset = pString[i] - ' ';
		int tcx = offset % DEBUGFONT_ONELINE_CHARCOUNT * DEBUGFONT_WIDTH;
		int tcy = offset / DEBUGFONT_ONELINE_CHARCOUNT * DEBUGFONT_HIGHT;

		Draw_Sprite(g_TextureId, dx + i * DEBUGFONT_WIDTH, dy, 16.0f, 32.0f, tcx,tcy, DEBUGFONT_WIDTH, DEBUGFONT_HIGHT);
	}
}

// デバッグフォントモジュールの終了
void DebugFont_Fin(void)
{
	Texture_Release(&g_TextureId, 1);
}
