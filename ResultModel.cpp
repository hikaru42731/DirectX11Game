#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "AnimationModel.h"
#include "ResultModel.h"

void R_Model::Init()
{
	resultModel = new AnimationModel();
	resultModel->Load("asset\\model\\Player\\Idle.fbx");
	resultModel->LoadAnimation("asset\\model\\Player\\Win.fbx", "Win");
	resultModel->LoadAnimation("asset\\model\\Player\\Lose.fbx", "Lose");

	objPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 1.57*2, 0.0f);
	objScale = D3DXVECTOR3(0.02f, 0.02f, 0.02f);

	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "vertexLightingPS.cso");
	animeFrame = 0;
}

void R_Model::Uninit()
{
	resultModel->UnLoadAnimation("Win");
	resultModel->UnLoadAnimation("Lose");
	resultModel->UnLoad();
	delete resultModel;

	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void R_Model::Update()
{
	if (win == true)
	{
		resultModel->Update("Win", "Win", animeRate, animeFrame);
	}
	else
	{
		resultModel->Update("Lose", "Lose", animeRate, animeFrame);
	}
	animeFrame++;
}

void R_Model::Draw()
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
	D3DXMatrixTranslation(&trans, objPosition.x, objPosition.y, objPosition.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	// モデル描画
	resultModel->Draw();
}

