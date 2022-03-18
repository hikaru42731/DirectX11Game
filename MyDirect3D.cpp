//==========================================
//
//           Direct3Dの制御
//
//
//==========================================
#include "MyDirect3D.h"
#include "Config.h"

/*-----------------------------------------------------------------------------
   グローバル変数
------------------------------------------------------------------------------*/
static LPDIRECT3D9 g_pD3D = NULL;  // Direct3Dインターフェース
static LPDIRECT3DDEVICE9 g_pDevice = NULL;  // Direct3Dデバイスインターフェース


// Direct3D関連の初期化
//
// 戻り値:初期化に失敗したらfalse
//
bool MyDirect3D_Init(HWND g_hWnd)
{
	// Direct3Dインターフェースの取得
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL) {
		MessageBox(NULL, "Direct3Dのインターフェースの作成に失敗しました", "エラー", MB_OK);
		return 0;
	}
	/*Direect3Dデバイスの取得*/

	// デバイスの取得に必要な情報構造体の作成
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.BackBufferWidth = SCREEN_WIDTH;                       // バックバッファの横幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;                      // バックバッファの縦幅
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;                    // バックバッファのフォーマット
	d3dpp.BackBufferCount = 1;                                  //バックバッファの数 
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;                   // スワップ方法の設定
	d3dpp.Windowed = TRUE;                                      // ウィンドウ OR フルスクリーン
	d3dpp.EnableAutoDepthStencil = TRUE;                        // 深度バッファ・ステンシルバッファの使用
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                  // 深度バッファ・ステンシルバッファのフォーマット
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // フルスクリーン時のリフレッシュレートの設定
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;   // リフレッシュレートとPresent処理の関係
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // CPUの処理速度を参照

	// Direct3Dデバイスの取得

	HRESULT hr;
	hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		g_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pDevice);

	// HRESURT型変数は成功や失敗が値として入っているので、
	// 失敗や成功を判定する場合はマクロを使用する　FAILED(hr) SUCCEEDED(hr)
	if (FAILED(hr)) {
		// デバイスの取得に失敗
		MessageBox(NULL, "Dirct3Dデバイスの取得に失敗しました", "エラー", MB_OK);

		return false;
	}

	// UVアドレッシングモード
	g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	g_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
	g_pDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, D3DCOLOR_RGBA(255, 255, 255, 255));

	// テクスチャフィルタリングの設定

	// D3DPT_TRIANGLELIST
	// D3DPT_TRIANGLESTRIP3
	// D3DPT_POINTLIST
	// D3DPT_LINELIST
	// D3DPT_LINESTRIP
	// D3DPT_TRIANGLEFAN
	g_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	//g_pDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);// D3DTEXF_ANISOTROPIC を使った場合
	// D3DSAMP_MAGFILTER  ->　大きくした時にどんなどんなフィルター（補間）をかけるか
	// D3DSAMP_MINFILTER  ->　小さくした時にどんなどんなフィルター（補間）をかけるか
	// D3DSAMP_MIPFILTER  ->  

	// アルファブレンドの設定
	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	// 半透明・・・　描画色 － 今から描画するRGB * 今から描画するα * 画面の画面のRGBA
	g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 頂点カラーとテクスチャのブレンド設定
	/*--------------------------------------------------------------------
	// デフォルト　　D3DTA_CURRENT　=>  D3DTA_DIFFUSE になっているはず

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

// Direct3D関連の終了処理
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

// デバイスの取得
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void)
{
	return g_pDevice;
}
