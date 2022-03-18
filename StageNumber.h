#pragma once
#include "GameObject.h"

class StageNumber : public GameObject
{
private:

	ID3D11Buffer* abilityOnVertexBuffer = NULL;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
	ID3D11ShaderResourceView* numberTexture[5];
	ID3D11ShaderResourceView* drawTexture;

	float posY;
	float sizeY;
	float sizeX;
	int number;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void AddNumber(int AddNumber) { number += AddNumber; }
};