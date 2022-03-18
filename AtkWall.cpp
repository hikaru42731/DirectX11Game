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
#include "AtkWall.h"

class Model* AtkWall::atkWallModel;
void AtkWall::Init()
{
	objScale = D3DXVECTOR3(8.5, 0, 8.5);
	objRotation = D3DXVECTOR3(0, 0, 0);

	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "vertexLightingPS.cso");
}
void AtkWall::Uninit()
{
	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void AtkWall::Update()
{
	objRotation.y += 0.1f;
	if (objScale.y < 40) 
	{
		objScale.y += 1.0f;
	}
	else
	{
		SetDestroy();
	}
}

void AtkWall::Draw()
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
	atkWallModel->Draw();
}

void AtkWall::Load()
{
	// モデル読み込み
	atkWallModel = new Model();
	atkWallModel->Load("asset\\model\\AtkWall\\AtkWall.obj");
}

void AtkWall::UnLoad()
{
	atkWallModel->Unload();
	delete atkWallModel;
}