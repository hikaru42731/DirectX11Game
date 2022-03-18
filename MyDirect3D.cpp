//==========================================
//
//           Direct3D�̐���
//
//
//==========================================
#include "MyDirect3D.h"
#include "Config.h"

/*-----------------------------------------------------------------------------
   �O���[�o���ϐ�
------------------------------------------------------------------------------*/
static LPDIRECT3D9 g_pD3D = NULL;  // Direct3D�C���^�[�t�F�[�X
static LPDIRECT3DDEVICE9 g_pDevice = NULL;  // Direct3D�f�o�C�X�C���^�[�t�F�[�X


// Direct3D�֘A�̏�����
//
// �߂�l:�������Ɏ��s������false
//
bool MyDirect3D_Init(HWND g_hWnd)
{
	// Direct3D�C���^�[�t�F�[�X�̎擾
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL) {
		MessageBox(NULL, "Direct3D�̃C���^�[�t�F�[�X�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return 0;
	}
	/*Direect3D�f�o�C�X�̎擾*/

	// �f�o�C�X�̎擾�ɕK�v�ȏ��\���̂̍쐬
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.BackBufferWidth = SCREEN_WIDTH;                       // �o�b�N�o�b�t�@�̉���
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;                      // �o�b�N�o�b�t�@�̏c��
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;                    // �o�b�N�o�b�t�@�̃t�H�[�}�b�g
	d3dpp.BackBufferCount = 1;                                  //�o�b�N�o�b�t�@�̐� 
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;                   // �X���b�v���@�̐ݒ�
	d3dpp.Windowed = TRUE;                                      // �E�B���h�E OR �t���X�N���[��
	d3dpp.EnableAutoDepthStencil = TRUE;                        // �[�x�o�b�t�@�E�X�e���V���o�b�t�@�̎g�p
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                  // �[�x�o�b�t�@�E�X�e���V���o�b�t�@�̃t�H�[�}�b�g
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // �t���X�N���[�����̃��t���b�V�����[�g�̐ݒ�
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;   // ���t���b�V�����[�g��Present�����̊֌W
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // CPU�̏������x���Q��

	// Direct3D�f�o�C�X�̎擾

	HRESULT hr;
	hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		g_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pDevice);

	// HRESURT�^�ϐ��͐����⎸�s���l�Ƃ��ē����Ă���̂ŁA
	// ���s�␬���𔻒肷��ꍇ�̓}�N�����g�p����@FAILED(hr) SUCCEEDED(hr)
	if (FAILED(hr)) {
		// �f�o�C�X�̎擾�Ɏ��s
		MessageBox(NULL, "Dirct3D�f�o�C�X�̎擾�Ɏ��s���܂���", "�G���[", MB_OK);

		return false;
	}

	// UV�A�h���b�V���O���[�h
	g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
	g_pDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, D3DCOLOR_RGBA(255, 255, 255, 255));

	// �e�N�X�`���t�B���^�����O�̐ݒ�

	// D3DPT_TRIANGLELIST
	// D3DPT_TRIANGLESTRIP3
	// D3DPT_POINTLIST
	// D3DPT_LINELIST
	// D3DPT_LINESTRIP
	// D3DPT_TRIANGLEFAN
	g_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	//g_pDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);// D3DTEXF_ANISOTROPIC ���g�����ꍇ
	// D3DSAMP_MAGFILTER  ->�@�傫���������ɂǂ�Ȃǂ�ȃt�B���^�[�i��ԁj�������邩
	// D3DSAMP_MINFILTER  ->�@�������������ɂǂ�Ȃǂ�ȃt�B���^�[�i��ԁj�������邩
	// D3DSAMP_MIPFILTER  ->  

	// �A���t�@�u�����h�̐ݒ�
	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	// �������E�E�E�@�`��F �| ������`�悷��RGB * ������`�悷�郿 * ��ʂ̉�ʂ�RGBA
	g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���_�J���[�ƃe�N�X�`���̃u�����h�ݒ�
	/*--------------------------------------------------------------------
	// �f�t�H���g�@�@D3DTA_CURRENT�@=>  D3DTA_DIFFUSE �ɂȂ��Ă���͂�

	g_pDevice->SetTextureStageState(0, D3DTSS_COLORARG0, D3DTA_CURRENT);
	g_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	g_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG0, D3DTA_CURRENT);
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	----------------------------------------------------------------------*/

	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	return true;
}

// Direct3D�֘A�̏I������
//
void MyDirect3D_Fin(void)
{
	if (g_pDevice) {
		g_pDevice->Release();
		g_pDevice = NULL;
	}

	if (g_pD3D) {
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

// �f�o�C�X�̎擾
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void)
{
	return g_pDevice;
}
