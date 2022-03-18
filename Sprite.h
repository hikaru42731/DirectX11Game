#ifndef SPRITE_H_
#define SPRITE_H_

#include <d3dx9.h>
// スプライトの初期化
void Init_Sprite(void);

// スプライトの終了
void Fin_Sprite(void);

void Draw_SpriteTest(int textureId, float dx, float dy);

void Draw_SpriteTestIndex(int textureId, float dx, float dy);

void Sprite_SetColor(D3DCOLOR color);

//void Sprite_SetColor(D3DCOLOR color);

// スプライトの描画
// *テクスチャサイズのポリゴンで指定座標に描画
//
// 引数: textureId...テクスチャ管理番号
//               dx...座標X
//               dy...座標X
//               tcx...テクスチャの切り取り座標X
//               tcy...テクスチャの切り取り座標Y
//               tcw...テクスチャの切り取り幅
//               tch...テクスチャの切り取り高さ
//
void Draw_Sprite(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch);

// スプライトの描画(回転機能付き)
// *テクスチャサイズのポリゴンで指定座標に描画
//
// 引数: textureId...テクスチャ管理番号
//               dx...座標X
//               dy...座標X
//               tcx...テクスチャの切り取り座標X
//               tcy...テクスチャの切り取り座標Y
//               tcw...テクスチャの切り取り幅
//               tch...テクスチャの切り取り高さ
//
void Draw_SpriteR(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch, float cx, float cy, float angle);


#endif // !SPRITE_H_
