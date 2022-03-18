#pragma once
#include "scene.h"
class Win :public Scene
{
protected:
	class R_Model* resultWinModel;
	class R_Texture* resultWinModelTexture;
	class ResultCursor* cur;
	int gameId;
	bool end;
	int cnt;
public:
	bool Init();
	void Uninit();
	void Update();
};