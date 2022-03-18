#pragma once
#include "scene.h"
class Game :public Scene
{
protected:
	class Player* player;
	class Enemy* enemy1;
	class Enemy* enemy2;

	static bool enemyEmpty;
	void Result();
public:
	bool Init();
	void Uninit();
	void Update();
	static int id;
	static bool GetEnemyEmpty() { return enemyEmpty; }
};