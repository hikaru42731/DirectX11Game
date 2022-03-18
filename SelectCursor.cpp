#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Scene.h"
#include "Input.h"
#include "Audio.h"
#include "AbilityON.h"
#include "GameObject.h"
#include "StageNumber.h"
#include "SelectCursor.h"

std::vector<AbilityON*> AbilityList;
void SelectCursor::Init()
{
	VERTEX_3D vertex[4];
	objPosition = D3DXVECTOR3(0, 190, 0);
	objScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	sizeY = 88;
	sizeX = 230;
	// ����
	vertex[0].position = objPosition;
	vertex[0].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f);
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);
	// �E��
	vertex[1].position = D3DXVECTOR3(objPosition.x + sizeX, objPosition.y, objPosition.z);
	vertex[1].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f);
	vertex[1].texCoord = D3DXVECTOR2(1.0f, 0.0f);
	// ����
	vertex[2].position = D3DXVECTOR3(objPosition.x, objPosition.y + sizeY, objPosition.z);
	vertex[2].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f);
	vertex[2].texCoord = D3DXVECTOR2(0.0f, 1.0f);
	// �E��
	vertex[3].position = D3DXVECTOR3(objPosition.x + sizeX, objPosition.y + sizeY, objPosition.z);
	vertex[3].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f);
	vertex[3].texCoord = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4; //���_�o�b�t�@�̐�
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{}; // ���_�o�b�t�@������f�[�^
	sd.pSysMem = vertex; // ��������������_�f�[�^�ŏ��������Ă���

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &selectCursorVertexBuffer);// �o���オ�������̂�VertexBuffer�ɓ����
	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/Red.png",
		NULL,
		NULL,
		&selectCursorTexture,
		NULL);
	// �e�N�X�`����ǂݍ��߂����m�F����
	assert(selectCursorTexture);

	// Shader�ňӖ��킩���G���[���o��Ƃ���cso�̖��O����������
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "unlitTexturePS.cso");
	line = 0;
	id = 0;
	moveSe = Manager::GetScene()->AddGameObject<Audio>();
	moveSe->Load("asset\\Audio\\Cursor.wav");
	moveSe->SetVolume(1.5f);
	pressSe = Manager::GetScene()->AddGameObject<Audio>();
	pressSe->Load("asset\\Audio\\HitSword.wav");
	pressSe->SetVolume(1.5f);
}

void SelectCursor::Uninit()
{
	selectCursorVertexBuffer->Release();
	selectCursorTexture->Release();
	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void SelectCursor::Update()
{

	Scene* scene = Manager::GetScene();
	AbilityList = scene->GetGameObjects<AbilityON>();
	StageNumber* stageNum = scene->GetGameObject<StageNumber>();
	if (Input::GetKeyTrigger('W'))	//�����
	{
		moveSe->Play(false);
		if (id != 0 && id != 6)
		{
			id--;
		}
	}
	if (Input::GetKeyTrigger('S'))	//������
	{ 
		moveSe->Play(false);
		if (id != 5 && id != 11)
		{
			id++;
		}
	}
	if (Input::GetKeyTrigger('A'))	//������
	{
		moveSe->Play(false);
		if (line == 1)
		{
			id -= 6;
		}
	}
	if (Input::GetKeyTrigger('D'))	//�E����
	{
		moveSe->Play(false);
		if (line == 0 && id != 6)
		{
			id += 6;
		}
	}

	if (id >= 6)line = 1;
	else line = 0;

	if (id < 11) 
	{
		if (Input::GetKeyTrigger('L'))	//����L�[
		{
			pressSe->Play(false);
			AbilityList[id]->SetON(1);
		}
	}
	if (id == 11)
	{
		if (Input::GetKeyTrigger('L'))	//����L�[
		{
			pressSe->Play(false);
			stageNum->AddNumber(1);
		}
	}

	objPosition.x = line * 640.0f;
	objPosition.y = 190.0f + (sizeY * (id % 6));
}

void SelectCursor::Draw()
{
	// ���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(selectCursorVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
	// ����
	vertex[0].position = objPosition;
	vertex[0].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 0.5f);
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);
	// �E��
	vertex[1].position = D3DXVECTOR3(objPosition.x + sizeX, objPosition.y, objPosition.z);
	vertex[1].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 0.5f);
	vertex[1].texCoord = D3DXVECTOR2(1.0f, 0.0f);
	// ����
	vertex[2].position = D3DXVECTOR3(objPosition.x, objPosition.y + sizeY, objPosition.z);
	vertex[2].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 0.5f);
	vertex[2].texCoord = D3DXVECTOR2(0.0f, 1.0f);
	// �E��
	vertex[3].position = D3DXVECTOR3(objPosition.x + sizeX, objPosition.y + sizeY, objPosition.z);
	vertex[3].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 0.5f);
	vertex[3].texCoord = D3DXVECTOR2(1.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(selectCursorVertexBuffer, 0);
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(objVertexLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(objVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(objPixelShader, NULL, 0);

	//�}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &selectCursorVertexBuffer,
		&stride, &offset);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &selectCursorTexture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}
