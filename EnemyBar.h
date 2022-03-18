#pragma once
#include "GameObject.h"
class Enemy_Bar : public GameObject
{
private:
	ID3D11Buffer* enemyBarVertexBuffer = NULL;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
	ID3D11ShaderResourceView* enemyBarTexture = NULL;
	int num;
	float sizeX;
	Camera* cam;
	Enemy* barEnemy;
public:
	void Init();   // �������֐�
	void Uninit(); // �I���֐�
	void Update(); // �X�V�֐�
	void Draw();   // �`��֐�
};