#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
private:
	static class Model* bulletModel;
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
	void SetSpeed(float Sp) { speed = Sp; }
};

class EBullet : public GameObject
{
private:
	static class Model* enemyBulletModel;
	D3DXVECTOR3 forwardVec;
	int cnt;
	float speed;
	bool induct;
	float inductRange;
	int existCnt;
	D3DXVECTOR3 target;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	static void Load();
	static void UnLoad();
	void SetTarget(D3DXVECTOR3 Tar) { target = Tar; }
	void SetSpeed(float Sp) { speed = Sp; }
	void Rotate();
};