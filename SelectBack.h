#pragma once
#include "GameObject.h"

class SelectBack : public GameObject
{
private:

	ID3D11Buffer* selectBackVertexBuffer = NULL;//NULLチェックするならNULLは必要、しないなら不要
	ID3D11ShaderResourceView* selectBackTexture = NULL;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};