#pragma once
// クラス継承によりメンバ変数をまとめる fieldクラスにGameObjectクラスを継承する
class EnemyWarp : public GameObject
{
private:
	ID3D11Buffer* enemyWarpVertexBuffer = NULL;//NULLチェックするならNULLは必要、しないなら不要
	static ID3D11ShaderResourceView* enemyWarpTexture;
	
	bool warp;
	float telescopicSpeed;	//伸縮スピード
	bool extend;
	static class Audio* seWarp;
	bool audioPlay;
public:
	void Init();   // 初期化関数
	void Uninit(); // 終了関数
	void Update(); // 更新関数
	void Draw();   // 描画関数
	void SetWarp(bool Warp) { warp = Warp; }

	static void Load();
	static void UnLoad();
};