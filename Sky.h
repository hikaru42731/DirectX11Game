#pragma once
#include "GameObject.h"

// �N���X�p���ɂ�胁���o�ϐ����܂Ƃ߂� field�N���X��GameObject�N���X���p������
class Sky : public GameObject
{
	ID3D11Buffer* skyVertexBuffer = NULL;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
	ID3D11ShaderResourceView* skyTexture = NULL;
public:
	void Init();   // �������֐�
	void Uninit(); // �I���֐�
	void Update(); // �X�V�֐�
	void Draw();   // �`��֐�

};
