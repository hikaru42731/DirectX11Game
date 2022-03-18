#pragma once

#include "GameObject.h"

// クラス継承によりメンバ変数をまとめる fieldクラスにGameObjectクラスを継承する
class Explosion : public GameObject
{
private:
	ID3D11Buffer*				 explosionVertexBuffer = NULL;//NULLチェックするならNULLは必要、しないなら不要
	static ID3D11ShaderResourceView* explosionTexture;

	int animeCount;
	class Camera* explosionCam;
	class Scene* explosionScene;
	D3DXVECTOR4 color;
	D3DXVECTOR2 moveVec;
	float gravity;
	float size;
	int cnt;
public:
	void Init();   // 初期化関数
	void Uninit(); // 終了関数
	void Update(); // 更新関数
	void Draw();   // 描画関数
	static void Load();	//ロード
	static void UnLoad();	//ロード
};

// クラス継承によりメンバ変数をまとめる fieldクラスにGameObjectクラスを継承する
class Particle : public GameObject
{
private:
	int num;
public:
	void Init();   // 初期化関数
	void Uninit(); // 終了関数
	void Update();
	void Draw();
};

// クラス継承によりメンバ変数をまとめる fieldクラスにGameObjectクラスを継承する
class Bomb : public GameObject
{
private:
	ID3D11Buffer* bombVertexBuffer = NULL;//NULLチェックするならNULLは必要、しないなら不要
	static ID3D11ShaderResourceView* bombTexture;

	int animeCount;
	int cnt;
public:
	void Init();   // 初期化関数
	void Uninit(); // 終了関数
	void Update(); // 更新関数
	void Draw();   // 描画関数

	static void Load();
	static void UnLoad();
};

class LASTCHARGE : public GameObject
{
private:
	ID3D11Buffer* lastChargeVertexBuffer = NULL;//NULLチェックするならNULLは必要、しないなら不要
	static ID3D11ShaderResourceView* chargeTexture;
	static ID3D11ShaderResourceView* attackTexture;

	int animeCount;
	int cnt;
	bool charge;
public:
	void Init();   // 初期化関数
	void Uninit(); // 終了関数
	void Update(); // 更新関数
	void Draw();   // 描画関数

	static void Load();
	static void UnLoad();
};