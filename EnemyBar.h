#pragma once
#include "GameObject.h"
class Enemy_Bar : public GameObject
{
private:
	ID3D11Buffer* enemyBarVertexBuffer = NULL;//NULLチェックするならNULLは必要、しないなら不要
	ID3D11ShaderResourceView* enemyBarTexture = NULL;
	int num;
	float sizeX;
	Camera* cam;
	Enemy* barEnemy;
public:
	void Init();   // 初期化関数
	void Uninit(); // 終了関数
	void Update(); // 更新関数
	void Draw();   // 描画関数
};