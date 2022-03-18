#pragma once
#include "GameObject.h"
class AttackJudge:public GameObject
{
private:
	int cnt;
	int existTime;
	class Model* atkModel;
public:
	void Init();
	void Update();
	void Draw();
	void Uninit();
	float big;
	static void Load();
	static void UnLoad();
	void SetExistTime(int t) { existTime = t; }
};

class EnemyAttackJudge :public GameObject
{
private:
	int cnt;
	int existTime;
	bool armorBreak; //スーパーアーマー貫通するか
	static class Audio* hitSe;
	class Model* enemyAtkModel;
public:
	void Init();
	void Update();
	void Draw();
	void Uninit();
	float big;
	
	static void Load();
	static void UnLoad();
	void SetExistTime(int Time) { existTime = Time; }
	void SetArmorBreak(bool ArmorBreak) { armorBreak = ArmorBreak; }
};