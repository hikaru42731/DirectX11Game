#include "Main.h"
#include "Renderer.h"
#include "AbilityON.h"

void AbilityON::Init()
{
	VERTEX_3D vertex[4];
	objPosition = D3DXVECTOR3(0, 0, 0);
	posY = 190;
	sizeY = 87;
	sizeX = 157;
	// ����
	vertex[0].position = objPosition;
	vertex[0].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);
	// �E��
	vertex[1].position = D3DXVECTOR3(objPosition.x + sizeX, objPosition.y, objPosition.z);
	vertex[1].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2(1.0f, 0.0f);
	// ����
	vertex[2].position = D3DXVECTOR3(objPosition.x, objPosition.y + sizeY, objPosition.z);
	vertex[2].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2(0.0f, 1.0f);
	// �E��
	vertex[3].position = D3DXVECTOR3(objPosition.x + sizeX, objPosition.y + sizeY, objPosition.z);
	vertex[3].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4; //���_�o�b�t�@�̐�
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{}; // ���_�o�b�t�@������f�[�^
	sd.pSysMem = vertex; // ��������������_�f�[�^�ŏ��������Ă���

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &abilityOnVertexBuffer);// �o���オ�������̂�VertexBuffer�ɓ����
	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/ON.png",
		NULL,
		NULL,
		&onTexture,
		NULL);
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/OFF.png",
		NULL,
		NULL,
		&offTexture,
		NULL);
	abilityTexture = onTexture;
	// �e�N�X�`����ǂݍ��߂����m�F����
	assert(abilityTexture);

	// Shader�ňӖ��킩���G���[���o��Ƃ���cso�̖��O����������
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "unlitTexturePS.cso");
	on = 0;
	objScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void  AbilityON::Uninit()
{
	abilityOnVertexBuffer->Release();
		abilityTexture->Release();
	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void   AbilityON::Update()
{
	if (on == 0) { abilityTexture = onTexture; }
	else { abilityTexture = offTexture; }
}

void   AbilityON::Draw()
{
	// ���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(abilityOnVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
	// ����
	vertex[0].position = objPosition;
	vertex[0].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);
	// �E��
	vertex[1].position = D3DXVECTOR3(objPosition.x + sizeX, objPosition.y, objPosition.z);
	vertex[1].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2(1.0f, 0.0f);
	// ����
	vertex[2].position = D3DXVECTOR3(objPosition.x, objPosition.y + sizeY, objPosition.z);
	vertex[2].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2(0.0f, 1.0f);
	// �E��
	vertex[3].position = D3DXVECTOR3(objPosition.x + sizeX, objPosition.y + sizeY, objPosition.z);
	vertex[3].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = D3DXVECTOR2(1.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(abilityOnVertexBuffer, 0);
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
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &abilityOnVertexBuffer,
		&stride, &offset);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &abilityTexture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}
