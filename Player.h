#pragma once
#include "GameObject.h"

// �N���X�p���ɂ�胁���o�ϐ����܂Ƃ߂� Player�N���X��GameObject�N���X���p������
class Player : public GameObject
{
private:
	class AnimationModel* playerModel;
	static class Audio* playerSe[10];
	class Enemy* nearEnemy;
	class Enemy* target;
	class Scene* playerScene;
	class AttackJudge* playerAtk;
	class Bullet* playerBul;
	class Bullet* playerBul1;
	class Bullet* playerBul2;
	class Camera* playerCamera;
	class Locus* locus;
	class LASTCHARGE* las;

	void PDefault();
	void PAttack();
	void PGuard();
	void PInvicible();
	void PRevenge();
	void PFalter();
	void PBlow();
	void HitWall();
	void GetDamage();
	void ColEnemy();

	float speed;	//���݂̑���
	float jumpPower; //�W�����v��
	bool invicible;	//���G
	bool jump;		//�W�����v���Ă邩
	bool combo1, combo2, combo3, comboLast;	//���i�ڂ̍U����
	bool revenge, morter;	//�J�E���^�[�Ǘ�
	bool attackAction;	//�U�����쒆��
	bool jCommandAction;	//�K�[�h�A��𒆂�
	bool interval;	//�K�[�h����̃C���^�[�o�������ǂ���
	bool diagonal;	//�΂߈ړ����Ă邩
	bool audioPlay;
	bool enemyWarp;
	float intervalCnt;
	float counterCnt;
	float yPrev = 0, yTemp = 0;
	float axel;
	float move;
	float enemyLength;
	float attackSpeed; //�U���Ǐ]���x
	float attackCnt;	//�U�����Ԑ���
	float invicibleCnt;	//������Ԑ���
	float playerY;	//�U���O�̃v���C���[y���W
	float targetY;	//�U���O�̃^�[�Q�b�gy���W
	float fallVelocity;	//��������
	bool yRock, atkAdd;
	float atkDelayTime;//�U�����Ă��玟�̍U�����o�Ȃ��Ԃ̎���
	bool walk;

	D3DXVECTOR3 prevPos;
	D3DXVECTOR3 vecForward;
	D3DXVECTOR3 vecRight;
	D3DXVECTOR3 cameraFVec;
	D3DXVECTOR3 cameraRVec;
	D3DXVECTOR3 cameraRot;
	D3DXVECTOR3 rotAngle;
	D3DXVECTOR3 atkDir;
	D3DXVECTOR3 targetDir;	//�U�����Ɏ擾����^�[�Q�b�g�ւ̌���
	D3DXVECTOR3 cameraPos;	//�����瑤����J������������Ƃ�
	D3DXVECTOR3 cameraTar;	//�J�����̃^�[�Q�b�g�������炩��w�肷�鎞
	D3DXVECTOR3 locusVer= D3DXVECTOR3(GetRotation().x, GetRotation().y + 1.57, GetRotation().z - 1.62);	//�c��]�Z�b�g
	D3DXVECTOR3 locusSide= D3DXVECTOR3(GetRotation().x, GetRotation().y - 2.7, GetRotation().z);;	//����]�Z�b�g
	D3DXVECTOR3 locusDia= D3DXVECTOR3(GetRotation().x, GetRotation().y - 1.57, GetRotation().z + 0.5);;	//�΂߉�]�Z�b�g
	float targetLength;			//�^�[�Q�b�g�Ƃ̋���

	bool slashRun;
	float slashRunSpeed;
	D3DXMATRIX looking;

	D3DXMATRIX rot;

	int animeFrame, animeDodgeFrame, animeAttackFrame, animeFalterFrame;
	float  animeRunRate, animeJumpRate, animeGuardRate, animeDodgeRate, animeAttackRate, animeFalterRate;

	bool assult, abi1;	//ON,OFF�pbool,�R���{���ɓ��o�Ȃ��悤�ɂ���bool
	bool spotSlash, abi2;
	bool chargeKnock, abi3;
	bool def1, de1;
	bool def2, de2;
	bool rush, abi4;
	bool slamDunk, abi5;
	bool slashUpper, abi6;
	bool def3, de3;
	bool lastCharge, abi7;
	bool longRangeSlap, abi8;
	bool firePursuit, abi9;
	bool defL, deL;
	bool jumpSlash, abi10;
	bool slashDive, abi11;

	bool downDerive;
	bool down;
	int downCnt;

	bool hit;	//�U�����q�b�g�������ǂ���
	float comboCnt;	//�Z�֐������̎��Ԍo��

	void AbilitySet();
	void ModelLoad();
	void ModelUnLoad();
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	static void AudioLoad();
	Enemy* GetTarget() { return target; }

	void Jcommand();
	void RotateRock();

	Enemy* SetTarget();
	bool GetI_Action() { return jCommandAction; }
	void SetAction(bool Action) { attackAction = Action; }
	void SetArmor(bool Armor) { Armor = Armor; }

	//��i��,��i��
	void Assault();	//�ːi�i��������������Ă��鎞�j

	void SpotSlash();	//���̏�

	void ChargeKnock();//���̏�i�����x���j

	void Default1();	//�Ȃɂ��X�L�����Ȃ��Ƃ�(������)

	void Default2();	//�����X�L�����Ȃ��Ƃ��@�i�ߋ����j

	//�O�i��
	void Rush();		//�O�i�؂�

	void SlamDunk();	//�󒆂Ƀ��[�v���Ă���@������

	void SlashUpper(); //�a�蔲���Ă����ɑł��グ��

	void Default3();	//�X�L���Ȃ�


	//�ŏI�i
	void LastCharge();	//�͋����@������

	void LongLangeSlap();	//�L�͈͂ɓ͂���i�U��

	void FirePursuit();	//�΂����

	void DefaultLast();	//�X�L���Ȃ�


	//��A��i�ڂ̂Ƃ��㉺�ɓG�������Ƃ�
	void JumpSlash();	//��

	void SlashDive();	//��

	//�J�E���^�[
	void Counter();
	void Morter();

	void Duplicate_Reset() { abi1 = true, abi2 = true, abi3 = true, de1 = true, de2 = true, abi4 = false, abi5 = false, abi6 = false, def3 = false, abi7 = false, abi8 = false, abi9 = false, deL = true, abi10 = true, abi11 = true; }

	void SetAbility(bool a, bool b, bool c, bool d, bool e, bool f, bool g, bool h, bool i, bool j, bool k) { assult = a, spotSlash = b, chargeKnock = c, rush = d, slamDunk = e, slashUpper = f, lastCharge = g, longRangeSlap = h, firePursuit = i, jumpSlash = j, slashDive = k; }
};
