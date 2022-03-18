#pragma once
#include "scene.h"
class Lose :public Scene
{
protected:
	class R_Model* resultLoseModel;
	class R_Texture* resultLoseTexture;
	class ResultCursor* cur;
	int gameId;
	bool end;
	int cnt;
public:
	bool Init();
	void Uninit();
	void Update();

};