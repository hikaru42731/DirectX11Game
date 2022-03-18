#pragma once
#include "GameObject.h"

// �N���X�p���ɂ�胁���o�ϐ����܂Ƃ߂� field�N���X��GameObject�N���X���p������
class Field : public GameObject
{

	ID3D11Buffer* fieldVertexBuffer = NULL;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
	ID3D11Buffer* fieldIndexBuffer = NULL;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
	ID3D11ShaderResourceView* fieldTexture = NULL;

	VERTEX_3D	fieldVertex[21][21];
public:
	void Init();   // �������֐�
	void Uninit(); // �I���֐�
	void Update(); // �X�V�֐�
	void Draw();   // �`��֐�

	float GetHeight(D3DXVECTOR3 Position);

};