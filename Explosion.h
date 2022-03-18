#pragma once

#include "GameObject.h"

// �N���X�p���ɂ�胁���o�ϐ����܂Ƃ߂� field�N���X��GameObject�N���X���p������
class Explosion : public GameObject
{
private:
	ID3D11Buffer*				 explosionVertexBuffer = NULL;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
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
	void Init();   // �������֐�
	void Uninit(); // �I���֐�
	void Update(); // �X�V�֐�
	void Draw();   // �`��֐�
	static void Load();	//���[�h
	static void UnLoad();	//���[�h
};

// �N���X�p���ɂ�胁���o�ϐ����܂Ƃ߂� field�N���X��GameObject�N���X���p������
class Particle : public GameObject
{
private:
	int num;
public:
	void Init();   // �������֐�
	void Uninit(); // �I���֐�
	void Update();
	void Draw();
};

// �N���X�p���ɂ�胁���o�ϐ����܂Ƃ߂� field�N���X��GameObject�N���X���p������
class Bomb : public GameObject
{
private:
	ID3D11Buffer* bombVertexBuffer = NULL;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
	static ID3D11ShaderResourceView* bombTexture;

	int animeCount;
	int cnt;
public:
	void Init();   // �������֐�
	void Uninit(); // �I���֐�
	void Update(); // �X�V�֐�
	void Draw();   // �`��֐�

	static void Load();
	static void UnLoad();
};

class LASTCHARGE : public GameObject
{
private:
	ID3D11Buffer* lastChargeVertexBuffer = NULL;//NULL�`�F�b�N����Ȃ�NULL�͕K�v�A���Ȃ��Ȃ�s�v
	static ID3D11ShaderResourceView* chargeTexture;
	static ID3D11ShaderResourceView* attackTexture;

	int animeCount;
	int cnt;
	bool charge;
public:
	void Init();   // �������֐�
	void Uninit(); // �I���֐�
	void Update(); // �X�V�֐�
	void Draw();   // �`��֐�

	static void Load();
	static void UnLoad();
};