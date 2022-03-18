#pragma once
#include "GameObject.h"
class Player_Bar : public GameObject
{
private:
	ID3D11Buffer* playerBarVertexBuffer = NULL;//NULLチェックするならNULLは必要、しないなら不要
	ID3D11ShaderResourceView* playerBarTexture = NULL;
	int num;
	float sizeX;
	Camera* cam;
	Player* barPlayer;
public:
	void Init();   // 初期化関数
	void Uninit(); // 終了関数
	void Update(); // 更新関数
	void Draw();   // 描画関数
};