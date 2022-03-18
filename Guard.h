#pragma once
#include "GameObject.h"

class Guard : public GameObject
{
private:
	static class Model* guardModel;
	class Player* guardPlayer;
	D3DXVECTOR3 guardPosition;
	D3DXVECTOR3 guardForward;
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