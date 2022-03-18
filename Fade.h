#pragma once
class Fade
{
private:
	static ID3D11Buffer* fadeVertexBuffer;//NULLチェックするならNULLは必要、しないなら不要
	static ID3D11ShaderResourceView* fadeTexture;
	
	static ID3D11VertexShader* fadeVertexShader;
	static ID3D11PixelShader* fadePixelShader;
	static ID3D11InputLayout* fadeVertexLayout;
	static bool extend;
	static bool shrink;
	static float alpha;
	static bool end;
	static class Audio* fadeSe;
	int cnt;
public:
	static void Init();   // 初期化関数
	static void Uninit(); // 終了関数
	static void Update(); // 更新関数
	static void Draw();   // 描画関数
	static void Load();
	static void UnLoad();
	static void StartFade(int Mode) { if (Mode == 0) { extend = true; shrink = false; } else { shrink = true; extend = false; } }
	static bool GetFadeEnd() { return end; }
	static void SetFadeEnd() { end = false; }
};