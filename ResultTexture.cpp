#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "ResultTexture.h"

ID3D11ShaderResourceView* R_Texture::resultTexture=NULL;
bool R_Texture::win = false;
void R_Texture::Init()
{
	VERTEX_3D vertex[4];
	// ����
	vertex[0].position = D3DXVECTOR3(400.0f, 0.0f, 0.0f);    // ���_�̃|�W�V����
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // �ǂ̖ʂ���Ȃ̂�(�@��)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); 
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);			  
	// �E��
	vertex[1].position = D3DXVECTOR3(920, 0.0f, 0.0f);
	vertex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2(1.0f, 0.0f);
	// ����
	vertex[2].position = D3DXVECTOR3(400.0f, SCREEN_HEIGHT, 0.0f);
	vertex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2(0.0f, 1.0f);
	// �E��
	vertex[3].position = D3DXVECTOR3(920, SCREEN_HEIGHT, 0.0f);
	vertex[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4; //���_�o�b�t�@�̐�
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{}; // ���_�o�b�t�@������f�[�^
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &resultTextureVertexBuffer);// �o���オ�������̂�VertexBuffer�ɓ����

	// Shader�ňӖ��킩���G���[���o��Ƃ���cso�̖��O����������
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "unlitTexturePS.cso");

	objPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void R_Texture::Uninit()
{
}

void R_Texture::Update()
{
}
void R_Texture::Draw()
{
	Renderer::GetDeviceContext()->Unmap(resultTextureVertexBuffer, 0);

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
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &resultTextureVertexBuffer,
		&stride, &offset);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &resultTexture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void R_Texture::Load(bool W)
{
	win = W;
	if (win == true)
	{
		// �e�N�X�`���ǂݍ���
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
			"asset/texture/Win.png",
			NULL,
			NULL,
			&resultTexture,
			NULL);
	}
	else
	{
		// �e�N�X�`���ǂݍ���
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
			"asset/texture/Lose.png",
			NULL,
			NULL,
			&resultTexture,
			NULL);
	}
	// �e�N�X�`����ǂݍ��߂����m�F����
	assert(m_Texture);
}
