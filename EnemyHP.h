#pragma once
#include "GameObject.h"

class EnemyHP : public GameObject
{
private:
	ID3D11Buffer*			 	 enemyHpVertexBuffer = NULL;//NULLチェックするならNULLは必要、しないなら不要
	ID3D11ShaderResourceView*	 enemyHpTexture = NULL;
	Camera* cam;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};