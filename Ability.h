#pragma once
#include "GameObject.h"

// �N���X�p���ɂ�胁���o�ϐ����܂Ƃ߂� field�N���X��GameObject�N���X���p������
class Ability : public GameObject
{
	bool ability[11];
	bool game;	//�V�[�����Q�[����
	class Player* abilityPlayer;
	class Scene* abilityScene;
public:
	void Init();   // �������֐�
	void Uninit(); // �I���֐�
	void Update(); // �X�V�֐�
	void Draw();   // �`��֐�
};