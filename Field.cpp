#include "Main.h"
#include "Renderer.h"
#include "Field.h"


float g_FieldHeight[21][21] =
{
{0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 1.0f, 2.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 1.0f, 2.0f, 1.0f, 3.0f, 1.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 1.0f, 5.0f, 5.0f, 7.0f, 3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 4.0f, 5.0f, 6.0f, 7.0f, 3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 4.0f, 5.0f, 5.0f, 4.0f, 3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 1.0f, 2.0f, 4.0f, 3.0f, 2.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 1.0f, 2.0f, 4.0f, 4.0f, 1.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 1.0f, 2.0f, 3.0f, 2.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
{0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
};
// Field:: ��Field�N���X�̊֐����w�肵�Ă���
void Field::Init()
{
	
	// ���_�o�b�t�@�쐬
	{
		for (int x = 0; x <= 20; x++)
		{
			for (int z = 0; z <= 20; z++)
			{
				float y = g_FieldHeight[x][z];
				fieldVertex[x][z].position = D3DXVECTOR3((x - 10) * 5.0f, y, (z - 10) * -5.0f);
				fieldVertex[x][z].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				fieldVertex[x][z].diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				fieldVertex[x][z].texCoord = D3DXVECTOR2(x * 0.5f, z * 0.5f);
			}
		}

		//�@���x�N�g���Z�o
		for (int x = 1; x <= 19; x++)
		{
			for (int z = 1; z <= 19; z++)
			{
				D3DXVECTOR3 vx, vz, vn;

				vx = fieldVertex[x + 1][z].position - fieldVertex[x - 1][z].position;
				vz = fieldVertex[x][z - 1].position - fieldVertex[x][z + 1].position;

				D3DXVec3Cross(&vn, &vz, &vx);
				D3DXVec3Normalize(&vn, &vn);
				fieldVertex[x][z].normal = vn;
			}
		}
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * 21 * 21; //���_�o�b�t�@�̐�
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd; // ���_�o�b�t�@������f�[�^
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = fieldVertex; // ��������������_�f�[�^�ŏ��������Ă���

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &fieldVertexBuffer);// �o���オ�������̂�VertexBuffer�ɓ����
	}
	//�C���f�b�N�X�o�b�t�@����
	{
		unsigned int index[(22 * 2) * 20 - 2];

		int i = 0;
		for (int x = 0; x < 20; x++)
		{
			for (int z = 0; z < 21; z++)
			{
				index[i] = x * 21 + z;
				i++;

				index[i] = (x + 1) * 21 + z;
				i++;
			}
			if (x == 19) break;

			index[i] = (x + 1) * 21 + 20;
			i++;

			index[i] = (x + 1) * 21;
			i++;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * ((22 * 2) * 20 - 2); //���_�o�b�t�@�̐�
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd; // ���_�o�b�t�@������f�[�^
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &fieldIndexBuffer);// �o���オ�������̂�IndexBuffer�ɓ����
	}

	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/rocky.png",
		NULL,
		NULL,
		&fieldTexture,
		NULL);

	// �e�N�X�`����ǂݍ��߂����m�F����
	assert(fieldTexture);

	// Shader�ňӖ��킩���G���[���o��Ƃ���cso�̖��O����������
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "vertexLightingPS.cso");

	objPosition = D3DXVECTOR3(0.0f,0.0f,0.0f);
	objRotation = D3DXVECTOR3(0.0f,0.0f,0.0f);
	objScale    = D3DXVECTOR3(1.0f,1.0f,1.0f);
}


void Field::Uninit()
{
	fieldVertexBuffer->Release();
	fieldIndexBuffer->Release();
	fieldTexture->Release();

	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void Field::Update()
{

}

void Field::Draw()
{
	//�}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, objScale.x, objScale.y, objScale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, objRotation.x, objRotation.y, objRotation.z);
	D3DXMatrixTranslation(&trans, objPosition.x, objPosition.y-1.0f, objPosition.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	
	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &fieldVertexBuffer,
		&stride, &offset);

	//�C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(fieldIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &fieldTexture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->DrawIndexed((22 * 2) * 20 - 2, 0, 0);
}

float Field::GetHeight(D3DXVECTOR3 Position)
{
	int x, z;

	//�u���b�N�ԍ��Z�o
	x = Position.x / 5.0f + 10.0f;
	z = Position.z / -5.0f + 10.0f;

	D3DXVECTOR3 pos0, pos1, pos2, pos3;

	pos0 = fieldVertex[x][z].position;
	pos1 = fieldVertex[x+1][z].position;
	pos2 = fieldVertex[x][z+1].position;
	pos3 = fieldVertex[x+1][z+1].position;

	D3DXVECTOR3 v12, v1p,c;
	v12 = pos2 - pos1;
	v1p = Position - pos1;

	D3DXVec3Cross(&c, &v12, &v1p);

	float py;
	D3DXVECTOR3 n;

	if (c.y > 0.0f)
	{
		//����|���S��
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);
	}
	else
	{
		//�E���|���S��
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&n, &v12, &v13);
	}

	//�����擾
	py = -((Position.x - pos1.x) * n.x + (Position.z - pos1.z) * n.z) / n.y + pos1.y;

	return py;
}

