#include "Debug_font.h"
#include "Texture.h"
#include "Sprite.h"
#include<string.h>

#define DEBUGFONT_WIDTH 32
#define DEBUGFONT_HIGHT 64
#define DEBUGFONT_ONELINE_CHARCOUNT 16

static int g_TextureId = TEXTURE_INVLIT_ID;

// �f�o�b�O�t�H���g���W���[���̏�����
void DebugFont_Init(void)
{
	g_TextureId = Texture_SetTextureLoadFile("Asset/debug_font_32x64.png");

	// �e�N�X�`���̓ǂݍ���
	/*if (Texture_Load() > 0) {
		MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
	}*/
}

// �f�o�b�O�t�H���g�̕\��
// 
// ����  pString...�f�o�b�O������ւ̃|�C���^
//         dx,dy...�f�o�b�O������̕`����W
void DebugFont_Draw(const char* pString, float dx, float dy)
{
	for (int i = 0; i < strlen(pString); i++) {

		int offset = pString[i] - ' ';
		int tcx = offset % DEBUGFONT_ONELINE_CHARCOUNT * DEBUGFONT_WIDTH;
		int tcy = offset / DEBUGFONT_ONELINE_CHARCOUNT * DEBUGFONT_HIGHT;

		Draw_Sprite(g_TextureId, dx + i * DEBUGFONT_WIDTH, dy, 16.0f, 32.0f, tcx,tcy, DEBUGFONT_WIDTH, DEBUGFONT_HIGHT);
	}
}

// �f�o�b�O�t�H���g���W���[���̏I��
void DebugFont_Fin(void)
{
	Texture_Release(&g_TextureId, 1);
}
