#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "Scene.h"
#include "Model.h"
#include "Sky.h"

Model* SkyModel;
void Sky::Init()
{
	// モデル読み込み
	SkyModel = new Model();
	SkyModel->Load("asset\\model\\Skydome\\skydome.obj");

	objPosition = D3DXVECTOR3(0.0f, 0.0, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 2.5f, 0.0f);
	objScale = D3DXVECTOR3(500.0f, 400.0f, 500.0f);

	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "vertexLightingPS.cso");
}

void Sky::Uninit()
{
	SkyModel->Unload();
	delete SkyModel;

	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void Sky::Update()
{
	objRotation.y += 0.0005f;
}

void Sky::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(objVertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(objVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(objPixelShader, NULL, 0);
	

	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, objScale.x, objScale.y, objScale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, objRotation.y, objRotation.x, objRotation.z);
	D3DXMatrixTranslation(&trans, objPosition.x, objPosition.y - 100, objPosition.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	// モデル描画
	SkyModel->Draw();
}


