#ifndef DEBUG_FONT_H_
#define DEBUG_FONT_H_

// デバッグフォントモジュールの初期化
void DebugFont_Init(void);

// デバッグフォントの表示
// 
// 引数  pString...デバッグ文字列へのポインタ
//         dx,dy...デバッグ文字列の描画座標
void DebugFont_Draw(const char* pString, float dx, float dy);

// デバッグフォントモジュールの終了
void DebugFont_Fin(void);


#endif // !DEBUG_FONT_H_

