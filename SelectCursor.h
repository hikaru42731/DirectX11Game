#pragma once
#include "GameObject.h"

class SelectCursor : public GameObject
{
private:
	ID3D11Buffer* selectCursorVertexBuffer = NULL;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
	ID3D11ShaderResourceView* selectCursorTexture = NULL;
	int sizeY;
	int sizeX;
	int line;
	bool select;
	int id;
	class Audio* moveSe;
	class Audio* pressSe;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};