#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "Scene.h"
#include "Player.h"
#include "Model.h"
#include "Locus.h"

class Model* Locus::locusSideModel;
class Model* Locus::locusVerticalModel;
class Model* Locus::locusDirgonalModel;
class Model* Locus::locusLongModel;
class Model* Locus::locusDiveModel;
void Locus::Init()
{
	objPosition = D3DXVECTOR3(0.0f, 0.5f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(2.0, 2.0, 2.0);

	locusPlayer = Manager::GetScene()->GetGameObject<Player>();
	guardForward = locusPlayer->GetForward();
	D3DXVec3Normalize(&guardForward, &guardForward);
	playerPosition = locusPlayer->GetPosition();
	vec = 0;
	forwardPow = 1;
	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "vertexLightingPS.cso");
	
}

void Locus::Uninit()
{
	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void Locus::Update()
{
	objPosition = locusPlayer->GetPosition() - locusPlayer->GetForward() * forwardPow;
	objPosition.y = locusPlayer->GetPosition().y + 2;
	
	if (rotate == true)
	{
		rotateSpeed +=0.3f;
		
		if (rotateSpeed > 3.0)
		{
			rotate = false;
			rotateSpeed = 0;
		}
		switch(vec)
		{
		case 0:
			objRotation.z -= rotateSpeed * 0.2f;	//縦回転
			objRotation.y = locusPlayer->GetRotation().y + 1.57f;
			break;
		case 1:
			objRotation.y += rotateSpeed * 0.2f;	//横回転
			break;
		case 2:
			objRotation.z -= rotateSpeed * 0.1f;
			objRotation.y -= rotateSpeed * 0.1f;	//斜め
			break;
		case 3:
			objScale = D3DXVECTOR3(2,2,2) * (rotateSpeed * 1.2f);
			break;
		case 4:
			objRotation.z -= rotateSpeed * 0.2f;	//縦回転
			objRotation.y = locusPlayer->GetRotation().y+1.57f;
			break;
		default:
			break;
		}
	}
	else
	{
		objRotation.y = locusPlayer->GetForward().y+1.57f;
		objScale = D3DXVECTOR3(2, 2, 2);
		rotateSpeed = 0.1f;
		forwardPow = 1;
	}

}

void Locus::Draw()
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
	if (rotate == true) {
		switch (vec)
		{
		case 0:
			locusVerticalModel->Draw();
			break;
		case 1:
			locusSideModel->Draw();
			break;
		case 2:
			locusDirgonalModel->Draw();
			break;
		case 3:
			locusLongModel->Draw();
			break;
		case 4:
			locusDiveModel->Draw();
			break;
		default:
			break;
		}
		
	}
	
}

void Locus::Load()
{
	// モデル読み込み
	locusSideModel = new Model();
	locusSideModel->Load("asset\\model\\Locus\\LocusSide.obj");
	locusVerticalModel = new Model();
	locusVerticalModel->Load("asset\\model\\Locus\\LocusVertical.obj");
	locusDirgonalModel = new Model();
	locusDirgonalModel->Load("asset\\model\\Locus\\LocusDiagonal.obj");
	locusLongModel = new Model();
	locusLongModel->Load("asset\\model\\Locus\\LongRange.obj");
	locusDiveModel = new Model();
	locusDiveModel->Load("asset\\model\\Locus\\Dive.obj");
}

void Locus::UnLoad()
{
	locusSideModel->Unload();
	delete locusSideModel;
	locusVerticalModel->Unload();
	delete locusVerticalModel;
	locusDirgonalModel->Unload();
	delete locusDirgonalModel;
	locusLongModel->Unload();
	delete locusLongModel;
	locusDiveModel->Unload();
	delete locusDiveModel;
}
