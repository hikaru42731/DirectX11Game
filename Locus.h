#pragma once
#include "GameObject.h"
class Locus : public GameObject
{
private:
	static class Model* locusSideModel;
	static class Model* locusVerticalModel;
	static class Model* locusDirgonalModel;
	static class Model* locusLongModel;
	static class Model* locusDiveModel;
	class Player* locusPlayer;
	D3DXVECTOR3 playerPosition;
	D3DXVECTOR3 guardForward;
	D3DXVECTOR3 forwardVector;
	int forwardPow;
	int vec;
	float rotateSpeed;
	bool rotate;
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	static void Load();
	static void UnLoad();
	void SetRotate(D3DXVECTOR3 Rotate, int Mode) { if (rotate == false) { objRotation = Rotate; }rotate = true; vec = Mode; }
	void SetForwardPow(int ForwardLength) { forwardPow = ForwardLength; }
};
