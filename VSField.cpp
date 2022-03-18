#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "Scene.h"
#include "Model.h"
#include "VSField.h"


void VSField::Init()
{
	// モデル読み込み
	vsFieldModel = new Model();
	vsFieldModel->Load("asset\\model\\Stage\\Stage.obj");

	objPosition = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "vertexLightingPS.cso");
}


void VSField::Uninit()
{
	vsFieldModel->Unload();
	delete vsFieldModel;

	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void VSField::Update()
{
	
}

void VSField::Draw()
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
	vsFieldModel->Draw();
}

