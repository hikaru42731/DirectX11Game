#pragma once
#include "GameObject.h"

// クラス継承によりメンバ変数をまとめる fieldクラスにGameObjectクラスを継承する
class Field : public GameObject
{

	ID3D11Buffer* fieldVertexBuffer = NULL;//NULLチェックするならNULLは必要、しないなら不要
	ID3D11Buffer* fieldIndexBuffer = NULL;//NULLチェックするならNULLは必要、しないなら不要
	ID3D11ShaderResourceView* fieldTexture = NULL;

	VERTEX_3D	fieldVertex[21][21];
public:
	void Init();   // 初期化関数
	void Uninit(); // 終了関数
	void Update(); // 更新関数
	void Draw();   // 描画関数

	float GetHeight(D3DXVECTOR3 Position);

};