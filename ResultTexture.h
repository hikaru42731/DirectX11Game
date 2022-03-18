#pragma once
#include "GameObject.h"

class R_Texture : public GameObject
{
private:

	ID3D11Buffer* resultTextureVertexBuffer = NULL;//NULLチェックするならNULLは必要、しないなら不要
	static ID3D11ShaderResourceView* resultTexture;
	static bool win;
public:

	//Polygon2D();//コンストラクタ（インスタンスを生成したときに自動的に呼ばれる）
	//~Polygon2D()//デコンストラクタ（インスタンスが終了するときに自動的に呼ばれる）
	void Init();
	void Uninit();
	void Update();
	void Draw();
	static void Load(bool W);
	bool GetWin() { return win; }
};