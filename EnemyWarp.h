#pragma once
// �N���X�p���ɂ�胁���o�ϐ����܂Ƃ߂� field�N���X��GameObject�N���X���p������
class EnemyWarp : public GameObject
{
private:
	ID3D11Buffer* enemyWarpVertexBuffer = NULL;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
	static ID3D11ShaderResourceView* enemyWarpTexture;
	
	bool warp;
	float telescopicSpeed;	//�L�k�X�s�[�h
	bool extend;
	static class Audio* seWarp;
	bool audioPlay;
public:
	void Init();   // �������֐�
	void Uninit(); // �I���֐�
	void Update(); // �X�V�֐�
	void Draw();   // �`��֐�
	void SetWarp(bool Warp) { warp = Warp; }

	static void Load();
	static void UnLoad();
};