#define _CRT_SECURE_NO_WARNINGS
#include "Texture.h"

#include <d3dx9.h>
#include <string.h>
#include "MyDirect3D.h"
#include"debugPrintf.h"


/*--------------------------------
  �\���̐錾
---------------------------------*/
typedef struct Texture_tag {
	LPDIRECT3DTEXTURE9 pTexture;
	unsigned long width;         // �e�N�X�`���̕�
	unsigned long hight;         // �e�N�X�`���̍���
	char FileName[TEXTURE_FILENAME_MAX];
}Texture;

/*--------------------------------
  �O���[�o���ϐ��錾
---------------------------------*/
static Texture g_Texture[TEXTURE_MAX];

// �e�N�X�`���Ǘ����W���[���̏�����
void Texture_Init(void)
{
	// �e�N�X�`���Ǘ��e�[�u���̏�����
	for (int i = 0; i < TEXTURE_MAX; i++) {
		g_Texture[i].pTexture = NULL;
		g_Texture[i].width = 0;
		g_Texture[i].width = 0;
		g_Texture[i].FileName[0] = 0;
	}
}

// �e�N�X�`���Ǘ����W���[���̏I��
void Texture_Fin(void)
{
	for (int i = TEXTURE_MAX; i >= 0; i--) {

	}
}


// �e�N�X�`���t�@�C���̓ǂݍ��ݗ\��
// 
// ����  pFilname�E�E�E�t�@�C����
//
// �߂�l�@�e�N�X�`���Ǘ��ԍ�
//            �ő�Ǘ����𒴂��Ă����ꍇ��INVALIT_TEXTURE_ID
//
int Texture_SetTextureLoadFile(const char* pFileName)
{
	// �t�@�C��������
	for (int i = 0; i < TEXTURE_MAX; i++) {
		// �e�[�u�����󂢂Ă�����X�L�b�v
		if (g_Texture[i].FileName[0] == 0) {
			continue;
		}

		// ���Ɏw��̃t�@�C�����\�񂳂�Ă��邩?
		if (strcmp(pFileName, g_Texture[i].FileName) == 0) {
			// �\��ς�
			return i;
		}
	}

	// �V�K�\��
	for (int i = 0; i < TEXTURE_MAX; i++) {
		// �e�[�u�����g�p����������X�L�b�v
		if (g_Texture[i].FileName[0] != 0) {
			continue;
		}
		strcpy(g_Texture[i].FileName, pFileName);

		return i;
	}

	return TEXTURE_INVLIT_ID;
}

// �e�N�X�`���̓ǂݍ���
//
// �\�񂳂�Ă���e�N�X�`����ǂݍ���
//
// �߂�l�@�@�ǂݍ��܂�Ȃ������t�@�C����
//
int Texture_Load(void)
{
	int err_count = 0;
	
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (pDevice == NULL) {
		MessageBox(NULL, "�f�o�C�X�̎擾�Ɏ��s���܂���", "�G���[", MB_OK);
	}

	for (int i = 0; i < TEXTURE_MAX; i++) {
		if (g_Texture[i].FileName[0] == 0) {
			// ��̃e�[�u���Ȃ̂ŃX�L�b�v
			continue;
		}

		if (g_Texture[i].pTexture != NULL) {
			// ���ɓǂݍ��܂�Ă���
			continue;
		}

		// �e�N�X�`���̓ǂݍ���
		HRESULT hr = D3DXCreateTextureFromFile(pDevice, g_Texture[i].FileName, &g_Texture[i].pTexture);
		if (FAILED(hr)) {
			Debug_Printf("�ǂݍ��ߖ��������t�@�C��:[%s]\n", g_Texture[i].FileName);
			err_count++;
		}
		else {
			// �e�N�X�`���̓ǂݍ���
			D3DXIMAGE_INFO info;
			D3DXGetImageInfoFromFile(g_Texture[i].FileName, &info);
			g_Texture[i].width = info.Width;
			g_Texture[i].hight = info.Height;
		}
	}

	return err_count;
}

// �e�N�X�`���̕������
//
// �����@textureids[]�E�E�E���������textureid��������int�^�z��̐擪�A�h���X
// �@�@�@�@�@count�@ �E�E�E����������e�N�X�`���̐�
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

// �e�N�X�`���̑S���
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

// �e�N�X�`���̕��̎擾
//
// �����@textureid�E�E�E�e�N�X�`���Ǘ��ԍ�
//
// �߂�l�@�e�N�X�`���̕�
//
unsigned long Texture_GetTextureWidth(int textureid)
{
	return g_Texture[textureid].width;
}

// �e�N�X�`���̍����̎擾
//
// �����@textureid�E�E�E�e�N�X�`���Ǘ��ԍ�
//
// �߂�l�@�e�N�X�`���̍���
//
unsigned long Texture_GetTextureHight(int textureid)
{
	return g_Texture[textureid].hight;
}

// �e�N�X�`���C���^�[�t�F�[�X�|�C���^�̎擾
//
// �����@�e�N�X�`���Ǘ��ԍ�
//
// �߂�l�@�e�N�X�`���C���^�[�t�F�[�X�|�C���^
//            �ǂݍ��߂ĂȂ��܂��͕s���ȊǗ��ԍ��������ꍇ��NULL
//
LPDIRECT3DTEXTURE9 Texture_GetTexture(int textureid)
{
	return g_Texture[textureid].pTexture;
}