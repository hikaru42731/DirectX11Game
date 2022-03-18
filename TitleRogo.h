#pragma once
#include "GameObject.h"

class TitleRogo : public GameObject
{
private:

	ID3D11Buffer* titleVertexBuffer = NULL;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
	ID3D11ShaderResourceView* titleTexture = NULL;
	class T_Camera* cam;
public:

	
	void Init();
	void Uninit();
	void Update();
	void Draw();
};