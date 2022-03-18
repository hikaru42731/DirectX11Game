#pragma once
#include "GameObject.h"

class EnemyHP : public GameObject
{
private:
	ID3D11Buffer*			 	 enemyHpVertexBuffer = NULL;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
	ID3D11ShaderResourceView*	 enemyHpTexture = NULL;
	Camera* cam;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};