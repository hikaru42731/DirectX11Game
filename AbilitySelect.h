#pragma once
#include "scene.h"
class AbilitySelect :public Scene
{
protected:
	int abilityId=0;
	int cnt;
	bool end;
	static int gameId;
	class StageNumber* stageNumber;
public:
	bool Init();
	void Uninit();
	void Update();
	static int GameStageNumber() { return gameId; }
	static void SetStageNumber(int StageNum) { gameId = StageNum; }
};