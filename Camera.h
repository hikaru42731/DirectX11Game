#pragma once

#include "GameObject.h"
// �J�����N���X ���@�Q�[���I�u�W�F�N�g�N���X�@���p��
class Camera : public GameObject
{
private:
	D3DXMATRIX  viewMatrix,viewRotate, viewPosition, viewTarget;
	D3DXMATRIX ProjectionMatrix;
	D3DXVECTOR3 target;   // �����_
	D3DXVECTOR3 startPos;	//�C�x���g�J�����p�ړ��X�^�[�g���W
	D3DXVECTOR3 beforeCameraPos;	//�C�x���g���畜�A����ʒu�w��ʏ�J����
	D3DXVECTOR3 beginPos;	//�ʏ�J�������A�p�ړ��X�^�[�g���W
	bool setPos,posReturn;
	float movePosLen;
	int returnCnt;
	float xLength;
	float zLength;
	D3DXVECTOR3 prevPos;
	float moveLength;

	int targetCnt;
	
	float rotateSpeed;
	D3DXVECTOR3 rotAngle;
	D3DXVECTOR3 dir;
	int cnt;

	float length,moveCameraLength,moveDistance;
	class Enemy* enemy, * exist, *targetEnemy;
	class Player* player;
	class TargetMark* marker;
	class Audio* targetSe;
	bool event;
	bool getLength;
	bool move;
	float prevY;
	bool setY;
	bool enemyEmpty;
	int emptyCnt;

	D3DXVECTOR3 goalPos;
public:
	 void Init();   // �������֐�
	 void Uninit(); // �I���֐�
	 void Update(); // �X�V�֐�
	 void Draw();   // �`��֐�

	 void CameraMove();

	 D3DXMATRIX GetViewMatrix(){ return viewMatrix; }
	 bool CheckView(D3DXVECTOR3 Position);
	 Enemy* GetTarget() { return targetEnemy; }
	 Enemy* GetExist() { return exist; }
	 bool GetEvent() { return event; }
	 void SetEvent(bool Event) { event = Event; }
	 void SetGoal(D3DXVECTOR3 GoalPosition) { move = true; goalPos = GoalPosition; }
	 void SetMove(bool Move) { move = Move; }
	 void SetTarget(D3DXVECTOR3 Target) { target = Target; }
	 void SetTargetCam(bool TargetCamera) { targetCam = TargetCamera; }
	 void SetStartPos(D3DXVECTOR3 Start) { getLength = false; startPos = Start; }
	 void SetRotAngle(D3DXVECTOR3 RotateAngle) { rotAngle = RotateAngle; }
};

class R_Camera : public GameObject
{
private:
	D3DXMATRIX  viewMatrix, viewRotate, viewPosition, viewTarget;
	D3DXMATRIX projectionMatrix;
	D3DXVECTOR3 target;   // �����_
	float rotateAngle;
public:
	void Init();   // �������֐�
	void Uninit(); // �I���֐�
	void Update(); // �X�V�֐�
	void Draw();   // �`��֐�

	D3DXMATRIX GetViewMatrix() { return viewMatrix; }
};

class T_Camera : public GameObject
{
private:
	D3DXMATRIX  viewMatrix, viewRotate, viewPosition, viewTarget;
	D3DXMATRIX projectionMatrix;
	D3DXVECTOR3 target;   // �����_
	float rotateAngle;
public:
	void Init();   // �������֐�
	void Uninit(); // �I���֐�
	void Update(); // �X�V�֐�
	void Draw();   // �`��֐�

	D3DXMATRIX GetViewMatrix() { return viewMatrix; }
};