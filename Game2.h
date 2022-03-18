#pragma once
#include "scene.h"
class Game2 :public Scene
{
protected:
	class Player* player;
	class Enemy* enemy1;
	class Enemy* enemy2;
	class Enemy* enemy3;
	static bool enemyEmpty;


	void Result();
public:
	bool Init();
	void Uninit();
	void Update();
	static int ID;
	static bool GetEnemyEmpty() { return enemyEmpty; }
};