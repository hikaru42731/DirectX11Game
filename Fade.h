#pragma once
class Fade
{
private:
	static ID3D11Buffer* fadeVertexBuffer;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
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
	static void Init();   // �������֐�
	static void Uninit(); // �I���֐�
	static void Update(); // �X�V�֐�
	static void Draw();   // �`��֐�
	static void Load();
	static void UnLoad();
	static void StartFade(int Mode) { if (Mode == 0) { extend = true; shrink = false; } else { shrink = true; extend = false; } }
	static bool GetFadeEnd() { return end; }
	static void SetFadeEnd() { end = false; }
};