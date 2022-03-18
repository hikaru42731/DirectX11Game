#pragma once
#include "GameObject.h"

class PlayerHP : public GameObject
{
private:

	ID3D11Buffer* playerHpVertexBuffer = NULL;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
	ID3D11ShaderResourceView* playerHpTexture = NULL;
	Camera* cam;
	float animenum;
public:

	void Init();
	void Uninit();
	void Update();
	void Draw();
};