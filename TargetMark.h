#pragma once
#include "GameObject.h"
class TargetMark : public GameObject
{
private:
	ID3D11Buffer* targetMarkVertexBuffer = NULL;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
	ID3D11ShaderResourceView* targetMarkTexture = NULL;
	int num;
	float size;
	D3DXVECTOR3 targetPos;
	D3DXVECTOR4 color;

	Camera* cam;
	Player* targetMarkPlayer;
	Enemy* targetMarkenemy;
public:
	void Init();   // �������֐�
	void Uninit(); // �I���֐�
	void Update(); // �X�V�֐�
	void Draw();   // �`��֐�
};