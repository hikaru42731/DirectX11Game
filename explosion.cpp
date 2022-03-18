#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Scene.h"
#include "Camera.h"
#include "Explosion.h"

// Explosion:: ��Explosion�N���X�̊֐����w�肵�Ă���
ID3D11ShaderResourceView* Explosion::explosionTexture;
ID3D11ShaderResourceView* Bomb::bombTexture;
ID3D11ShaderResourceView* LASTCHARGE::attackTexture;
ID3D11ShaderResourceView* LASTCHARGE::chargeTexture;
void Particle::Init()
{
	num = 30;
	
}
void Particle::Uninit()
{
}
void Particle::Update()
{
	for (int i = 0; i < num; i++)
	{
		Manager::GetScene()->AddGameObject<Explosion>()->SetPosition(objPosition);
		
	}
	SetDestroy();
}
void Particle::Draw()
{

}

void Explosion::Init()
{
	VERTEX_3D vertex[4];
	// ����
	vertex[0].position = D3DXVECTOR3(-10.0f, 10.0f, 0.0f);    // ���_�̃|�W�V����
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // �ǂ̖ʂ���Ȃ̂�(�@��)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); 
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);			  
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
	bd.CPUAccessFlags =  D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{}; // ���_�o�b�t�@������f�[�^
	sd.pSysMem = vertex; // ��������������_�f�[�^�ŏ��������Ă���

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &explosionVertexBuffer);// �o���オ�������̂�VertexBuffer�ɓ����

	// Shader�ňӖ��킩���G���[���o��Ƃ���cso�̖��O����������
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "unlitTexturePS.cso");

	if (rand() % 10 > 5)
	{
		if (rand() % 4 <= 2)
		{
			moveVec = D3DXVECTOR2(rand() % 5 + 1, rand() % 5 + 1);
		}
		else
		{
			moveVec = D3DXVECTOR2(rand() % 5 + 1, -rand() % 5 + 1);
		}
	}
	else
	{
		if (rand() % 4 <= 2)
		{
			moveVec = D3DXVECTOR2(-rand() % 5 - 1, rand() % 5 + 1);
		}
		else
		{
			moveVec = D3DXVECTOR2(-rand() % 5 - 1, -rand() % 5 + 1);
		}
	}
	D3DXVec2Normalize(&moveVec, &moveVec);
	objRotation = D3DXVECTOR3(moveVec.x,moveVec.y,0);
	color = D3DXVECTOR4(0, 0, 0, 1);
	objScale = D3DXVECTOR3(rand()%3+1.0f, rand() % 3 + 1.0f, 1.0f);
}

void Explosion::Uninit()
{
	explosionVertexBuffer->Release();

	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void Explosion::Update()
{
	cnt++;
	if (cnt % 3 == 0) {
		animeCount++;
	}
	moveVec.y += 0.05f;
	objPosition.x -= moveVec.x * 0.2;
	objPosition.y -= moveVec.y * 0.2;
	color.x += 0.08f;
	color.y += 0.08f;
	if (animeCount >= 16)
	{
		SetDestroy();
		return;
	}
}

void Explosion::Draw()
{
	//�e�N�X�`�����W�Z�o
	float x = animeCount % 4 * (1.0f / 4);
	float y = animeCount / 5 * (1.0f / 1);

	// ���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(explosionVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// ����
	vertex[0].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);    // ���_�̃|�W�V����
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // �ǂ̖ʂ���Ȃ̂�(�@��)
	vertex[0].diffuse = color; 
	vertex[0].texCoord = D3DXVECTOR2(x, y);			  
	// �E��
	vertex[1].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = color;
	vertex[1].texCoord = D3DXVECTOR2(x + 0.2f, y);
	// ����
	vertex[2].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = color;
	vertex[2].texCoord = D3DXVECTOR2(x, y + 0.25f);
	// �E��
	vertex[3].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].diffuse = color;
	vertex[3].texCoord = D3DXVECTOR2(x + 0.2f, y + 0.25f);

	Renderer::GetDeviceContext()->Unmap(explosionVertexBuffer, 0);

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
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &explosionVertexBuffer,
		&stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material{};
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &explosionTexture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void Explosion::Load()
{
	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/explosion.png",
		NULL,
		NULL,
		&explosionTexture,
		NULL);

	// �e�N�X�`����ǂݍ��߂����m�F����
	assert(explosionTexture);
}

void Explosion::UnLoad()
{
	explosionTexture->Release();
}

void Bomb::Init()
{
	VERTEX_3D vertex[4];
	// ����
	vertex[0].position = D3DXVECTOR3(-10.0f, 10.0f, 0.0f);    // ���_�̃|�W�V����
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // �ǂ̖ʂ���Ȃ̂�(�@��)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); 
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);			  
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

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &bombVertexBuffer);// �o���オ�������̂�VertexBuffer�ɓ����



	// Shader�ňӖ��킩���G���[���o��Ƃ���cso�̖��O����������
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "unlitTexturePS.cso");

	objPosition = D3DXVECTOR3(0.0f, 4.0f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(4.0f, 3.0f, 2.0f);
	cnt = 0;
	animeCount = 6;
}


void Bomb::Uninit()
{
	bombVertexBuffer->Release();

	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void Bomb::Update()
{
	cnt++;
	if (cnt % 3 == 0) {
		animeCount++;
	}
	
	
	if (animeCount >= 13)
	{
		SetDestroy();
		return;
	}
}

void Bomb::Draw()
{
	//�e�N�X�`�����W�Z�o
	float x = (animeCount % 5 * (1.0f / 5));
	float y = (animeCount / 5 * (1.0f / 3));

	// ���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(bombVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// ����
	vertex[0].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);    // ���_�̃|�W�V����
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // �ǂ̖ʂ���Ȃ̂�(�@��)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); 
	vertex[0].texCoord = D3DXVECTOR2(x, y);			  
	// �E��
	vertex[1].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2(x + 0.2f, y);
	// ����
	vertex[2].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2(x, y + 0.33f);
	// �E��
	vertex[3].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = D3DXVECTOR2(x + 0.2f, y + 0.33f);

	Renderer::GetDeviceContext()->Unmap(bombVertexBuffer, 0);

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
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &bombVertexBuffer,
		&stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material{};
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &bombTexture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void Bomb::Load()
{
	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/Bomb.png",
		NULL,
		NULL,
		&bombTexture,
		NULL);

	// �e�N�X�`����ǂݍ��߂����m�F����
	assert(bombTexture);
}

void Bomb::UnLoad()
{
	bombTexture->Release();
}

void  LASTCHARGE::Init()
{
	VERTEX_3D vertex[4];
	// ����
	vertex[0].position = D3DXVECTOR3(-10.0f, 10.0f, 0.0f);    // ���_�̃|�W�V����
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // �ǂ̖ʂ���Ȃ̂�(�@��)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); 
	vertex[0].texCoord = D3DXVECTOR2(0.0f, 0.0f);			 
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

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &lastChargeVertexBuffer);// �o���オ�������̂�VertexBuffer�ɓ����



	// Shader�ňӖ��킩���G���[���o��Ƃ���cso�̖��O����������
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "unlitTexturePS.cso");

	objPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	cnt = 0;
	charge = true;
	animeCount = 0;
}


void  LASTCHARGE::Uninit()
{
	lastChargeVertexBuffer->Release();

	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void LASTCHARGE::Update()
{
	
	if (charge == true)
	{
		if (cnt % 4 == 0) {
			animeCount++;
		}


		if (animeCount >= 13)
		{
			charge = false;
			animeCount = 0;
			
			return;
		}
	}
	else
	{
		if (cnt % 4 == 0) {
			animeCount++;
		}


		if (animeCount >= 13)
		{
			SetDestroy();
			return;
		}
	}
	cnt++;
}

void  LASTCHARGE::Draw()
{
	//�e�N�X�`�����W�Z�o
	float x = (animeCount % 5 * (1.0f / 5));
	float y = (animeCount / 5 * (1.0f / 3));

	// ���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(lastChargeVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	// ����
	vertex[0].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);    // ���_�̃|�W�V����
	vertex[0].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // �ǂ̖ʂ���Ȃ̂�(�@��)
	vertex[0].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); 
	vertex[0].texCoord = D3DXVECTOR2(x, y);			  
	// �E��
	vertex[1].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = D3DXVECTOR2(x + 0.2f, y);
	// ����
	vertex[2].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = D3DXVECTOR2(x, y + 0.33f);
	// �E��
	vertex[3].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = D3DXVECTOR2(x + 0.2f, y + 0.33f);

	Renderer::GetDeviceContext()->Unmap(lastChargeVertexBuffer, 0);

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
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &lastChargeVertexBuffer,
		&stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material{};
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	if (charge == true)
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &chargeTexture);
	}
	else
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &attackTexture);
	}

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void  LASTCHARGE::Load()
{
	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/Charge.png",
		NULL,
		NULL,
		&chargeTexture,
		NULL);

	// �e�N�X�`����ǂݍ��߂����m�F����
	assert(chargeTexture);
	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/Last.png",
		NULL,
		NULL,
		&attackTexture,
		NULL);

	// �e�N�X�`����ǂݍ��߂����m�F����
	assert(attackTexture);
}

void  LASTCHARGE::UnLoad()
{
	chargeTexture->Release();
	attackTexture->Release();
}
