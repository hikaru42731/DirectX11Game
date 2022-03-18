#include "Main.h"
#include "Renderer.h"
#include "SelectBack.h"


void SelectBack::Init()
{
	VERTEX_3D vertex[4];
	// ����
	vertex[0].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);
	// �E��
	vertex[1].position = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	vertex[1].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2(1.0f, 0.0f);
	// ����
	vertex[2].position = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	vertex[2].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2(0.0f, 1.0f);
	// �E��
	vertex[3].position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	vertex[3].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4; //���_�o�b�t�@�̐�
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{}; // ���_�o�b�t�@������f�[�^
	sd.pSysMem = vertex; // ��������������_�f�[�^�ŏ��������Ă���

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &selectBackVertexBuffer);// �o���オ�������̂�VertexBuffer�ɓ����

	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/AbilitySelector.png",
		NULL,
		NULL,
		&selectBackTexture,
		NULL);
	// �e�N�X�`����ǂݍ��߂����m�F����
	assert(selectBackTexture);

	// Shader�ňӖ��킩���G���[���o��Ƃ���cso�̖��O����������
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "unlitTexturePS.cso");

}

void  SelectBack::Uninit()
{
	selectBackVertexBuffer->Release();
	selectBackTexture->Release();

	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void   SelectBack::Update()
{

}

void   SelectBack::Draw()
{
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
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &selectBackVertexBuffer,
		&stride, &offset);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &selectBackTexture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}
