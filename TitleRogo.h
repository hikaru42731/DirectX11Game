#pragma once
#include "GameObject.h"

class TitleRogo : public GameObject
{
private:

	ID3D11Buffer* titleVertexBuffer = NULL;//NULLチェックするならNULLは必要、しないなら不要
	ID3D11ShaderResourceView* titleTexture = NULL;
	class T_Camera* cam;
public:

	
	void Init();
	void Uninit();
	void Update();
	void Draw();
};