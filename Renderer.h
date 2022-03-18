#pragma once

struct VERTEX_3D
{
    D3DXVECTOR3 position;
    D3DXVECTOR3 normal;
    D3DXVECTOR4 diffuse;
    D3DXVECTOR2 texCoord;
};



struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	float		Dummy[3];
};



struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	D3DXVECTOR4	Direction;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Ambient;
};


// DirectXÇÃï`âÊÇÃä«óù
class Renderer
{
private:
	// ñΩñºãKë•
	static D3D_FEATURE_LEVEL       featureLevel; 

	static ID3D11Device*           renderDevice;
	static ID3D11DeviceContext*    renderDeviceContext;
	static IDXGISwapChain*         renderSwapChain;
	static ID3D11RenderTargetView* renderTargetView;
	static ID3D11DepthStencilView* renderDepthStencilView;

	static ID3D11Buffer*			worldBuffer;
	static ID3D11Buffer*			viewBuffer;
	static ID3D11Buffer*			projectionBuffer;
	static ID3D11Buffer*			materialBuffer;
	static ID3D11Buffer*			lightBuffer;


	static ID3D11DepthStencilState* depthStateEnable;
	static ID3D11DepthStencilState* depthStateDisable;




public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void SetDepthEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(D3DXMATRIX* WorldMatrix);
	static void SetViewMatrix(D3DXMATRIX* ViewMatrix);
	static void SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);

	static ID3D11Device* GetDevice( void ){ return renderDevice; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return renderDeviceContext; }



	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);


};
