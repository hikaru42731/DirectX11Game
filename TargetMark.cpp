#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Scene.h"
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "TargetMark.h"


void TargetMark::Init()
{
	size = 2.5f;

	VERTEX_3D vertex[4];
	// ����
	vertex[0].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ���_�̃|�W�V����
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // �ǂ̖ʂ���Ȃ̂�(�@��)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); //
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);
	// �E��
	vertex[1].position = D3DXVECTOR3(0.0f + size, 0.0f, 0.0f);
	vertex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2(1.0f, 0.0f);
	// ����
	vertex[2].position = D3DXVECTOR3(0.0f, 0.0f + size, 0.0f);
	vertex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2(0.0f, 1.0f);
	// �E��
	vertex[3].position = D3DXVECTOR3(0.0f + size, 0.0f + size, 0.0f);
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

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &targetMarkVertexBuffer);// �o���オ�������̂�VertexBuffer�ɓ����

	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/Target.png",
		NULL,
		NULL,
		&targetMarkTexture,
		NULL);

	// �e�N�X�`����ǂݍ��߂����m�F����
	assert(targetMarkTexture);

	// Shader�ňӖ��킩���G���[���o��Ƃ���cso�̖��O����������
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "unlitTexturePS.cso");

	objPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	Scene* scene = Manager::GetScene();
	cam = scene->GetGameObject<Camera>();
	targetMarkPlayer = scene->GetGameObject<Player>();
	targetMarkenemy = scene->GetGameObject<Enemy>();
	num = 0;
}

void TargetMark::Uninit()
{
	targetMarkVertexBuffer->Release();
	targetMarkTexture->Release();

	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void TargetMark::Update()
{
	if (cam->GetMode() == DEFAULT) {
		if (cam->GetTarget() != NULL)
		{
			num = 4;
			if (cam->TargetON())
			{
				Enemy* target = cam->GetTarget();
				D3DXVECTOR3 targetpos = target->GetPosition();
				objPosition = targetpos - target->GetForward();
				objPosition.y = target->GetPosition().y + 1.5f;
				color = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
			}
			else
			{
				Enemy* target = cam->GetExist();
				D3DXVECTOR3 targetpos = target->GetPosition();
				objPosition = targetpos - target->GetForward();
				objPosition.y = target->GetPosition().y + 1.5f;
				color = D3DXVECTOR4(0.0f, 0.0f, 0.5f, 0.5f);
			}
		}
		else
		{
			num = 0;
		}
	}
}

void TargetMark::Draw()
{
	// ���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(targetMarkVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// ����
	vertex[0].position = D3DXVECTOR3(-size, size / 2, 0.0f);    // ���_�̃|�W�V����
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // �ǂ̖ʂ���Ȃ̂�(�@��)
	vertex[0].diffuse = color;
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);			  //
	// �E��
	vertex[1].position = D3DXVECTOR3(size / 2, size / 2, 0.0f);
	vertex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = color;
	vertex[1].texCoord = D3DXVECTOR2(1.0f, 0.0f);
	// ����
	vertex[2].position = D3DXVECTOR3(-size / 2, -size / 2, 0.0f);
	vertex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = color;
	vertex[2].texCoord = D3DXVECTOR2(0.0f, 1.0f);
	// �E��
	vertex[3].position = D3DXVECTOR3(size / 2, -size / 2, 0.0f);
	vertex[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].diffuse = color;
	vertex[3].texCoord = D3DXVECTOR2(1.0f, 1.0f);

	Renderer::GetDeviceContext()->Unmap(targetMarkVertexBuffer, 0);

	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(objVertexLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(objVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(objPixelShader, NULL, 0);

	// �J�����̃r���[�}�g���N�X�擾
	
	D3DXMATRIX view = cam->GetViewMatrix();

	//�@�r���[�̋t�s�� �t�̌v�Z������s��B�J�����ɑ΂��ċt�v�Z�����邱�ƂŃr���{�[�h�ɂȂ�
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view); // �t�s��
	invView._41 = 0.0f; // �ړ�������0�ɐݒ肵�Ĉړ��o���Ȃ��悤�ɂ���@float�Œl���Ǘ����Ă��邩��0�ɐݒ肵�Ă���
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	//�}�g���N�X�ݒ�
	D3DXMATRIX world, scale, trans;
	D3DXMatrixScaling(&scale, objScale.x, objScale.y, objScale.z);
	D3DXMatrixTranslation(&trans, objPosition.x, objPosition.y, objPosition.z);
	world = scale * invView * trans;
	Renderer::SetWorldMatrix(&world);

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &targetMarkVertexBuffer,
		&stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material{};
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &targetMarkTexture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(num, 0);
}
