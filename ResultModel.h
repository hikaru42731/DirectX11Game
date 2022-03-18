#pragma once
#include "GameObject.h"
class R_Model : public GameObject
{
private:
	class AnimationModel* resultModel;
	bool win;
	float animeRate;
	int animeFrame;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetWin(bool Win) { win = Win; }
};