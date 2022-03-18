#pragma once
#include "GameObject.h"

class AbilityON : public GameObject
{
private:

	ID3D11Buffer* abilityOnVertexBuffer=NULL;//NULLチェックするならNULLは必要、しないなら不要
	ID3D11ShaderResourceView* onTexture;
	ID3D11ShaderResourceView* offTexture;
	ID3D11ShaderResourceView* abilityTexture;
	float posY;
	float sizeY;
	float sizeX;
	int on;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetON(int On) { on += On; on %= 2; }
	int GetON() { return (on+1)%2; }
};