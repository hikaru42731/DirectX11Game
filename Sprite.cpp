#include "Sprite.h"
#include <d3dx9.h>
#include "MyDirect3D.h"
#include "Texture.h"

/*-----------------------------------------------------------------------------
   �\����
------------------------------------------------------------------------------*/
// ���_�\����
typedef struct Vertex2D_tag {
	D3DXVECTOR4 Posision;
	D3DCOLOR Color;
	D3DXVECTOR2 TexCord;

}Vertex2D;
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

/*-----------------------------------------------------------------------------
   �O���[�o���ϐ��錾
------------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;// ���_�o�b�t�@�C���^�[�t�F�[�X
LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;  // �C���f�b�N�X�o�b�t�@�[�C���^�[�t�F�[�X
D3DCOLOR g_Color=0xffffffff;

// �X�v���C�g�̏�����
void Init_Sprite(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) {
		return;
	}

	g_Color = (255, 255, 255, 255);

	pDevice->CreateVertexBuffer(
		sizeof(Vertex2D) * 4, // ���_�o�b�t�@�̗ʁi�o�C�g�j
		D3DUSAGE_WRITEONLY, // �g����
		FVF_VERTEX2D,       // FVF
		D3DPOOL_MANAGED,    // �������̊Ǘ����@
		&g_pVertexBuffer,   // �擾�����C���^�[�t�F�[�X�̃A�h���X���L�^���邽�߂̃|�C���^�ւ̃A�h���X
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

// �X�v���C�g�̏I��
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

// �X�v���C�g�̕`��
// *�e�N�X�`���T�C�Y�̃|���S���Ŏw����W�ɕ`��
//
// ����: textureId...�e�N�X�`���Ǘ��ԍ�
//               dx...���WX
//               dy...���WY
//               dw...�|���S���T�C�Y��
//               dh...�|���S���T�C�Y����
//              tcx...�e�N�X�`���؂�����WX
//              tcy...�e�N�X�`���؂�����WY
//               
void Draw_SpriteTestIndex(int textureId, float dx, float dy)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) {
		MessageBox(NULL, "�_�C���N�g�RD�̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
	}

	// �f�o�C�X��FVF�̐ݒ������
	pDevice->SetFVF(FVF_VERTEX2D);

	// �f�o�C�X�Ƀe�N�X�`���̐ݒ������
	pDevice->SetTexture(0, Texture_GetTexture(textureId));
	//pDevice->SetTexture(0, NULL);


	// �|���S���̃T�C�Y�̓e�N�X�`���T�C�Y
	unsigned long w = Texture_GetTextureWidth(textureId);
	unsigned long h = Texture_GetTextureHight(textureId);

	// ���_�f�[�^
	Vertex2D v[] = {
		{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f)},
		{ D3DXVECTOR4(dx + w - 0.5f, dy - 0.5f,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f)},
		{ D3DXVECTOR4(dx - 0.5f, dy + h - 0.5f,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f)},
		{ D3DXVECTOR4(dx + w - 0.5f, dy + h - 0.5f,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,1.0f)},
	};

	// ���_�o�b�t�@�����b�N���ăf�[�^����������
	Vertex2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);// 0������ƑS�̂����b�N

	// �f�[�^����������
	memcpy(pV, v, sizeof(v));

	g_pVertexBuffer->Unlock();

	// �f�o�C�X�ɗ��p���钸�_�o�b�t�@���w�肷��
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2D));

	// �C���f�b�N�X�o�b�t�@�����b�N���ăf�[�^����������
	WORD* pI;
	g_pIndexBuffer->Lock(0, 0, (void**)&pI, 0);
	pI[0] = 0;
	pI[1] = 1;
	pI[2] = 2;
	pI[3] = 1;
	pI[4] = 3;
	pI[5] = 2;
	g_pIndexBuffer->Unlock();
	
	// �f�o�C�X�ɗ��p���钸�_�o�b�t�@���w�肷��
	pDevice->SetIndices(g_pIndexBuffer);

	// �|���S���`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

// �X�v���C�g�̕`��
// *�e�N�X�`���T�C�Y�̃|���S���Ŏw����W�ɕ`��
//
// ����: textureId...�e�N�X�`���Ǘ��ԍ�
//               dx...���WX
//               dy...���WY
//               dw...�|���S���T�C�Y��
//               dh...�|���S���T�C�Y����
//              tcx...�e�N�X�`���؂�����WX
//              tcy...�e�N�X�`���؂�����WY
//               
void Draw_SpriteTest(int textureId, float dx, float dy)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) {
		MessageBox(NULL, "�_�C���N�g�RD�̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
	}

	// �f�o�C�X��FVF�̐ݒ������
	pDevice->SetFVF(FVF_VERTEX2D);

	// �f�o�C�X�Ƀe�N�X�`���̐ݒ������
	pDevice->SetTexture(0, Texture_GetTexture(textureId));
	//pDevice->SetTexture(0, NULL);


	// �|���S���̃T�C�Y�̓e�N�X�`���T�C�Y
	unsigned long w = Texture_GetTextureWidth(textureId);
	unsigned long h = Texture_GetTextureHight(textureId);

	// ���_�f�[�^
	Vertex2D v[] = {
		{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f,1.0f,1.0f),g_Color,D3DXVECTOR2(0.0f,0.0f)},
		{ D3DXVECTOR4(dx + w - 0.5f, dy - 0.5f,1.0f,1.0f),g_Color,D3DXVECTOR2(1.0f,0.0f)},
		{ D3DXVECTOR4(dx - 0.5f, dy + h - 0.5f,1.0f,1.0f),g_Color,D3DXVECTOR2(0.0f,1.0f)},
		{ D3DXVECTOR4(dx + w - 0.5f, dy + h - 0.5f,1.0f,1.0f),g_Color,D3DXVECTOR2(1.0f,1.0f)},
	};

	
	Vertex2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);// 0������ƑS�̂����b�N

	// �f�[�^����������
	memcpy(pV, v, sizeof(v));

	g_pVertexBuffer->Unlock();

	// �f�o�C�X�ɗ��p���钸�_�o�b�t�@���w�肷��
	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex2D));

	// �|���S���`��i�ȈՔŁj
	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);	
}


// �X�v���C�g�̕`��
// *�e�N�X�`���T�C�Y�̃|���S���Ŏw����W�ɕ`��
//
// ����: textureId...�e�N�X�`���Ǘ��ԍ�
//               dx...���WX
//               dy...���WY
//               dw...�|���S���T�C�Y��
//               dh...�|���S���T�C�Y����
//              tcx...�e�N�X�`���؂�����WX
//              tcy...�e�N�X�`���؂�����WY
//               
void Draw_Sprite(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) {
		MessageBox(NULL, "�_�C���N�g�RD�̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
	}

	// �f�o�C�X��FVF�̐ݒ������
	pDevice->SetFVF(FVF_VERTEX2D);

	// �f�o�C�X�Ƀe�N�X�`���̐ݒ������
	pDevice->SetTexture(0, Texture_GetTexture(textureId));
	//pDevice->SetTexture(0, NULL);


	// �|���S���̃T�C�Y�̓e�N�X�`���T�C�Y
	unsigned long w = Texture_GetTextureWidth(textureId);
	unsigned long h = Texture_GetTextureHight(textureId);

	// �e�N�X�`���؂���UV���W
	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;

	// ���_�f�[�^
	Vertex2D v[] = {
		{ D3DXVECTOR4(dx -      0.5f,dy -      0.5f,1.0f,1.0f),g_Color,D3DXVECTOR2(u0,v0)},
		{ D3DXVECTOR4(dw + dx - 0.5f,dy -      0.5f,1.0f,1.0f),g_Color,D3DXVECTOR2(u1,v0)},
		{ D3DXVECTOR4(dx -      0.5f,dh + dy - 0.5f,1.0f,1.0f),g_Color,D3DXVECTOR2(u0,v1)},
		{ D3DXVECTOR4(dw + dx - 0.5f,dh + dy - 0.5f,1.0f,1.0f),g_Color,D3DXVECTOR2(u1,v1)},
	};



	// �|���S���`��i�ȈՔŁj
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));

	g_Color = D3DCOLOR_RGBA(255, 255, 255, 255);
}

// �X�v���C�g�̕`��
// *�e�N�X�`���T�C�Y�̃|���S���Ŏw����W�ɕ`��
//
// ����: textureId...�e�N�X�`���Ǘ��ԍ�
//               dx...���WX
//               dy...���WY
//               dw...�|���S���T�C�Y��
//               dh...�|���S���T�C�Y����
//              tcx...�e�N�X�`���؂�����WX
//              tcy...�e�N�X�`���؂�����WY
//               cx...��]�̒��S���W
//               cy...��]�̒��S���W
//            angle...��]�p�x
//        
void Draw_SpriteR(int textureId, float dx, float dy, float dw, float dh, int tcx, int tcy, int tcw, int tch,float cx,float cy,float angle)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice) {
		MessageBox(NULL, "�_�C���N�g�RD�̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
	}

	// �f�o�C�X��FVF�̐ݒ������
	pDevice->SetFVF(FVF_VERTEX2D);

	// �f�o�C�X�Ƀe�N�X�`���̐ݒ������
	pDevice->SetTexture(0, Texture_GetTexture(textureId));
	//pDevice->SetTexture(0, NULL);


	// �|���S���̃T�C�Y�̓e�N�X�`���T�C�Y
	unsigned long w = Texture_GetTextureWidth(textureId);
	unsigned long h = Texture_GetTextureHight(textureId);

	// �e�N�X�`���؂���UV���W
	float u0 = (float)tcx / w;
	float v0 = (float)tcy / h;
	float u1 = (float)(tcx + tcw) / w;
	float v1 = (float)(tcy + tch) / h;	

	// ���_�f�[�^
	Vertex2D v[] = {
		{ D3DXVECTOR4(   - 0.5f,     - 0.5f ,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(u0,v0)},
		{ D3DXVECTOR4(dw - 0.5f,     - 0.5f ,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(u1,v0)},
		{ D3DXVECTOR4(   - 0.5f,  dh - 0.5f ,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(u0 ,v1)},
		{ D3DXVECTOR4(dw - 0.5f,  dh - 0.5f ,1.0f,1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(u1,v1)},
	};

	
	// ���s�ړ��s��
	D3DXMATRIX mtxTranslationC;
	D3DXMatrixTranslation(&mtxTranslationC, -cx, -cy, 0.0f);

	D3DXMATRIX mtxTranslationI;
	D3DXMatrixTranslation(&mtxTranslationI, cx + dx, cy + dy, 0.0f);

	// ��]
	D3DXMATRIX mtxRotation;
	D3DXMatrixRotationZ(&mtxRotation, angle);

	// �g��E�k��
	D3DXMATRIX mtxScale;
	D3DXMatrixScaling(&mtxScale, 1.0f, 1.0f, 1.0f);

	// �s��̍���
	D3DXMATRIX mtxWorld;
	mtxWorld = mtxTranslationC * mtxScale * mtxRotation * mtxTranslationI;

	// ���W�ϊ�
	for (int i = 0; i < 4; i++) {
		D3DXVec4Transform(&v[i].Posision, &v[i].Posision, &mtxWorld);
	}

	// �|���S���`��i�ȈՔŁj
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}


void Sprite_SetColor(D3DCOLOR color)
{
	g_Color = color;
}