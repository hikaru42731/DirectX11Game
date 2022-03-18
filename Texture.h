//======================================================
//
//             �e�N�X�`���Ǘ�[Texture.h]
//
//
//======================================================

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <d3d9.h>

/*------------------------------------
  �萔��`
-------------------------------------*/
#define TEXTURE_FILENAME_MAX (64) // �e�N�X�`���t�@�C�����ő啶����
#define TEXTURE_MAX (1024)        // �e�N�X�`���Ǘ��ő吔
#define TEXTURE_INVLIT_ID (-1)    // �����ȃe�N�X�`���Ǘ��ԍ�
#define TEXTURE_INVLIT_ID1 (-1)

// �e�N�X�`���Ǘ����W���[���̏�����
void Texture_Init(void);

// �e�N�X�`���Ǘ����W���[���̏I��
void Texture_Fin(void);

// �e�N�X�`���t�@�C���̓ǂݍ��ݗ\��
// 
// ����  pFilname�E�E�E�t�@�C����
//
// �߂�l�@�e�N�X�`���Ǘ��ԍ�
//            �ő�Ǘ����𒴂��Ă����ꍇ��INVALIT_TEXTURE_ID
//
int Texture_SetTextureLoadFile(const char* pFileName);

// �e�N�X�`���̓ǂݍ���
//
// �\�񂳂�Ă���e�N�X�`����ǂݍ���
//
// �߂�l�@�@�ǂݍ��܂�Ȃ������t�@�C����
//              �܂��͑��̕s������������ꍇ�� -1
//
int Texture_Load(void);

// �e�N�X�`���̕������
//
// �����@textureids[]�E�E�E���������textureid��������int�^�z��̐擪�A�h���X
// �@�@�@�@�@count�@ �E�E�E����������e�N�X�`���̐�
//
void Texture_Release(int textureids[], int count);

// �e�N�X�`���̑S���
void Texture_AllRelease(void);

// �e�N�X�`���̕��̎擾
//
// �����@textureid�E�E�E�e�N�X�`���Ǘ��ԍ�
//
// �߂�l�@�e�N�X�`���̕�
//
unsigned long Texture_GetTextureWidth(int textureid);

// �e�N�X�`���̍����̎擾
//
// �����@textureid�E�E�E�e�N�X�`���Ǘ��ԍ�
//
// �߂�l�@�e�N�X�`���̍���
//
unsigned long Texture_GetTextureHight(int textureid);

// �e�N�X�`���C���^�[�t�F�[�X�|�C���^�̎擾
//
// �����@�e�N�X�`���Ǘ��ԍ�
//
// �߂�l�@�e�N�X�`���C���^�[�t�F�[�X�|�C���^
//            �ǂݍ��߂ĂȂ��܂��͕s���ȊǗ��ԍ��������ꍇ��NULL
//
LPDIRECT3DTEXTURE9 Texture_GetTexture(int textureid);

#endif // TEXTURE_H_