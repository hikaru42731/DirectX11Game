#pragma once
#include "GameObject.h"

class SelectBack : public GameObject
{
private:

	ID3D11Buffer* selectBackVertexBuffer = NULL;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
	ID3D11ShaderResourceView* selectBackTexture = NULL;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};