//==========================================
//
//          Direct3D�̐���
//
//
//==========================================

#ifndef MYDIRECT3D_H_
#define MYDIRECT3D_H_
#include <d3d9.h>

// Direct3D�֘A�̏�����
//
// �߂�l:�������Ɏ��s������false
//
bool MyDirect3D_Init(HWND g_hWnd);

// Direct3D�֘A�̏I������
//
void MyDirect3D_Fin(void);

//
// Direct�f�o�C�X�̎擾
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void);

#endif // !MYDIRECT3D_H_
