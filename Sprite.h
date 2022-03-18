#ifndef SPRITE_H_
#define SPRITE_H_

#include <d3dx9.h>
// �X�v���C�g�̏�����
void Init_Sprite(void);

// �X�v���C�g�̏I��
void Fin_Sprite(void);

void Draw_SpriteTest(int textureId, float dx, float dy);

void Draw_SpriteTestIndex(int textureId, float dx, float dy);

void Sprite_SetColor(D3DCOLOR color);

//void Sprite_SetColor(D3DCOLOR color);

// �X�v���C�g�̕`��
// *�e�N�X�`���T�C�Y�̃|���S���Ŏw����W�ɕ`��
//
// ����: textureId...�e�N�X�`���Ǘ��ԍ�
//               dx...���WX
//               dy...���WX
//               tcx...�e�N�X�`���̐؂�����WX
//               tcy...�e�N�X�`���̐؂�����WY
//               tcw...�e�N�X�`���̐؂��蕝
//               tch...�e�N�X�`���̐؂��荂��
//
void Draw_Sprite(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch);

// �X�v���C�g�̕`��(��]�@�\�t��)
// *�e�N�X�`���T�C�Y�̃|���S���Ŏw����W�ɕ`��
//
// ����: textureId...�e�N�X�`���Ǘ��ԍ�
//               dx...���WX
//               dy...���WX
//               tcx...�e�N�X�`���̐؂�����WX
//               tcy...�e�N�X�`���̐؂�����WY
//               tcw...�e�N�X�`���̐؂��蕝
//               tch...�e�N�X�`���̐؂��荂��
//
void Draw_SpriteR(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch, float cx, float cy, float angle);


#endif // !SPRITE_H_
