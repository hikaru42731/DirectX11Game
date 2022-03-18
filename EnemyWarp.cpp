#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Scene.h"
#include "Camera.h"
#include "Enemy.h"
#include "EnemyWarp.h"
#include "Audio.h"

std::vector<Enemy*> w_enemyList;
class Audio* EnemyWarp::seWarp;
ID3D11ShaderResourceView* EnemyWarp::enemyWarpTexture;
void EnemyWarp::Init()
{
	VERTEX_3D vertex[4];
	// ����
	vertex[0].position = D3DXVECTOR3(-10.0f, 10.0f, 0.0f);    // ���_�̃|�W�V����
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // �ǂ̖ʂ���Ȃ̂�(�@��)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); //
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);			  //
	// �E��
	vertex[1].position = D3DXVECTOR3(10.0f, 10.0f, 0.0f);
	vertex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2(1.0f, 0.0f);
	// ����
	vertex[2].position = D3DXVECTOR3(-10.0f, -10.0f, 0.0f);
	vertex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2(0.0f, 1.0f);
	// �E��
	vertex[3].position = D3DXVECTOR3(10.0f, -10.0f, 0.0f);
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
	sd.pSysMem = vertex; // ��������������_�f�[�^�ŏ��������Ă���

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &enemyWarpVertexBuffer);// �o���オ�������̂�VertexBuffer�ɓ����

	// Shader�ňӖ��킩���G���[���o��Ƃ���cso�̖��O����������
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "unlitTexturePS.cso");

	objPosition = D3DXVECTOR3(0.0f, 4.0f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(1.0f, 0.5f, 2.0f);
	extend = true;
	audioPlay = false;
	w_enemyList.clear();
}

void EnemyWarp::Uninit()
{
	enemyWarpVertexBuffer->Release();

	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void EnemyWarp::Update()
{
	if (warp == true)
	{
		if (audioPlay == false)
		{
			seWarp->Play(false);
			audioPlay = true;
		}
		if (extend == true)
		{
			telescopicSpeed += 0.001f;
			objScale.x += telescopicSpeed;;
			if (objScale.x > 2.0f)extend = false;
		}

	}
	else
	{
		audioPlay = false;
		objScale.x = 1.0f;
		extend = true;
	}
}

void EnemyWarp::Draw()
{
	// ���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(enemyWarpVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// ����
	vertex[0].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);    // ���_�̃|�W�V����
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // �ǂ̖ʂ���Ȃ̂�(�@��)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); //
	vertex[0].texCoord = D3DXVECTOR2(0, 0);			  //
	// �E��
	vertex[1].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2(1, 0);
	// ����
	vertex[2].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2(0, 1);
	// �E��
	vertex[3].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = D3DXVECTOR2(1,1);

	Renderer::GetDeviceContext()->Unmap(enemyWarpVertexBuffer, 0);

	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(objVertexLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(objVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(objPixelShader, NULL, 0);

	// �J�����̃r���[�}�g���N�X�擾
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>(); // �P�̂œǂݍ��ގ���GetGameObject���g���B�����ǂݍ��ގ��A�G��ʂȂǂ�GetGameObjets���g���ă��[�v�����œǂݍ��ށB
	D3DXMATRIX view = camera->GetViewMatrix();

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
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &enemyWarpVertexBuffer,
		&stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material{};
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &enemyWarpTexture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		if (warp == true) {
			// �|���S���`��
			Renderer::GetDeviceContext()->Draw(4, 0);
		}
}


void EnemyWarp::Load()
{
	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/Warp.png",
		NULL,
		NULL,
		&enemyWarpTexture,
		NULL);

	// �e�N�X�`����ǂݍ��߂����m�F����
	assert(enemyWarpTexture);
	seWarp = new Audio;
	seWarp->Load("asset\\Audio\\Warp.wav");
}

void EnemyWarp::UnLoad()
{
	enemyWarpTexture->Release();
}
