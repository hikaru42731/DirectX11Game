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
#include "Guard.h"

#define EXISTTIME (35)
class Model* Guard::guardModel;
void Guard::Init()
{

	guardPlayer = Manager::GetScene()->GetGameObject<Player>();
	
	objScale = D3DXVECTOR3(2.0, 2.0, 2.0);
	cnt = 0;

	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "vertexLightingPS.cso");

}
void Guard::Uninit()
{
	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void Guard::Update()
{
	cnt++;

	if (guardPlayer != NULL) {
		guardForward = guardPlayer->GetForward();
		D3DXVec3Normalize(&guardForward, &guardForward);
		D3DXVECTOR3 guardpos = guardPlayer->GetPosition();
		guardpos.y = guardPlayer->GetPosition().y + 2.0f;
		objPosition = guardpos - guardForward * 2;
		objRotation.y = guardPlayer->GetRotation().y + 1.57f;
	}

	objScale -= D3DXVECTOR3(0.03, 0.03, 0.03);
	

	if (cnt > EXISTTIME)
	{
		SetDestroy();
	}
}

void Guard::Draw()
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
	guardModel->Draw();
}

void Guard::Load()
{
	// モデル読み込み
	guardModel = new Model();
	guardModel->Load("asset\\model\\Guard\\Guard.obj");
}

void Guard::UnLoad()
{
	guardModel->Unload();
	delete guardModel;
}