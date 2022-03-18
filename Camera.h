#pragma once

#include "GameObject.h"
// カメラクラス ←　ゲームオブジェクトクラス　を継承
class Camera : public GameObject
{
private:
	D3DXMATRIX  viewMatrix,viewRotate, viewPosition, viewTarget;
	D3DXMATRIX ProjectionMatrix;
	D3DXVECTOR3 target;   // 注視点
	D3DXVECTOR3 startPos;	//イベントカメラ用移動スタート座標
	D3DXVECTOR3 beforeCameraPos;	//イベントから復帰する位置指定通常カメラ
	D3DXVECTOR3 beginPos;	//通常カメラ復帰用移動スタート座標
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
	 void Init();   // 初期化関数
	 void Uninit(); // 終了関数
	 void Update(); // 更新関数
	 void Draw();   // 描画関数

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
	D3DXVECTOR3 target;   // 注視点
	float rotateAngle;
public:
	void Init();   // 初期化関数
	void Uninit(); // 終了関数
	void Update(); // 更新関数
	void Draw();   // 描画関数

	D3DXMATRIX GetViewMatrix() { return viewMatrix; }
};

class T_Camera : public GameObject
{
private:
	D3DXMATRIX  viewMatrix, viewRotate, viewPosition, viewTarget;
	D3DXMATRIX projectionMatrix;
	D3DXVECTOR3 target;   // 注視点
	float rotateAngle;
public:
	void Init();   // 初期化関数
	void Uninit(); // 終了関数
	void Update(); // 更新関数
	void Draw();   // 描画関数

	D3DXMATRIX GetViewMatrix() { return viewMatrix; }
};