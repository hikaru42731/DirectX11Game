#pragma once
#include "GameObject.h"
class Player_Bar : public GameObject
{
private:
	ID3D11Buffer* playerBarVertexBuffer = NULL;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
	ID3D11ShaderResourceView* playerBarTexture = NULL;
	int num;
	float sizeX;
	Camera* cam;
	Player* barPlayer;
public:
	void Init();   // �������֐�
	void Uninit(); // �I���֐�
	void Update(); // �X�V�֐�
	void Draw();   // �`��֐�
};