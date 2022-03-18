#pragma once
#include "GameObject.h"

class AtkWall : public GameObject
{
private:
	static class Model* atkWallModel;
	D3DXVECTOR3 playerPosition;
	D3DXVECTOR3 playerForward;
	D3DXVECTOR3 forwardVec;
	int cnt;
	float speed;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	static void Load();
	static void UnLoad();
};