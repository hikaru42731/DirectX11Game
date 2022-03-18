#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "Scene.h"
#include "Model.h"
#include "VSField.h"


void VSField::Init()
{
	// ���f���ǂݍ���
	vsFieldModel = new Model();
	vsFieldModel->Load("asset\\model\\Stage\\Stage.obj");

	objPosition = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

	// Shader�ňӖ��킩���G���[���o��Ƃ���cso�̖��O����������
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
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(objVertexLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(objVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(objPixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, objScale.x, objScale.y, objScale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, objRotation.y, objRotation.x, objRotation.z);
	D3DXMatrixTranslation(&trans, objPosition.x, objPosition.y - 100, objPosition.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	// ���f���`��
	vsFieldModel->Draw();
}

