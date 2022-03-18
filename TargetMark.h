#pragma once
#include "GameObject.h"
class TargetMark : public GameObject
{
private:
	ID3D11Buffer* targetMarkVertexBuffer = NULL;//NULLチェックするならNULLは必要、しないなら不要
	ID3D11ShaderResourceView* targetMarkTexture = NULL;
	int num;
	float size;
	D3DXVECTOR3 targetPos;
	D3DXVECTOR4 color;

	Camera* cam;
	Player* targetMarkPlayer;
	Enemy* targetMarkenemy;
public:
	void Init();   // 初期化関数
	void Uninit(); // 終了関数
	void Update(); // 更新関数
	void Draw();   // 描画関数
};