#pragma once
#include "GameObject.h"

// クラス継承によりメンバ変数をまとめる fieldクラスにGameObjectクラスを継承する
class VSField : public GameObject
{
	ID3D11Buffer* vsFieldVertexBuffer = NULL;//NULLチェックするならNULLは必要、しないなら不要
	ID3D11ShaderResourceView* vsFieldTexture = NULL;
	class Model* vsFieldModel;
public:
	void Init();   // 初期化関数
	void Uninit(); // 終了関数
	void Update(); // 更新関数
	void Draw();   // 描画関数

};