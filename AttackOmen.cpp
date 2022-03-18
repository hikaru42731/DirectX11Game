#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Scene.h"
#include "Model.h"
#include "Enemy.h"
#include "Explosion.h"
#include "AnimationModel.h"
#include "Camera.h"
#include "Player.h"
#include "AttackOmen.h"

class Model* Omen::omenModel;
void Omen::Init()
{
	objRotation = D3DXVECTOR3(0, 0, 0);

	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "vertexLightingPS.cso");

}
void Omen::Uninit()
{
	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void Omen::Update()
{
	objRotation.y += 0.1f;
}

void Omen::Draw()
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
	omenModel->Draw();
}

void Omen::Load()
{
	// モデル読み込み
	omenModel = new Model();
	omenModel->Load("asset\\model\\Omen\\Omen.obj");
}

void Omen::UnLoad()
{
	omenModel->Unload();
	delete omenModel;
}