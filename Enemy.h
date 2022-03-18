#pragma once
#include "GameObject.h"
// ゲームオブジェクトからメンバを継承
class Enemy : public GameObject
{
protected:
	class Player* enemyPlayer;
	class Scene* enemyScene;
	class EnemyAttackJudge* enemyAtk;
	class EBullet* enemyBullet;
	class Camera* enemyCamera;
	//static class AnimationModel* e_LoadModel;
	class AnimationModel* enemyModel;
	class EnemyWarp* enemyWarp;
	class Omen* enemyOmen;
	class AtkWall* enemyWall;
	class Audio* attackSe;
	class Audio* bigAttackSe;
	class Audio* attackChargeSe;
	class Audio* atkWallOmenSe;
	class Audio* atkWallHitSe;

	int cnt;
	bool debugMode;
	int defenceKeep;
	int revengeReset;
	int revengeRand;
	bool warp;
	int atkRand;
	D3DXVECTOR3 aiStartPos;
	D3DXVECTOR3 aiMovedir;
	D3DXVECTOR3 aiDestination;	//Aiの目的地

	D3DXVECTOR3 atkStartPos;
	D3DXVECTOR3 atkMoveDir;
	D3DXVECTOR3 centerDir;	//攻撃時に使用
	float aiMoveLen;
	float aiCnt;
	bool aiAction;
	float aiSpeed;
	float atkMoveLen;
	float centerLen;

	float enemyDefaultCnt;	//Mode(DEFAULT)でいる時間

	float enemyPlayerLength;	//プレイヤーまでの距離
	float enemyAudioVolume;	//プレイヤーまでの距離
	D3DXVECTOR3 enemyPlayerDir;//プレイヤーへの向き
	
	
	bool enemyAtkAction;

	float blendRate;
	float falterRate;
	float dieRate;
	float walkRate;
	float attackRate;
	int animFrame;
	int falterFrame;
	int dieFrame;
	int walkFrame;
	int attackFrame;
	//↑アニメーション管理変数

	float enemyAtkCnt;
	bool unInduct;	//誘導切りされたかどうか
	D3DXVECTOR3 targetPos,startPos;
	D3DXVECTOR3 atkDestination,warpDir;	//攻撃中のワープに使う座標（目的地、始点、方向）
	float atkLen,moveLen,warpSpeed;						//ワープの距離（始点から目的地の距離、始点から現在座標の距離、スピード）
	float chasePow;	//追従距離

	int DiedFrame=0;//死んでからの時間

	enum TYPE
	{
		Enemy1,	//0
		Enemy2, //1
		Enemy3  //2
	};

	void ModelLoad();
	void ModelUnLoad();
	
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void E1Update();
	void E2Update();
	void E3Update();

	void E1Default();
	void E1Attack();
	void E1Chase();
	void E1Revenge();
	void E2Default();
	void E2Attack();
	void E2Revenge();
	void EnemyFalter();
	void EnemyBlow();
	void RevengeAdd();
	void GetDamage();
	void HitWall();

	
	void EnemyRotate();
	void EnemyAttack();
	void ShortRange1();
	void ShortRange2();
	void ShortRange3();
	void MiddleRange1();
	void MiddleRange2();
	void MiddleRange3();
	void LongRange1();
	void LongRange2();
	void LongRange3();
	void Counter1();
	void Counter2();
	void Counter3();
	bool Exist;
	bool GetExist(){ return Exist; }
	bool GetWarp() { return warp; }
	int GetType() { return enemyType; }
	void SetAicnt(int AiCnt) { aiCnt = AiCnt; }
};