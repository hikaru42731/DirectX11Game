#pragma once
#include "GameObject.h"

class PlayerHP : public GameObject
{
private:

	ID3D11Buffer* playerHpVertexBuffer = NULL;//NULLチェックするならNULLは必要、しないなら不要
	ID3D11ShaderResourceView* playerHpTexture = NULL;
	Camera* cam;
	float animenum;
public:

	void Init();
	void Uninit();
	void Update();
	void Draw();
};