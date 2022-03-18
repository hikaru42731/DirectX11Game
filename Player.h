#pragma once
#include "GameObject.h"

// クラス継承によりメンバ変数をまとめる PlayerクラスにGameObjectクラスを継承する
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

	float speed;	//現在の速さ
	float jumpPower; //ジャンプ力
	bool invicible;	//無敵
	bool jump;		//ジャンプしてるか
	bool combo1, combo2, combo3, comboLast;	//何段目の攻撃か
	bool revenge, morter;	//カウンター管理
	bool attackAction;	//攻撃動作中か
	bool jCommandAction;	//ガード、回避中か
	bool interval;	//ガード回避のインターバル中かどうか
	bool diagonal;	//斜め移動してるか
	bool audioPlay;
	bool enemyWarp;
	float intervalCnt;
	float counterCnt;
	float yPrev = 0, yTemp = 0;
	float axel;
	float move;
	float enemyLength;
	float attackSpeed; //攻撃追従速度
	float attackCnt;	//攻撃時間制御
	float invicibleCnt;	//回避時間制御
	float playerY;	//攻撃前のプレイヤーy座標
	float targetY;	//攻撃前のターゲットy座標
	float fallVelocity;	//落下加速
	bool yRock, atkAdd;
	float atkDelayTime;//攻撃してから次の攻撃が出ない間の時間
	bool walk;

	D3DXVECTOR3 prevPos;
	D3DXVECTOR3 vecForward;
	D3DXVECTOR3 vecRight;
	D3DXVECTOR3 cameraFVec;
	D3DXVECTOR3 cameraRVec;
	D3DXVECTOR3 cameraRot;
	D3DXVECTOR3 rotAngle;
	D3DXVECTOR3 atkDir;
	D3DXVECTOR3 targetDir;	//攻撃中に取得するターゲットへの向き
	D3DXVECTOR3 cameraPos;	//こちら側からカメラをいじるとき
	D3DXVECTOR3 cameraTar;	//カメラのターゲットをこちらから指定する時
	D3DXVECTOR3 locusVer= D3DXVECTOR3(GetRotation().x, GetRotation().y + 1.57, GetRotation().z - 1.62);	//縦回転セット
	D3DXVECTOR3 locusSide= D3DXVECTOR3(GetRotation().x, GetRotation().y - 2.7, GetRotation().z);;	//横回転セット
	D3DXVECTOR3 locusDia= D3DXVECTOR3(GetRotation().x, GetRotation().y - 1.57, GetRotation().z + 0.5);;	//斜め回転セット
	float targetLength;			//ターゲットとの距離

	bool slashRun;
	float slashRunSpeed;
	D3DXMATRIX looking;

	D3DXMATRIX rot;

	int animeFrame, animeDodgeFrame, animeAttackFrame, animeFalterFrame;
	float  animeRunRate, animeJumpRate, animeGuardRate, animeDodgeRate, animeAttackRate, animeFalterRate;

	bool assult, abi1;	//ON,OFF用bool,コンボ中に二回出ないようにするbool
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

	bool hit;	//攻撃がヒットしたかどうか
	float comboCnt;	//技関数内部の時間経過

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

	//一段目,二段目
	void Assault();	//突進（距離が少し離れている時）

	void SpotSlash();	//その場

	void ChargeKnock();//その場（発生遅い）

	void Default1();	//なにもスキルがないとき(中距離)

	void Default2();	//何もスキルがないとき　（近距離）

	//三段目
	void Rush();		//三段切り

	void SlamDunk();	//空中にワープしてから叩きつける

	void SlashUpper(); //斬り抜けてから上に打ち上げる

	void Default3();	//スキルなし


	//最終段
	void LastCharge();	//力強く叩きつける

	void LongLangeSlap();	//広範囲に届く一段攻撃

	void FirePursuit();	//火を放つ

	void DefaultLast();	//スキルなし


	//一、二段目のとき上下に敵がいたとき
	void JumpSlash();	//上

	void SlashDive();	//下

	//カウンター
	void Counter();
	void Morter();

	void Duplicate_Reset() { abi1 = true, abi2 = true, abi3 = true, de1 = true, de2 = true, abi4 = false, abi5 = false, abi6 = false, def3 = false, abi7 = false, abi8 = false, abi9 = false, deL = true, abi10 = true, abi11 = true; }

	void SetAbility(bool a, bool b, bool c, bool d, bool e, bool f, bool g, bool h, bool i, bool j, bool k) { assult = a, spotSlash = b, chargeKnock = c, rush = d, slamDunk = e, slashUpper = f, lastCharge = g, longRangeSlap = h, firePursuit = i, jumpSlash = j, slashDive = k; }
};
