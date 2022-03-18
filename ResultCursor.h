#pragma once
#include "GameObject.h"

class ResultCursor : public GameObject
{
private:
	ID3D11Buffer* resultCurVertexBuffer = NULL;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
	ID3D11ShaderResourceView* resultCurTexture = NULL;
	int sizeY;
	int sizeX;
	class Audio* moveSe;
	class Audio* pressSe;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};