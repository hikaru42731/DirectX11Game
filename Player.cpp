#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "Audio.h"
#include "Scene.h"
#include "Input.h"
#include <list>
#include "Bullet.h"
#include "Field.h"
#include "Camera.h"
#include "Enemy.h"
#include "AttackJudge.h"
#include "AnimationModel.h"
#include "Explosion.h"
#include "Game.h"
#include "Title.h"
#include "Locus.h"
#include "Guard.h"
#include "Player.h"

#define FOLLOW (2.0f)
#define mFALTERTIME (30)
#define mBLOWTIME (60)
std::vector<Enemy*> enemyList;
class Audio* Player::playerSe[10];

void Player::AudioLoad()
{
	for (int i = 0; i < 10; i++)
	{
		playerSe[i] = new Audio;
	}
	playerSe[0]->Load("asset\\Audio\\Attack.wav");
	playerSe[1]->Load("asset\\Audio\\HitSword.wav");
	playerSe[2]->Load("asset\\Audio\\HardSword.wav");
	playerSe[3]->Load("asset\\Audio\\Kick.wav");
	playerSe[4]->Load("asset\\Audio\\Guard.wav");
	playerSe[5]->Load("asset\\Audio\\Fire.wav");
	playerSe[6]->Load("asset\\Audio\\Jump.wav");
	playerSe[7]->Load("asset\\Audio\\LastCharge.wav");
	playerSe[8]->Load("asset\\Audio\\Run.wav");
	playerSe[9]->Load("asset\\Audio\\Dive.wav");
}

void Player::AbilitySet()
{
	bool Abl1 = { *Manager::GetAbility1() };
	bool Abl2 = { *Manager::GetAbility2() };
	bool Abl3 = { *Manager::GetAbility3() };
	bool Abl4 = { *Manager::GetAbility4() };
	bool Abl5 = { *Manager::GetAbility5() };
	bool Abl6 = { *Manager::GetAbility6() };
	bool Abl7 = { *Manager::GetAbility7() };
	bool Abl8 = { *Manager::GetAbility8() };
	bool Abl9 = { *Manager::GetAbility9() };
	bool Abl10 = { *Manager::GetAbility10() };
	bool Abl11 = { *Manager::GetAbility11() };
	Player::SetAbility(Abl1, Abl2, Abl3, Abl4, Abl5, Abl6, Abl7, Abl8, Abl9, Abl10, Abl11);	//Managerから受け取ったアビリティ情報をセットする
	// モデル読み込み
}

void Player::ModelLoad()
{
	playerModel = new AnimationModel();
	playerModel->Load("asset\\model\\Player\\Idle.fbx");
	playerModel->LoadAnimation("asset\\model\\Player\\Idle.fbx", "Stop");
	playerModel->LoadAnimation("asset\\model\\Player\\Run.fbx", "Run");
	playerModel->LoadAnimation("asset\\model\\Player\\Jump.fbx", "Jump");
	playerModel->LoadAnimation("asset\\model\\Player\\p_Falter.fbx", "Falter");
	playerModel->LoadAnimation("asset\\model\\Player\\p_Blow.fbx", "Blow");
	playerModel->LoadAnimation("asset\\model\\Player\\Guard.fbx", "Guard");
	playerModel->LoadAnimation("asset\\model\\Player\\Dodge.fbx", "Dodge");
	playerModel->LoadAnimation("asset\\model\\Player\\!OnGround.fbx", "SkyDodge");
	playerModel->LoadAnimation("asset\\model\\Player\\Sliding.fbx", "Assault");
	playerModel->LoadAnimation("asset\\model\\Player\\SpotSlash.fbx", "SpotSlash");
	playerModel->LoadAnimation("asset\\model\\Player\\ChargeKnock.fbx", "ChargeKnock");
	playerModel->LoadAnimation("asset\\model\\Player\\Slamdunk.fbx", "SlamDunk");
	playerModel->LoadAnimation("asset\\model\\Player\\Rush.fbx", "Rush");
	playerModel->LoadAnimation("asset\\model\\Player\\Upper.fbx", "Upper");
	playerModel->LoadAnimation("asset\\model\\Player\\LongRange.fbx", "LongRange");
	playerModel->LoadAnimation("asset\\model\\Player\\LongRangeSlap.fbx", "LongSlap");
	playerModel->LoadAnimation("asset\\model\\Player\\LastCharge.fbx", "LastCharge");
	playerModel->LoadAnimation("asset\\model\\Player\\TripleFiga.fbx", "Triple");
	playerModel->LoadAnimation("asset\\model\\Player\\Default1.fbx", "Def1");
	playerModel->LoadAnimation("asset\\model\\Player\\Default2.fbx", "Def2");
	playerModel->LoadAnimation("asset\\model\\Player\\Default3.fbx", "Def3");
	playerModel->LoadAnimation("asset\\model\\Player\\DefaultLast.fbx", "DefL");
}

void Player::ModelUnLoad()
{
	playerModel->UnLoadAnimation("Stop");
	playerModel->UnLoadAnimation("Run");
	playerModel->UnLoadAnimation("Jump");
	playerModel->UnLoadAnimation("Falter");
	playerModel->UnLoadAnimation("Blow");
	playerModel->UnLoadAnimation("Guard");
	playerModel->UnLoadAnimation("Dodge");
	playerModel->UnLoadAnimation("SkyDodge");
	playerModel->UnLoadAnimation("Assult");
	playerModel->UnLoadAnimation("SpotSlash");
	playerModel->UnLoadAnimation("ChargeKnock");
	playerModel->UnLoadAnimation("SlamDunk");
	playerModel->UnLoadAnimation("Rush");
	playerModel->UnLoadAnimation("Upper");
	playerModel->UnLoadAnimation("LongRange");
	playerModel->UnLoadAnimation("LongSlap");
	playerModel->UnLoadAnimation("LastCharge");
	playerModel->UnLoadAnimation("Triple");
	playerModel->UnLoadAnimation("Def1");
	playerModel->UnLoadAnimation("Def2");
	playerModel->UnLoadAnimation("Def3");
	playerModel->UnLoadAnimation("DefL");

	playerModel->UnLoad();
	delete playerModel;
}

void Player::Init()
{
	ModelLoad();
	AbilitySet();
	//あとでまとめる
	{
		animeRunRate = 0.0f;
		animeJumpRate = 0.0f;
		animeGuardRate = 0.0f;
		animeDodgeRate = 0.0f;
		//アニメーションレート初期化

		objPosition = D3DXVECTOR3(0.0f, 0.0f, -10.0f);
		objRotation = D3DXVECTOR3(0.0f, 1.57f * 2, 0.0f);
		objScale = D3DXVECTOR3(0.02f, 0.02f, 0.02f);
		unWallPos = objPosition;
		//基本情報

		hp = 1000;		//HP
		speed = 0.3f;
		attackPower = 15;	//攻撃力
		deffencePower = 15;	//防御力
		jumpPower = 1.75f;
		onGround = true;//接地している状態で開始
		axel = 0.0f;
		move = 0.05f;
		attackSpeed = 5.0f;

		//ステータスセット

		walk = false;
		interval = false;
		attackAction = false;
		jCommandAction = false;
		pass = false;
		slashRun = false;
		superArmor = false;
		enemyWarp = false;
		//行動のbool初期化
		slashRunSpeed = 0;
		Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "vertexLightingVS.cso");
		Renderer::CreatePixelShader(&objPixelShader, "vertexLightingPS.cso");

		playerScene = Manager::GetScene();
		comboCnt = 0;
		mode = ENCOUNTER;
	}
	
}
void Player:: Uninit()
{
	ModelUnLoad();

	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();	
}

void Player::Update()
{
	playerCamera = playerScene->GetGameObject<Camera>();
	enemyList = playerScene->GetGameObjects<Enemy>();
	cameraFVec = playerCamera->GetForward();
	cameraRVec = playerCamera->GetRight();
	locus = playerScene->GetGameObject<Locus>();

	if (prevPos != objPosition)	//進行方向取得
	{
		moveVec = prevPos - objPosition;
		D3DXVec3Normalize(&moveVec, &moveVec);
	}
	prevPos = objPosition;

	HitWall();	//ステージ外に出たときの処理
	GetDamage();//被弾処理

	if(objPosition.y<=0)	//接地判定
	{
		objPosition.y = 0;
		onGround = true;
		fallVelocity = 1;	//加速の初期化
		jump = false;
	}
	else
	{
		onGround = false;//空中にいる
	}

	if (mode != DEFAULT)playerSe[8]->Stop();	//歩きSE

	if (interval == true)	//ガード回避インターバル
	{
		intervalCnt++;
		if (intervalCnt > 12.5f)	//ガードor回避してから12.5fは再び出せない
		{
			interval = false;
		}
	}

	ColEnemy();	//敵との衝突処理

	switch (mode)
	{
	case DEFAULT:
		PDefault();
		break;
	case ATTACK:
		PAttack();
		break;
	case REVENGE:
		PRevenge();
		break;
	case GUARD:
		PGuard();
		break;
	case INVICIBLE:
		PInvicible();
		break;
	case FALTER:
		PFalter();
		break;
	case BLOW:
		PBlow();
		break;
	case ENCOUNTER:
		playerModel->Update("Stop", "Stop", animeRunRate, animeFrame);
		if ((int)comboCnt % 2 == 0) {
			animeFrame ++;
		}
		comboCnt++;
		if (comboCnt == 500)mode = DEFAULT;
		break;
	default:
		mode = DEFAULT;
		break;
	}
}

void Player::Draw()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	if (!camera->CheckView(objPosition))	return;
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(objVertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(objVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(objPixelShader, NULL, 0);
	
	//マトリクス設定
	D3DXMATRIX world, scale, trans;
	D3DXMatrixScaling(&scale, objScale.x, objScale.y, objScale.z);
	D3DXMatrixRotationYawPitchRoll(&rot,objRotation.y, objRotation.x, objRotation.z);
	D3DXMatrixTranslation(&trans, objPosition.x, objPosition.y, objPosition.z);
	world = scale *rot* trans;
	Renderer::SetWorldMatrix(&world);

	// モデル描画
	playerModel->Draw();
}

void Player::Jcommand()
{
	playerCamera->SetEvent(false);
	pass = false;
	if (Input::GetKeyPress('W') && Input::GetKeyTrigger('J')) {
		playerSe[9]->Stop();
		objRotation.y = cameraRot.y + 1.57 * 2;
		invicibleCnt = 0;
		mode = INVICIBLE;
	}
	else if (Input::GetKeyPress('A') && Input::GetKeyTrigger('J'))
	{
		playerSe[9]->Stop();
		objRotation.y = cameraRot.y + 1.57;
		invicibleCnt = 0;
		mode = INVICIBLE;
	}
	else if (Input::GetKeyPress('S') && Input::GetKeyTrigger('J'))
	{
		playerSe[9]->Stop();
		objRotation.y = cameraRot.y;
		invicibleCnt = 0;
		mode = INVICIBLE;
	}
	else if (Input::GetKeyPress('D') && Input::GetKeyTrigger('J'))
	{
		playerSe[9]->Stop();
		objRotation.y = cameraRot.y + 1.57 * 3;
		invicibleCnt = 0;
		mode = INVICIBLE;
	}
	else if (Input::GetKeyTrigger('J'))
	{
		playerSe[9]->Stop();
		invicibleCnt = 0;
		if (playerCamera->GetTarget() != NULL) { target = playerCamera->GetTarget(); }
		else if (!enemyList.empty()) { target = SetTarget(); }
		else { return; }
		RotateRock();
		playerScene->AddGameObject<Guard>();
		mode = GUARD;
	}
}

void Player::RotateRock()
{
	if (target == NULL)target = SetTarget();
	D3DXVECTOR3 direction = objPosition - target->GetPosition();
	D3DXVec3Normalize(&direction, &direction);
	float vec = direction.x;
	if (objPosition.z  < target->GetPosition().z)objRotation.y = -vec + 1.57 * 2;
	else if (objPosition.z > target->GetPosition().z ) { objRotation.y = vec; }
}

Enemy*  Player::SetTarget()
{
	Scene* scene = Manager::GetScene();
	D3DXVECTOR3 tmp;
	float Dis=0;		//距離を一時的に取得する
	float nearDis=0;	//最も近い距離
	
	for (Enemy* enemy : enemyList)
	{
		if (enemy->Exist == false) { enemy = NULL; }
		else
		{
			tmp = objPosition - enemy->GetPosition();
			Dis = D3DXVec3Length(&tmp);
			if (nearDis == 0 || nearDis > Dis)
			{
				nearDis = Dis;
				nearEnemy = enemy;
			}
		}
	}
	return nearEnemy;
}

//攻撃技
void Player::Assault()
{
	comboCnt++;
	//アニメーション設定
	animeAttackFrame += 0.2f;
	animeAttackRate = 1.0f;
	playerModel->Update("Stop", "Assault", animeAttackRate, animeAttackFrame);
	
	float inertia;	//惰性
	if (comboCnt == 1.0f) { inertia = 0.6f; }

	D3DXVECTOR3 direction = objPosition - target->GetPosition();
	float length = D3DXVec3Length(&direction);
	D3DXVec3Normalize(&direction, &direction);

	if (comboCnt < 20 && attackSpeed>0) {

		if (comboCnt == 3) {	//判定出っぱなしにする
			playerAtk = playerScene->AddGameObject<AttackJudge>(); playerAtk->SetKnock(1.0f, GetForward());
			playerAtk->SetATKDamage(50,attackPower);
			playerAtk->big = 2.5f;
			playerAtk->SetExistTime(20);
			playerAtk->SetRevenge(1);
			playerSe[0]->Play();
		}
		if (comboCnt > 3) {
			playerAtk->SetPosition(objPosition - GetForward() * 2);
			if (ColisionJudge(*playerAtk, *target, playerAtk->big)) { playerSe[3]->Play(); abi1 = false; hit = true; attackAction = false; }
		}
		attackSpeed -= 0.1f;

		if (!ColisionJudge(*this, *target,1.5f))									//4.0fまで近づくまで突進
		{
			objPosition -= direction * attackSpeed;
			attackSpeed = 0;
		}
	}
	else if (comboCnt == 20 || attackSpeed <= 0)
	{
		attackAction = false; 
		atkDelayTime = 40;
	}
	else if (comboCnt >= 21 && comboCnt <= 26 && hit == false)
	{
		if (inertia > 0)inertia--;
		objPosition += direction * inertia;
		animeAttackRate++;
		
	}
	else//HITしなければここにくる
	{
		abi1 = false; 
		animeAttackRate = 0;
		return;
	}
}

void Player::SpotSlash()
{
	if (comboCnt == 0)animeAttackFrame = 0;
	comboCnt++;
	//アニメーション設定
	animeAttackFrame+=1.25f;
	animeAttackRate = 1.0f;
	playerModel->Update("Stop", "SpotSlash", animeAttackRate, animeAttackFrame);

	float inertia;	//惰性
	if (comboCnt == 1.0f) { inertia = 0.6f; }

	D3DXVECTOR3 direction = objPosition - target->GetPosition();
	float length = D3DXVec3Length(&direction);
	D3DXVec3Normalize(&direction, &direction);

	if (comboCnt < 60)
	{
		if (comboCnt < 20 && attackSpeed>0) {
			pass = false;
			attackSpeed -= 0.1f;

			if (!ColisionJudge(*this, *target, 1.5f))
			{
				objPosition -= direction * 1.0;
				attackSpeed = 0;
			}
			else
			{
				comboCnt == 20;
			}
		}
		else if (comboCnt == 20 || attackSpeed <= 0)
		{
			locus->SetRotate(-locusSide,1);

			playerAtk = playerScene->AddGameObject<AttackJudge>();
			playerAtk->SetPosition(objPosition - GetForward() * 4);
			playerAtk->SetKnock(1.0f, GetForward());
			playerAtk->SetATKDamage(25, attackPower);
			playerAtk->big = 5.0f;
			playerAtk->SetExistTime(20);
			playerAtk->SetRevenge(1);
			playerSe[0]->Play();
		}
		else if (comboCnt == 40)
		{
			locus->SetRotate(-locusVer,0);

			playerAtk = playerScene->AddGameObject<AttackJudge>();
			playerAtk->SetPosition(objPosition - GetForward() * 4);
			playerAtk->SetKnock(1.5f, GetForward());	
			playerAtk->SetATKDamage(50, attackPower);
			playerAtk->big = 5.0f;
			playerAtk->SetExistTime(20);
			playerAtk->SetRevenge(1);
			playerSe[0]->Play();
		}
		if (comboCnt > 40)
		{
			if (playerAtk != NULL) {
				if (ColisionJudge(*playerAtk, *target, playerAtk->big)) { playerSe[1]->Play(); abi2 = false; hit = true; attackAction = false; }
			}
		}
	}
	else
	{
		abi2 = false; attackAction = false;
	}
}

void Player::ChargeKnock()
{
	//アニメーション設定
	playerModel->Update("ChargeKnock", "ChargeKnock", animeAttackRate, animeAttackFrame);
	
	if (comboCnt == 0.0f) {  animeAttackRate = 1.0f; animeAttackFrame = 0; }
	if(comboCnt==40)playerSe[2]->Play();
	animeAttackFrame++;
	comboCnt++;
	D3DXVECTOR3 direction = objPosition - target->GetPosition();
	if (comboCnt < 90.0f)
	{
		if (target->GetWarp() == true) { enemyWarp = true; }
		if (comboCnt == 70)
		{
			attackAction = false;
			locus->SetRotate(locusVer, 0);
			if (enemyWarp == false)
			{
				for (Enemy* enemy : enemyList)
				{
					D3DXVECTOR3 dir = objPosition - enemy->GetPosition();
					float InDistance = D3DXVec3Length(&dir);

					if (InDistance <= targetLength + 5.0f)
					{
						playerAtk = playerScene->AddGameObject<AttackJudge>();
						playerAtk->SetPosition(enemy->GetPosition());
						D3DXVECTOR3 DiaAbove = GetForward() - GetUp();
						D3DXVec3Normalize(&DiaAbove, &DiaAbove);
						playerAtk->SetBlow(2.0f, DiaAbove);
						playerAtk->SetATKDamage(150, attackPower);
						playerAtk->big = 1.5f;
						playerAtk->SetExistTime(20);
						playerAtk->SetRevenge(3);
					}

				}
				playerAtk = playerScene->AddGameObject<AttackJudge>();
				playerAtk->SetPosition(target->GetPosition());
				D3DXVECTOR3 DiaAbove = GetForward() - GetUp();
				D3DXVec3Normalize(&DiaAbove, &DiaAbove);
				playerAtk->SetKnock(2.5f, DiaAbove);
				playerAtk->SetATKDamage(150, attackPower);
				playerAtk->SetStop(10);
				playerAtk->big = 1.5f;
				playerAtk->SetExistTime(20);
				playerAtk->SetRevenge(3);

				if (ColisionJudge(*playerAtk, *target, playerAtk->big)) { abi3 = false; hit = true; }
				else { abi3 = false; }
			}
		}
	}
	else 
	{
		animeAttackRate = 0;
		return;
	}
}

void Player::Default1()
{
	playerModel->Update("Def1", "Stop", animeAttackRate, animeAttackFrame);
	if (comboCnt == 0) { animeAttackFrame = 0; animeAttackRate = 0.0f; attackSpeed = 1; }
	comboCnt++;
	//アニメーション設定
	

	D3DXVECTOR3 direction = objPosition - target->GetPosition();
	float length = D3DXVec3Length(&direction);
	D3DXVec3Normalize(&direction, &direction);

	if (comboCnt < 70) {
		pass = false;
		animeAttackFrame++;
		if (attackSpeed > 0) {
			if (!ColisionJudge(*this, *target, 2.5f)) objPosition -= direction;
			else attackSpeed = 0;
		}
		if (comboCnt < 20 ) {

			if (comboCnt == 3) {	//判定出っぱなしにする
				playerAtk = playerScene->AddGameObject<AttackJudge>();
				playerAtk->SetKnock(1.0f, GetForward());
				playerAtk->SetATKDamage(10, attackPower);
				playerAtk->big = 1.5f;
				playerAtk->SetExistTime(20);
				playerAtk->SetRevenge(1);
				locus->SetRotate(locusSide, 1);
			}
			if (comboCnt > 3) {
				playerAtk->SetPosition(objPosition - GetForward() * 2);
			}
		}
		if (comboCnt == 50)
		{
			locus->SetRotate(locusVer, 0);
			locus->SetForwardPow(2);
			playerAtk = playerScene->AddGameObject<AttackJudge>();
			playerAtk->SetPosition(objPosition - GetForward()*4);
			playerAtk->SetKnock(1.5f, GetForward());
			playerAtk->SetATKDamage(20, attackPower);
			playerAtk->big = 4.5f;
			playerAtk->SetExistTime(20);
			playerAtk->SetRevenge(1);
		}
		if (comboCnt > 50) {
			if (playerAtk != NULL) {
				if (ColisionJudge(*playerAtk, *target, playerAtk->big)) {hit = true; }
			}
		}
	}
	else
	{
		de1 = false;
		attackAction = false;
		animeAttackRate = 0;
		return;
	}
}

void Player::Default2()
{
	playerModel->Update("Def2", "Def2", animeAttackRate, animeAttackFrame);
	if (comboCnt == 0) { animeAttackRate = 0.0f; animeAttackFrame = 0.0f; attackSpeed = 1.0f; }
	comboCnt++;
	animeAttackFrame++;
	D3DXVECTOR3 direction = objPosition - target->GetPosition();
	float length = D3DXVec3Length(&direction);
	D3DXVec3Normalize(&direction, &direction);
	if (comboCnt < 70)
	{
		if (attackSpeed > 0) {
			if (combo2 == true)
			{
				if (comboCnt < 30) {
					if (!ColisionJudge(*this, *target,1.5f)) objPosition -= direction; 
					else attackSpeed = 0;
				}
			}
			else
			{
				if (comboCnt < 30) {
					if (!ColisionJudge(*this, *target,1.5f)) objPosition -= direction * 0.75f; 
					else attackSpeed = 0;
				}
			}
		}
		
		if (comboCnt == 30)
		{
			locus->SetRotate(-locusSide, 1);
			locus->SetRotate(-locusVer, 0);
			playerAtk = playerScene->AddGameObject<AttackJudge>();
			playerAtk->SetPosition(objPosition - GetForward());
			playerAtk->SetKnock(1.5f, GetForward());
			playerAtk->SetATKDamage(30, attackPower);
			playerAtk->big = 3.5f;
			playerAtk->SetExistTime(20);
			playerAtk->SetRevenge(2);
		}
		if (playerAtk != NULL)
		{
			if (ColisionJudge(*playerAtk, *target, playerAtk->big)) { de2 = false; hit = true; attackAction = false; }
		}
	}
	else
	{
		de2 = false;
		attackAction = false;
		animeAttackRate = 0;
		return;
	}
}

void Player::Rush()
{
	RotateRock();
	//アニメーション設定
	playerModel->Update("Rush", "Rush", animeAttackRate, animeAttackFrame);
	if (comboCnt == 0.0f) { animeAttackFrame = 0; animeAttackRate = 1.0f; 
	}
	
	comboCnt++;
	animeAttackFrame++;

	D3DXVECTOR3 direction = objPosition - target->GetPosition();
	float length = D3DXVec3Length(&direction);
	
	D3DXVec3Normalize(&direction, &direction);
	if (comboCnt < 20.0f)
	{
		if (length > 3.0f) 
		{
			objPosition -= direction * 1.5f;
		}
	}
	if (comboCnt <= 119.0f) {
		if(comboCnt<60.0f)objPosition.y -= direction.y * 1.0f;
		
		if (comboCnt == 30.0f) {
			locus->SetRotate(locusVer, 0);
			playerAtk = playerScene->AddGameObject<AttackJudge>();
			playerAtk->SetPosition(objPosition - GetForward() * 2);
			playerAtk->SetKnock(0.75f, GetForward());
			playerAtk->SetATKDamage(25,attackPower);
			playerAtk->big = 1.5f;
			playerAtk->SetExistTime(20);
			playerAtk->SetRevenge(1);
			playerSe[1]->Play();
		}
		if (comboCnt == 50.0f)
		{
			locus->SetRotate(locusSide, 1);
			locus->SetForwardPow(4);

			playerAtk = playerScene->AddGameObject<AttackJudge>();
			playerAtk->SetPosition(objPosition - GetForward() * 4);
			playerAtk->SetKnock(0.75f, GetForward());
			playerAtk->SetATKDamage(50,attackPower);
			playerAtk->big = 1.5f;
			playerAtk->SetExistTime(20);
			playerAtk->SetRevenge(1);
			playerSe[1]->Play();
		}
		if (comboCnt == 80.0f)
		{
			locus->SetRotate(locusDia, 2);
			locus->SetForwardPow(6);

			playerAtk = playerScene->AddGameObject<AttackJudge>();
			playerAtk->SetPosition(objPosition - GetForward() * 6);
			D3DXVECTOR3 DiaAbove = GetForward() - GetUp();
			D3DXVec3Normalize(&DiaAbove, &DiaAbove);
			playerAtk->SetBlow(2.5f, DiaAbove);
			playerAtk->SetATKDamage(75,attackPower);
			playerAtk->big = 2.0f;
			playerAtk->SetExistTime(20);
			playerAtk->SetRevenge(2);
			playerSe[1]->Play();
			if (ColisionJudge(*playerAtk, *target, playerAtk->big)) { playerSe[2]->Play(); hit = true; }	//当たればHITをONに
		}
		if (comboCnt == 100)
		{ attackAction = false;	//最終段が出たタイミングかモーションが終わったタイミングでActionをOFFに
		objPosition = objPosition - GetForward() * 3.75; }
	}
	else
	{
		animeAttackFrame = 0;
		animeAttackRate = 0;
		return ;
	}
}

void Player::SlamDunk()
{
	if (comboCnt == 0.0f)
	{
		animeAttackFrame = 10; animeAttackRate = 0.0f; atkDir = objPosition - target->GetPosition(); 
		playerCamera->SetStartPos(playerCamera->GetPosition());
		cameraPos = objPosition - GetForward() * 40.0f+GetRight()*1.5f;
		cameraPos.y = objPosition.y + 2.5f;
		playerCamera->SetGoal(cameraPos);
	}
	playerCamera->SetEvent(true);
	
	//アニメーション設定
	if (comboCnt < 55)
	{
		playerModel->Update("LongRange", "Stop", animeAttackRate, animeAttackFrame);
		
	}
	else if (comboCnt < 100) 
	{
		if (comboCnt == 55) { animeAttackFrame = 10.0f; objPosition -= GetForward() * 7; }
		playerModel->Update("Triple", "Stop", animeAttackRate, animeAttackFrame); 
	}
	else if(comboCnt<150)
	{ 
		if (comboCnt == 100)animeAttackFrame = 0.0f;
		playerModel->Update("SlamDunk", "Stop", animeAttackRate, animeAttackFrame);
	}

		comboCnt++;
	
	D3DXVec3Normalize(&atkDir,&atkDir);
	
	if (comboCnt < 150.0f)
	{
		pass = true;
		if (comboCnt < 25) { if ( (int)comboCnt%2 ==0)animeAttackFrame++; }
		if(comboCnt>25&&comboCnt<55)animeAttackFrame+=2;
		if (comboCnt > 55 && comboCnt < 75)animeAttackFrame += 0;
		if (comboCnt > 35 && comboCnt < 45)
		{
			slashRun = true;
			slashRunSpeed = 3.5f;
			objPosition -= atkDir * slashRunSpeed;
			target->SetKnock(0, D3DXVECTOR3(0, 0, 0));
		}
		if (comboCnt == 46) { slashRun = false; locus->SetRotate(locusSide, 1);}
		{
			if (comboCnt == 60)
			{
				RotateRock();
				playerAtk = playerScene->AddGameObject<AttackJudge>();
				playerAtk->SetPosition(target->GetPosition());
				playerAtk->SetKnock(0.0f, GetForward());
				playerAtk->SetATKDamage(5, attackPower);
				playerAtk->SetExistTime(1);
				playerAtk->SetRevenge(1);
				playerSe[2]->Play();
				
			}
			if (comboCnt == 65)
			{
				RotateRock();
				playerAtk = playerScene->AddGameObject<AttackJudge>();
				playerAtk->SetPosition(target->GetPosition());
				playerAtk->SetKnock(0.0f, GetForward());
				playerAtk->SetATKDamage(5, attackPower);
				playerAtk->SetExistTime(1);
				playerSe[1]->Play();
			}
			if (comboCnt == 70)
			{
				playerAtk = playerScene->AddGameObject<AttackJudge>();
				playerAtk->SetPosition(target->GetPosition());
				playerAtk->SetKnock(0.0f, GetForward());
				playerAtk->SetATKDamage(5, attackPower);
				playerAtk->SetExistTime(1);
				playerAtk->SetRevenge(1);
				playerSe[1]->Play();
			}
			if (comboCnt == 75)
			{
				playerAtk = playerScene->AddGameObject<AttackJudge>();
				playerAtk->SetPosition(target->GetPosition());
				playerAtk->SetKnock(0.0f, GetForward());
				playerAtk->SetATKDamage(5, attackPower);
				playerAtk->SetExistTime(1);
				playerSe[1]->Play();
			}
			if (comboCnt == 85)
			{
				playerAtk = playerScene->AddGameObject<AttackJudge>();
				playerAtk->SetPosition(target->GetPosition());
				playerAtk->SetKnock(0.0f, GetForward());
				playerAtk->SetATKDamage(15, attackPower);
				playerAtk->SetExistTime(1);
				playerAtk->SetRevenge(1);
				playerSe[1]->Play();
			}
		}
		if (comboCnt > 55 && comboCnt < 75) if((int)comboCnt%2==0)animeAttackFrame++;
		if(comboCnt>75&&comboCnt<100) animeAttackFrame+=2; 
		if (comboCnt == 80)
		{
			D3DXVECTOR3 BulletPos = objPosition;
			BulletPos.y = objPosition.y + 1.0f;
			playerBul = playerScene->AddGameObject<Bullet>();
			playerBul->SetPosition(BulletPos);
			playerBul->SetKnock(1.0f, -GetForward());
			playerBul->SetSpeed(1.0f);
			playerBul->SetATKDamage(50, this->attackPower);
			playerSe[5]->Play();
		}

		if(playerBul!=NULL&&ColisionJudge(*playerBul,*target,1.5f))target->EnemyRotate();

		if(comboCnt>100&&comboCnt<130)animeAttackFrame++;
		if (comboCnt > 100 && comboCnt < 120)
		{
			if (!ColisionJudge(*this, *target, 10.0f)) 
			{
				RotateRock();
				atkDir = objPosition - target->GetPosition();
				D3DXVec3Normalize(&atkDir, &atkDir);
				objPosition -= atkDir * 0.75f;
			}
		}
		if (comboCnt == 130)
		{
			locus->SetRotate(locusVer, 0);
			locus->SetForwardPow(10);
			playerAtk = playerScene->AddGameObject<AttackJudge>();
			playerAtk->SetPosition(target->GetPosition());
			D3DXVECTOR3 DiaAbove = GetForward() - GetUp();
			D3DXVec3Normalize(&DiaAbove, &DiaAbove);
			playerAtk->SetBlow(2.5f, DiaAbove);
			playerAtk->SetATKDamage(75, attackPower);
			playerAtk->SetExistTime(20);
			playerAtk->SetRevenge(2);
			if (ColisionJudge(*playerAtk, *target, playerAtk->big)) { playerSe[1]->Play(); hit = true; }	//当たればHITをONに
		}
		if (comboCnt > 130)animeAttackFrame += 2;
		
	}
	else if (comboCnt == 150)
	{
		playerModel->Update("Stop", "Stop", animeAttackRate, animeAttackFrame);
		objPosition -= GetForward() * 10;
	}
	else if (comboCnt = 151)
	{
		pass = false;
		attackAction = false;
		return;
	}
	
}

void Player::SlashUpper()
{
	if (comboCnt == 0.0f)
	{
		animeAttackFrame = 10; animeAttackRate = 0.0f; atkDir = objPosition - target->GetPosition(); 

		cameraPos = objPosition - GetForward() * 40 + GetRight() * 7.5f;
		cameraPos.y = objPosition.y + 10;
		playerCamera->SetStartPos(playerCamera->GetPosition());
		playerSe[2]->Play();
	}
	playerCamera->SetEvent(true);
	playerCamera->SetGoal(cameraPos);

	//アニメーション設定
	
	if (comboCnt < 100)
	{
		playerModel->Update("LongRange", "Stop", animeAttackRate, animeAttackFrame);
		playerCamera->SetTarget(target->GetPosition());
	}
	else
	{
		playerModel->Update("Upper", "Stop", animeAttackRate, animeAttackFrame);
	}

	comboCnt++;

	D3DXVec3Normalize(&atkDir, &atkDir);

	if (comboCnt < 140.0f)
	{
		RotateRock();
		
		pass = true;
		if (comboCnt < 30)if ((int)comboCnt % 2 == 0)animeAttackFrame++;
		{	//1段目
			if (comboCnt >= 30 && comboCnt < 50)
			{
				if (comboCnt == 30)
				{
					locus->SetRotate(locusSide, 1);
					playerAtk = playerScene->AddGameObject<AttackJudge>();
					playerAtk->SetKnock(0.75f, GetForward());
					playerAtk->SetATKDamage(25, attackPower);
					playerAtk->big = 1.5f;
					playerAtk->SetExistTime(20);
					playerAtk->SetRevenge(1);
					playerSe[1]->Play();
				}
				playerAtk->SetPosition(objPosition - GetForward() * 2);

				slashRun = true;
				slashRunSpeed = 1.75f;
				objPosition -= atkDir * slashRunSpeed;
				animeAttackFrame++;
			}

			if (comboCnt == 50)
			{
				target->EnemyRotate();
				objPosition = target->GetPosition()+target->GetRight() * 7.5;
				atkDir = objPosition - target->GetPosition();
				
			}
			
		}
		{	//2段目
			if (comboCnt >= 55 && comboCnt < 75)
			{
				if (comboCnt == 55)
				{
					locus->SetRotate(locusSide, 1);
					playerAtk = playerScene->AddGameObject<AttackJudge>();
					playerAtk->SetKnock(0.75f, GetForward());
					playerAtk->SetATKDamage(25, attackPower);
					playerAtk->big = 1.5f;
					playerAtk->SetExistTime(20);
					playerAtk->SetRevenge(1);
					playerSe[1]->Play();
				}
				playerAtk->SetPosition(objPosition - GetForward() * 2);

				slashRun = true;
				slashRunSpeed = 1.75f;
				objPosition -= atkDir * slashRunSpeed;
				animeAttackFrame--;
			}
			if (comboCnt == 75)
			{
				target->EnemyRotate();
				objPosition = target->GetPosition() -target->GetRight() * 7.5;
				atkDir = objPosition - target->GetPosition();
			}
			
		}
		{	//3段目
			if (comboCnt >= 80 && comboCnt < 100)
			{
				if (comboCnt == 80)
				{
					locus->SetRotate(locusSide, 1);
					playerAtk = playerScene->AddGameObject<AttackJudge>();
					playerAtk->SetKnock(0.75f, GetForward());
					playerAtk->SetATKDamage(25, attackPower);
					playerAtk->big = 1.5f;
					playerAtk->SetExistTime(20);
					playerAtk->SetRevenge(1);
					playerSe[1]->Play();
				}
				playerAtk->SetPosition(objPosition - GetForward() * 2);

				slashRun = true;
				slashRunSpeed = 1.75f;
				objPosition -= atkDir * slashRunSpeed;
				animeAttackFrame++;
			}
			if (comboCnt == 100)
			{
				target->EnemyRotate();
				objPosition = target->GetPosition()+target->GetRight() *7.5;
				atkDir = objPosition - target->GetPosition();
			}
		}
		{	//4段目
			if (comboCnt >= 100)animeAttackFrame++;
			if (comboCnt >= 105 && comboCnt <= 125)
			{
				if (comboCnt == 125)
				{
					locus->SetRotate(locusVer, 0);
					playerAtk = playerScene->AddGameObject<AttackJudge>();
					D3DXVECTOR3 DiaAbove = GetForward() - (GetUp()*2);
					D3DXVec3Normalize(&DiaAbove, &DiaAbove);
					playerAtk->SetKnock(2.0f, DiaAbove);
					playerAtk->SetATKDamage(25, attackPower);
					playerAtk->big = 1.5f;
					playerAtk->SetExistTime(20);
					playerAtk->SetRevenge(2);
					playerSe[3]->Play();
				}
				playerAtk->SetPosition(objPosition - GetForward() * 2);
				if (ColisionJudge(*playerAtk, *target, 1.5f))	hit = true;

				slashRun = true;
				slashRunSpeed = 0.35f;
				objPosition -= atkDir * slashRunSpeed;
				animeAttackFrame++;
			}
		}
	}
	else
	{
		attackAction = false;
		return;
	}

}

void Player::Default3()
{
	playerModel->Update("DefL", "DefL", animeAttackRate, animeAttackFrame);

	if (comboCnt == 0.0f) { animeAttackRate = 1.0f; animeAttackFrame = 0; }
	if (comboCnt == 10)playerSe[2]->Play();
	animeAttackFrame++;
	comboCnt++;
	D3DXVECTOR3 direction = objPosition - target->GetPosition();
	D3DXVec3Normalize(&direction, &direction);
	if (comboCnt < 50.0f)
	{
		if (target->GetWarp() == true) { enemyWarp = true; }
		if (comboCnt < 30) {
			if (!ColisionJudge(*this, *target, 1.5f)) objPosition -= direction * 0.75f;
		}
		if (comboCnt == 30)
		{
			attackAction = false;
			locus->SetRotate(locusSide, 1);
			if (enemyWarp == false)
			{
				playerAtk = playerScene->AddGameObject<AttackJudge>();
				playerAtk->SetPosition(target->GetPosition());
				playerAtk->SetBlow(2.5f, GetForward());
				playerAtk->SetATKDamage(150, attackPower);
				playerAtk->big = 1.5f;
				playerAtk->SetExistTime(20);
				playerAtk->SetRevenge(5);
			}
		}
	}
	else
	{
		attackAction = false;
		combo3 = false;
		animeAttackRate = 0;
		return;
	}
}

void Player::LastCharge()
{
	if (comboCnt == 0.0f)
	{
		animeAttackFrame = 10; animeAttackRate = 0.0f; atkDir = objPosition - target->GetPosition(); 

		cameraPos = objPosition + GetForward() *10;
		cameraPos.y = objPosition.y - 1;
		playerCamera->SetStartPos(playerCamera->GetPosition());
		playerSe[7]->Play();
		
		las=playerScene->AddGameObject<LASTCHARGE>();
		las->SetPosition(target->GetPosition());
		las->SetScale(D3DXVECTOR3(20, 20, 1));
	}
	playerCamera->SetEvent(true);
	playerCamera->SetGoal(cameraPos);
	comboCnt++;
	//アニメーション設定
	playerModel->Update("LastCharge", "Stop", animeAttackRate, animeAttackFrame);
	animeAttackFrame++;
	if (comboCnt < 60)
	{
		D3DXVECTOR3 t_p = objPosition - target->GetPosition();
		targetLength = D3DXVec3Length(&t_p);
		if (comboCnt == 1)
		{
			for (Enemy* enemy : enemyList)
			{
				D3DXVECTOR3 dir = objPosition - enemy->GetPosition();
				float InDistance=D3DXVec3Length(&dir);
				if (InDistance <= targetLength+5.0f)
				{
					playerAtk = playerScene->AddGameObject<AttackJudge>();
					playerAtk->SetPosition(enemy->GetPosition());
					playerAtk->SetKnock(0.0f, GetForward());
				}
			}
			playerAtk = playerScene->AddGameObject<AttackJudge>();
			playerAtk->SetPosition(target->GetPosition());
			playerAtk->SetKnock(0.0f, GetForward());
		}
		if (comboCnt == 50)
		{
			for (Enemy* enemy : enemyList)
			{
				D3DXVECTOR3 dir = objPosition - enemy->GetPosition();
				float InDistance = D3DXVec3Length(&dir);
				if (InDistance <= targetLength+5.0f)
				{
					playerAtk = playerScene->AddGameObject<AttackJudge>();
					D3DXVECTOR3 DiaAbove = GetForward() - GetUp() ;
					D3DXVec3Normalize(&DiaAbove, &DiaAbove);
					playerAtk->SetPosition(enemy->GetPosition());
					playerAtk->SetBlow(3.0f, DiaAbove);
					playerAtk->SetATKDamage(100, attackPower);
					playerAtk->SetRevenge(2);
				}
			}
			playerAtk = playerScene->AddGameObject<AttackJudge>();
			D3DXVECTOR3 DiaAbove = GetForward() - GetUp();
			D3DXVec3Normalize(&DiaAbove, &DiaAbove);
			playerAtk->SetPosition(target->GetPosition());
			playerAtk->big = 1.5f;
			playerAtk->SetStop(30);
			playerAtk->SetBlow(3.0f, DiaAbove);
			playerAtk->SetATKDamage(100, attackPower);
			playerAtk->SetRevenge(10);
		}
	}
	else
	{
		attackAction = false;
		return;
	}
		
}

void Player::LongLangeSlap()
{
	playerModel->Update("LongSlap", "LongSlap", animeAttackRate, animeAttackFrame);

	if (comboCnt == 0.0f) 
	{
		animeAttackRate = 1.0f; animeAttackFrame = 50;
		cameraPos = objPosition - GetForward() * 7.5f;
		cameraPos.y = objPosition.y + 3;
		playerCamera->SetStartPos(playerCamera->GetPosition());
		cameraTar = D3DXVECTOR3(objPosition.x, objPosition.y + 3, objPosition.z);
		playerAtk = playerScene->AddGameObject<AttackJudge>();
		playerAtk->SetStop(40);
		playerAtk->SetDamage(0);
		playerAtk->SetRevenge(0);
		playerAtk->SetPosition(target->GetPosition());
	}
	if (comboCnt == 50)
	{
		cameraPos = objPosition + GetForward() * 7.5f+GetRight()*4.5f;
		cameraPos.y = objPosition.y + 1;
		playerCamera->SetStartPos(playerCamera->GetPosition());
	}
	playerCamera->SetEvent(true);
	playerCamera->SetTarget(cameraTar);
	playerCamera->SetGoal(cameraPos);
	if (comboCnt == 10)playerSe[2]->Play();
	if (comboCnt < 40 &&(int)comboCnt % 2 == 0) { animeAttackFrame++; }
	else if(comboCnt>=40&&comboCnt<60){ animeAttackFrame++; }
	
	comboCnt++;
	if (comboCnt < 110.0f)
	{
		if (target->GetWarp() == true) { enemyWarp = true; }
		if (comboCnt == 55)
		{
			locus->SetScale(D3DXVECTOR3(10, 10, 10));
			locus->SetRotate(locusVer, 3);
			
			if (enemyWarp == false)
			{
				playerAtk = playerScene->AddGameObject<AttackJudge>();
				playerAtk->SetStop(25);
				playerAtk->SetPosition(target->GetPosition());
				playerAtk->SetBlow(3.5f, GetForward());
				playerAtk->SetATKDamage(150, attackPower);
				playerAtk->big = 1.5f;
				playerAtk->SetExistTime(20);
				
				playerAtk->SetRevenge(5);
			}
		}
	}
	else
	{
		attackAction = false;
		combo3 = false;
		animeAttackRate = 0;
		return;
	}
}

void Player::FirePursuit()
{
	//アニメーション設定
	playerModel->Update("Triple", "Triple", animeAttackRate, animeAttackFrame);
	if (comboCnt == 0.0f)
	{
		animeAttackFrame = 0; animeAttackRate = 1.0f; 
		playerCamera->SetStartPos(playerCamera->GetPosition());
	}
	cameraPos = D3DXVECTOR3(target->GetPosition().x + 10, target->GetPosition().y + 3.0f, target->GetPosition().z + target->GetForward().z * 7.5f);
	//camera->SetEvent(true);
	playerCamera->SetPosition(cameraPos);
	comboCnt++;
	animeAttackFrame++;

	D3DXVECTOR3 direction = objPosition - target->GetPosition();
	float length = D3DXVec3Length(&direction);
	D3DXVec3Normalize(&direction, &direction);
	
	if (comboCnt <= 119.0f) {

		D3DXVECTOR3 BulletPos = objPosition;
		BulletPos.y = objPosition.y + 1.0f;

		if (comboCnt == 30.0f) {
			locus->SetRotate(locusVer, 0);
			playerBul = playerScene->AddGameObject<Bullet>();
			playerBul->SetPosition(BulletPos);
			playerBul->SetKnock(1.0f, GetForward());
			playerBul->SetSpeed(0.75);
			playerBul->SetRevenge(1);
			playerBul->SetATKDamage(50, this->attackPower);
			playerSe[5]->Play();
		}
		if (comboCnt == 50.0f)
		{
			Bullet* bul1;
			locus->SetRotate(locusSide, 1);
			bul1 = playerScene->AddGameObject<Bullet>();
			bul1->SetPosition(BulletPos);
			bul1->SetKnock(1.0f, GetForward());
			bul1->SetSpeed(0.75);
			bul1->SetRevenge(1);
			bul1->SetATKDamage(75, attackPower);
			playerSe[5]->Play();
		}
		if (comboCnt == 95.0f)
		{
			Bullet* bul2;
			locus->SetRotate(locusVer, 0);
			bul2 = playerScene->AddGameObject<Bullet>();
			bul2->SetATKDamage(100, this->attackPower);
			bul2->SetPosition(BulletPos);
			bul2->SetBlow(2.5f, GetForward());
			bul2->SetRevenge(10);
			bul2->SetSpeed(1.25);
			playerSe[5]->Play();
		}

		if (comboCnt == 119)
		{
			attackAction = false;	//最終段が出たタイミングかモーションが終わったタイミングでActionをOFFに
			objPosition = objPosition - GetForward() * 3.75;
		}
	}
	else
	{
		animeAttackFrame = 0;
		animeAttackRate = 0;
		return;
	}
}

void Player::DefaultLast()
{
	playerModel->Update("DefL", "DefL", animeAttackRate, animeAttackFrame);

	if (comboCnt == 0.0f) { animeAttackRate = 1.0f; animeAttackFrame = 0; }
	if (comboCnt == 10)playerSe[2]->Play();
	animeAttackFrame++;
	comboCnt++;
	D3DXVECTOR3 direction = objPosition - target->GetPosition();
	D3DXVec3Normalize(&direction, &direction);
	if (comboCnt < 80.0f)
	{
		if (target->GetWarp() == true) { enemyWarp = true; }
		if (comboCnt < 45) {
			if (!ColisionJudge(*this, *target, 1.5f)) objPosition -= direction * 0.75f;
		}
		
		if (comboCnt == 45)
		{
			attackAction = false;
			locus->SetRotate(locusSide, 1);
			if (enemyWarp == false)
			{
				playerAtk = playerScene->AddGameObject<AttackJudge>();
				playerAtk->SetPosition(target->GetPosition());
				playerAtk->SetBlow(3.5f, GetForward());
				playerAtk->SetATKDamage(100, attackPower);
				playerAtk->big = 1.5f;
				playerAtk->SetExistTime(20);
				playerAtk->SetRevenge(2);
			}
		}
	}
	else
	{
		attackAction = false;
		comboLast = false;
		animeAttackRate = 0;
		return;
	}
}

void Player::JumpSlash()
{
	RotateRock();
	if(yRock==false)playerModel->Update("Jump", "Jump", animeAttackRate, animeAttackFrame);
	else playerModel->Update("Rush", "Rush", animeAttackRate, animeAttackFrame);
	if (comboCnt == 0) { 
		playerSe[6]->Play(false);
		animeAttackRate = 0; animeAttackFrame = 0;
		targetDir = objPosition - target->GetPosition();
		D3DXVec3Normalize(&targetDir, &targetDir);
		yRock = false; atkAdd = false;
	}
	comboCnt++;
	animeAttackFrame++;
	if (comboCnt < 50)
	{
		if (yRock==false&&objPosition.y >= target->GetPosition().y-1.0f&&objPosition.y<=target->GetPosition().y+1.0f)
		{
			yRock = true;
			animeAttackRate = 1.0f;
			comboCnt = 30;
			animeAttackFrame = 70;
		}

		if (yRock == false)
		{
			objPosition.y += 0.5f;
			objPosition.x -= targetDir.x*0.5f;
			objPosition.z -= targetDir.z*0.5f;
		}else
		{
			playerAtk = playerScene->AddGameObject<AttackJudge>();
			playerAtk->SetPosition(objPosition - GetForward() * 2);
			playerAtk->big = 2.5f;
			playerAtk->SetStop(0);
			playerAtk->SetBlow(1.0f, GetForward());
			playerAtk->SetATKDamage(30, attackPower);
			playerAtk->SetRevenge(1);
			targetDir = objPosition - target->GetPosition();
			D3DXVec3Normalize(&targetDir, &targetDir);
			objPosition -= targetDir * 1.25f;
			comboCnt = 1;
		}
		if (playerAtk != NULL)
		{
			if (ColisionJudge(*playerAtk, *target, playerAtk->big)) { playerSe[1]->Play(false); comboCnt = 50; hit = true; abi1 = false; }
		}
	}
	else
	{
		attackAction = false;
	}
	
}

void Player::SlashDive()
{
	RotateRock();
	playerModel->Update("Jump", "Dodge", animeAttackRate, animeAttackFrame);
	
	//アニメーション設定
	if (comboCnt > 5) 
	{
		animeAttackFrame += 3;
	}
	if(animeAttackRate<1.0)	animeAttackRate += 0.2f;
	if(comboCnt==0)playerSe[9]->Play(false);
	comboCnt++;

	D3DXVECTOR3 direction = objPosition - target->GetPosition();
	float length = D3DXVec3Length(&direction);
	D3DXVec3Normalize(&direction, &direction);

	if (comboCnt < 26) {
		if (!ColisionJudge(*target, *this, 2.5f))
		{
			objPosition -= direction*2.5 ;
		}
		if (comboCnt == 10)
		{
			axel = 0;
			locus->SetRotate(locusVer, 4);
			playerAtk = playerScene->AddGameObject<AttackJudge>();
			playerAtk->SetPosition(objPosition - GetForward() * 2);
			playerAtk->big = 2.5f;
			playerAtk->SetBlow(1.0f, GetForward());
			playerAtk->SetATKDamage(30, attackPower);
			playerAtk->SetRevenge(1);
			playerAtk->SetExistTime(5);
		}
		if (comboCnt == 15)
		{
			locus->SetRotate(locusVer, 4);
			playerAtk = playerScene->AddGameObject<AttackJudge>();
			playerAtk->SetPosition(objPosition - GetForward() * 2);
			playerAtk->big = 2.5f;
			playerAtk->SetBlow(1.0f, GetForward());
			playerAtk->SetATKDamage(30, attackPower);
			playerAtk->SetRevenge(1);
			playerAtk->SetExistTime(5);
		}
		if (comboCnt == 20)
		{
			locus->SetRotate(locusVer, 4);
			playerAtk = playerScene->AddGameObject<AttackJudge>();
			playerAtk->SetPosition(objPosition - GetForward() * 2);
			playerAtk->big = 2.5f;
			playerAtk->SetBlow(1.0f, GetForward());
			playerAtk->SetATKDamage(30, attackPower);
			playerAtk->SetRevenge(1);
			playerAtk->SetExistTime(5);
		}
		if (comboCnt == 25)
		{
			locus->SetRotate(locusVer, 4);
			playerAtk = playerScene->AddGameObject<AttackJudge>();
			playerAtk->SetPosition(objPosition - GetForward() * 2);
			playerAtk->big = 2.5f;
			playerAtk->SetBlow(1.0f, GetForward());
			playerAtk->SetATKDamage(30, attackPower);
			playerAtk->SetRevenge(1);
			playerAtk->SetExistTime(5);
		}
		
	}
	else
	{
		locus->SetRotate(locusVer, 4);
		if (ColisionJudge(*target, *playerAtk, 2.5f))hit = true;
		atkDelayTime = 30;
		attackAction = false;
	}
}

void Player::Counter()
{
	playerModel->Update("Upper", "Stop", animeAttackRate, animeAttackFrame);

	if (comboCnt == 0.0f) { 
		antiInduction = true;
		animeAttackFrame = 10; animeAttackRate = 0.0f; atkDir = objPosition - target->GetPosition(); 
	}
	
	comboCnt++;
	animeAttackFrame++;
	if (comboCnt < 60)
	{
		if (comboCnt < 50)
		{
			objRotation.y += 0.08f;
		}
		
		if (target->GetWarp() == false&&enemyLength<15)
		{
			if (comboCnt == 20)
			{
				locus->SetRotate(locusSide, 1);
				playerAtk = playerScene->AddGameObject<AttackJudge>();
				playerAtk->SetPosition(target->GetPosition());
				playerAtk->big = 1.5f;
				playerAtk->SetStop(0);
				playerAtk->SetKnock(0.0f, GetForward());
				playerAtk->SetATKDamage(0, attackPower);
				playerAtk->SetRevenge(0);
				playerSe[3]->Play();
			}

			if (comboCnt == 50)
			{
				RotateRock();
				locus->SetRotate(locusVer, 0);
				playerAtk = playerScene->AddGameObject<AttackJudge>();
				D3DXVECTOR3 DiaAbove = GetForward() - GetUp();
				D3DXVec3Normalize(&DiaAbove, &DiaAbove);
				playerAtk->SetPosition(target->GetPosition());
				playerAtk->big = 1.5f;
				playerAtk->SetStop(0);
				playerAtk->SetBlow(3.0f, DiaAbove);
				playerAtk->SetATKDamage(100, attackPower);
				playerAtk->SetRevenge(0);
				playerSe[3]->Play();
				antiInduction = false;
			}

			if (comboCnt > 50 && playerAtk != NULL)
			{
				if (ColisionJudge(*playerAtk, *target, playerAtk->big * 2))
				{
					hit = true;
				}
			}
		}
	}
	else
	{
		attackAction = false;
	}

}

void Player::Morter()
{
	RotateRock();
	playerCamera->SetEvent(true);
	if(comboCnt<100) playerCamera->SetTarget(target->GetPosition());

	//アニメーション設定
	if(comboCnt<90)playerModel->Update("Upper", "Upper", animeAttackRate, animeAttackFrame);
	else playerModel->Update("ChargeKnock", "ChargeKnock", animeAttackRate, animeAttackFrame);

	if (comboCnt < 105 && Input::GetKeyTrigger('S'))
	{
		downDerive = true;
	}
	if (comboCnt == 0.0f) { antiInduction = true; downDerive = false; animeAttackFrame = 0; animeAttackRate = 1.0f; playerCamera->SetStartPos(playerCamera->GetPosition()); cameraPos = objPosition; cameraPos.y = objPosition.y + 25; }
	comboCnt++;
	if (comboCnt == 20.0f) { objPosition = target->GetPosition() + target->GetForward() * 2; objPosition.y = target->GetPosition().y - 1; }
	
	if (comboCnt > 20 && comboCnt < 80)animeAttackFrame++;
	if (comboCnt > 105 &&comboCnt<125&& downDerive == true)animeAttackFrame+=2;
	D3DXVECTOR3 direction = objPosition - target->GetPosition();
	float length = D3DXVec3Length(&direction);
	D3DXVec3Normalize(&direction, &direction);

	if (comboCnt == 30.0f)
	{
		locus->SetRotate(locusSide, 1);
		playerAtk = playerScene->AddGameObject<AttackJudge>();
		playerAtk->SetPosition(objPosition - GetForward() * 2);
		playerAtk->SetBlow(2.5f, GetForward());
		playerAtk->SetATKDamage(25, attackPower);
		playerAtk->big = 3.5f;
		playerAtk->SetExistTime(20);
		playerAtk->SetRevenge(0);
		playerSe[3]->Play();
	}
	if (comboCnt > 30 && comboCnt < 60) { animeAttackRate = 0; animeAttackFrame = 15; }
	if (comboCnt == 60.0f) {	objPosition = target->GetPosition() + target->GetForward() * 2; objPosition.y = target->GetPosition().y - 1; }
	if (comboCnt == 65.0f)
	{
		locus->SetRotate(locusVer, 0);
		playerAtk = playerScene->AddGameObject<AttackJudge>();
		playerAtk->SetPosition(objPosition - GetForward() * 4);
		playerAtk->SetBlow(3.5f, -GetUp());
		playerAtk->SetATKDamage(50, attackPower);
		playerAtk->big = 3.5f;
		playerAtk->SetExistTime(20);
		playerAtk->SetRevenge(0);
		playerSe[3]->Play();
	}
	if (comboCnt == 70)animeAttackFrame--;
	
	if (comboCnt == 105) 
	{ 
		animeAttackRate = 0;  objPosition = target->GetPosition() + target->GetForward() * 2; cameraPos = objPosition + GetRight() * 10.0f; playerCamera->SetStartPos(playerCamera->GetPosition());
		vecRight = GetRight();
		if (downDerive == false) { animeAttackFrame = 70; }
		else { animeAttackFrame = 40; }
		down = false;
	}
	
	if(down==false)playerCamera->SetGoal(cameraPos);
	
	if (downDerive == false)
	{
		if (comboCnt > 110) {
			if (down == false)
			{
			objPosition -= D3DXVECTOR3(0,1,0) * 0.65f;
			objRotation.z +=0.6f;
			cameraTar.y -= 0.5f;
			playerCamera->SetPosition(objPosition + (vecRight*7.5 + D3DXVECTOR3(0, 1, 0) * 5.0f));
			}
		};
		if ((int)comboCnt % 10 == 0 && comboCnt > 100)
		{
			if (objPosition.y > 2.0f)
			{
				locus->SetRotate(locusVer, 0);
				locus->SetForwardPow(2);
				playerAtk = playerScene->AddGameObject<AttackJudge>();
				playerAtk->SetPosition(target->GetPosition());
				playerAtk->SetKnock(1.25f, D3DXVECTOR3(0, 1, 0));
				playerAtk->SetATKDamage(10, attackPower);
				playerAtk->big = 3.5f;
				playerAtk->SetExistTime(20);
				playerAtk->SetRevenge(0);
				playerSe[1]->Play();
			}
			else
			{
				if (down == false)
				{
					downCnt = 0;
					locus->SetRotate(locusVer, 0);
					locus->SetForwardPow(2);
					playerAtk = playerScene->AddGameObject<AttackJudge>();
					playerAtk->SetPosition(target->GetPosition());
					playerAtk->SetBlow(2.0f, GetForward());
					playerAtk->SetATKDamage(30, attackPower);
					playerAtk->big = 3.5f;
					playerAtk->SetExistTime(20);
					playerAtk->SetRevenge(0);
					playerSe[1]->Play();
					if (ColisionJudge(*playerAtk, *target, playerAtk->big)) { hit = true; }	//当たればHITをONに
					down = true;
				}
				else
				{ 
					playerCamera->SetEvent(false);
					objRotation.z = 0;
					downCnt++;
				}
				if (downCnt == 3) 
				{
					antiInduction = false;
					attackAction = false;
				}
			}
		}
	}
	else
	{
		if(comboCnt>105)playerCamera->SetPosition(objPosition + (GetForward() * 5 + GetUp() * 10));
		if (comboCnt == 125)
		{
			locus->SetRotate(locusVer, 0);
			playerAtk = playerScene->AddGameObject<AttackJudge>();
			playerAtk->SetPosition(target->GetPosition());
			playerAtk->SetBlow(3.5f, GetUp() + GetForward());
			playerAtk->SetATKDamage(150, attackPower);
			playerAtk->big = 3.5f;
			playerAtk->SetExistTime(20);
			playerAtk->SetStop(50);
			playerAtk->SetRevenge(0);
			playerSe[2]->Play();
		}
		if (comboCnt == 175)
		{
			fallVelocity = 1;
			antiInduction = false;
			attackAction = false;
		}
	}
}
	
void Player::PDefault()
{
	pass = true;		//敵との接触判定を無くす
	superArmor = false;		//スーパーアーマー無効
	enemyWarp = false;	//DEFAULTでは敵のワープの有無は関係ない
	animeFrame++;
	if (onGround == true) {	//地上移動
		animeJumpRate = 0;
		playerModel->Update("Stop", "Run", animeRunRate, animeFrame);

		axel = 0.0f;
		walk = false;
		//四方向移動制御
		if (colWall != true) {
			{
				if (diagonal == true) { speed = 0.15f; }
				else { speed = 0.3f; }
				cameraRot = playerCamera->GetRotation();

				if (Input::GetKeyPress('A'))
				{
					walk = true;
					diagonal = false;

					objPosition -= cameraRVec * speed;
					if (objRotation.y < cameraRot.y + 1.57) objRotation.y += 0.15f;
					if (objRotation.y > cameraRot.y + 1.57) objRotation.y -= 0.15f;
					if (objRotation.y == cameraRot.y + 1.57)objRotation.y = cameraRot.y + 1.57;
				}
				if (Input::GetKeyPress('D'))
				{
					walk = true;
					diagonal = false;

					objPosition += cameraRVec * speed;
					objRotation.y < cameraRot.y + 1.57 ? objRotation.y -= 0.15f : objRotation.y += 0.15f;
					if (objRotation.y >= cameraRot.y + 1.57 * 3)objRotation.y = cameraRot.y + 1.57 * 3;
					if (objRotation.y <= cameraRot.y - 1.57)objRotation.y = cameraRot.y + 1.57 * 3;
				}

				if (Input::GetKeyPress('W'))
				{
					walk = true;
					diagonal = false;

					objPosition += cameraFVec * speed;
					if(!Input::GetKeyPress('A')&& !Input::GetKeyPress('D') )	objRotation.y = cameraRot.y + 1.57 * 2;
				}

				if (Input::GetKeyPress('S'))
				{
					walk = true;
					diagonal = false;

					objPosition -= cameraFVec * speed;
					objRotation.y < cameraRot.y + 1.57 * 2 ? objRotation.y -= 0.15f : objRotation.y += 0.15f;
					if (objRotation.y <= cameraRot.y)  objRotation.y = cameraRot.y;
					if (objRotation.y >= cameraRot.y + 1.57 * 4)  objRotation.y = cameraRot.y;
				}
			}
			//ななめ回転制御
			{
				if (Input::GetKeyPress('W') && Input::GetKeyPress('A'))
				{
					diagonal = true;
					if (objRotation.y < cameraRot.y + 1.57 * 1.5f) objRotation.y += 0.15f;
					if (objRotation.y > cameraRot.y + 1.57 * 1.5f) objRotation.y -= 0.15f;
					if (objRotation.y == cameraRot.y + 1.57 * 1.5f)objRotation.y = cameraRot.y + 1.57 * 1.5f;
				}
				if (Input::GetKeyPress('W') && Input::GetKeyPress('D'))
				{
					diagonal = true;
					objRotation.y < cameraRot.y + 1.57 ? objRotation.y -= 0.15f : objRotation.y += 0.15f;
					if (objRotation.y >= cameraRot.y + 1.57 * 2.5f)objRotation.y = cameraRot.y + 1.57 * 2.5f;
					if (objRotation.y <= cameraRot.y - 1.57 * 1.5f)objRotation.y = cameraRot.y + 1.57 * 2.5f;
				}
				if (Input::GetKeyPress('S') && Input::GetKeyPress('A'))
				{
					diagonal = true;
					if (objRotation.y < cameraRot.y + 1.57 * 0.5f) objRotation.y += 0.15f;
					if (objRotation.y > cameraRot.y + 1.57 * 0.5f) objRotation.y -= 0.15f;
					if (objRotation.y == cameraRot.y + 1.57 * 0.5f)objRotation.y = cameraRot.y + 1.57 * 0.5f;
				}
				if (Input::GetKeyPress('S') && Input::GetKeyPress('D'))
				{
					diagonal = true;
					objRotation.y < cameraRot.y + 1.57 * 2 ? objRotation.y -= 0.15f : objRotation.y += 0.15f;
					if (objRotation.y <= cameraRot.y + 1.57 * 3.5f)objRotation.y = cameraRot.y + 1.57 * 3.75f;
					if (objRotation.y >= cameraRot.y - 1.57 * 0.5f)objRotation.y = cameraRot.y + 1.57 * 3.75f;
				}
			}
			//移動相殺制御
			{
				if (Input::GetKeyPress('W') && Input::GetKeyPress('S')) walk = false;
				if (Input::GetKeyPress('A') && Input::GetKeyPress('D')) walk = false;
			}
		}
		if (Input::GetKeyTrigger(VK_SPACE) && jump == false) {
			playerSe[6]->Play(false);
			jump = true;
		}

		if (walk == true)
		{
			if (animeFrame % 50 == 0)playerSe[8]->Play();
			animeRunRate += 0.1;
			if (animeRunRate > 1) animeRunRate = 1;
		}
		else if (walk == false)
		{
			playerSe[8]->Stop();
			animeRunRate -= 0.1f;
			if (animeRunRate < 0) animeRunRate = 0;
		}
	}
	if (onGround == false)	//空中移動
	{
		if (jump == false)
		{
			fallVelocity+=0.1f;
			objPosition.y -= 0.25f*fallVelocity;
		}
		playerModel->Update("Stop", "Jump", animeJumpRate, animeFrame);
		animeJumpRate = 1.0f;
		if (Input::GetKeyPress('D'))
		{
			cameraRot = playerCamera->GetRotation();
			objPosition += cameraRVec * speed * 0.7;
			objRotation.y < cameraRot.y + 1.57 ? objRotation.y -= 0.15f : objRotation.y += 0.15f;
			if (objRotation.y >= cameraRot.y + 1.57 * 3)objRotation.y = cameraRot.y + 1.57 * 3;
			if (objRotation.y <= cameraRot.y - 1.57)objRotation.y = cameraRot.y + 1.57 * 3;
		}
		if (Input::GetKeyPress('A'))
		{
			cameraRot = playerCamera->GetRotation();
			objPosition -= cameraRVec * speed * 0.7;
			if (objRotation.y < cameraRot.y + 1.57) objRotation.y += 0.15f;
			if (objRotation.y > cameraRot.y + 1.57) objRotation.y -= 0.15f;
			if (objRotation.y == cameraRot.y + 1.57)objRotation.y = cameraRot.y + 1.57;
		}

		D3DXVECTOR3 forward = GetForward();

		if (Input::GetKeyPress('S'))
		{
			cameraRot = playerCamera->GetRotation();
			objPosition -= cameraFVec * speed * 0.7;
			objRotation.y < cameraRot.y + 1.57 * 2 ? objRotation.y -= 0.15f : objRotation.y += 0.15f;
			if (objRotation.y <= cameraRot.y)  objRotation.y = cameraRot.y;
			if (objRotation.y >= cameraRot.y + 1.57 * 4)  objRotation.y = cameraRot.y;
		}
		if (Input::GetKeyPress('W'))
		{
			cameraRot = playerCamera->GetRotation();
			objPosition += cameraFVec * speed * 0.7;
			if (objRotation.y < cameraRot.y + 1.57 * 2) objRotation.y += 0.15f;
			if (objRotation.y > cameraRot.y + 1.57 * 2) objRotation.y -= 0.15f;
			if (objRotation.y == cameraRot.y + 1.57 * 2)objRotation.y = cameraRot.y + 1.57 * 2;
		}
		if (animeJumpRate > 1.0f) animeJumpRate = 1.0f;
		if (animeJumpRate < 0.0f) animeJumpRate = 0.0f;
	}
	if (jump == true && onGround == true)
	{
		axel = jumpPower;
		objPosition.y += axel;
	}
	if (jump == true && onGround == false)
	{
		if (axel > -0.7f)
		{
			axel -= 0.1;
		}
		objPosition.y += axel;
	}
	//ガードor回避
	if (jCommandAction == false)
	{
		Jcommand();
	}

	//攻撃ボタン押したとき
	if (Input::GetKeyTrigger('L'))
	{
		atkDelayTime = 0;	//コンボ間カウントを0でセット
		comboCnt = 0;			//技関数内部でのカウントを0でセット
		pass = false;
		Duplicate_Reset();	//技を2度同じものが出ないように管理するboolを全てtrueにセットする関数（boolの名前は後ほど変更）
		{
			if (assult == false || spotSlash == false || chargeKnock == false) def1 = true;
			if (spotSlash == false && chargeKnock == false)def2 = true;
			if (rush == false && slamDunk == false && slashUpper == false)def3 = true;
			if (lastCharge == false && longRangeSlap == false && firePursuit == false)defL = true;
		}
		{
			if (playerCamera->GetTarget() != NULL) { target = playerCamera->GetTarget(); }
			else if (!enemyList.empty()) { target = SetTarget(); }
			else { return; }
		}	//ターゲット指定

		RotateRock();//敵を向く

		D3DXVECTOR3 dirtotarget = objPosition - target->GetPosition();
		playerY = objPosition.y;
		targetY = target->GetPosition().y;
		enemyLength = D3DXVec3Length(&dirtotarget);
		combo1 = true;
		combo2 = false;
		combo3 = false;
		comboLast = false;
		animeAttackRate = 0;
		attackAction = true;	//技の動作中か（攻撃ボタンを押したタイミングでtrueにし、falseは技関数の最後の当たり判定が出たタイミングで行う）
		mode = ATTACK;
	}
}

void Player::PAttack()
{
	if (jCommandAction == false)
	{
		Jcommand();	//どこからでも回避かガードでキャンセルできるようにする
	}

	if (target == NULL)mode = DEFAULT;	//敵がいなければ攻撃モードから抜ける
	else {
		{//一段目処理
			//何段目の攻撃か　優先度高い技はONになってるか　適正な距離か
			if (attackAction == true)
			{
				if (combo1 == true && jumpSlash == true && playerY + 5.0f < targetY)
				{
					JumpSlash();
				}
				else if (combo1 == true && slashDive == true && playerY > targetY + 5.0f)
				{
					SlashDive();
				}
				else if (combo1 == true && assult == true && enemyLength > 15.0f && abi1 == true)
				{
					attackSpeed = FOLLOW;
					Assault();
				}
				else if (combo1 == true && spotSlash == true && enemyLength > 7.5f && abi2 == true)
				{
					attackSpeed = FOLLOW / 2;
					SpotSlash();
				}
				else if (combo1 == true && chargeKnock == true && enemyLength <= 7.5f && abi3 == true)
				{
					ChargeKnock();
				}
				else if (combo1 == true && def1 == true && enemyLength > 15.0f)
				{
					Default1();
				}
				else if (combo1 == true && def2 == true && enemyLength <= 15.0f)
				{
					Default2();
				}
			}
			else
			{
				if (combo1 == true && hit == true)
				{
					if (Input::GetKeyTrigger('L'))
					{
						D3DXVECTOR3 dirtotarget = objPosition - target->GetPosition();
						enemyLength = D3DXVec3Length(&dirtotarget);
						comboCnt = 0;		//カウントリセット
						RotateRock();
						pass = false;	//敵との判定干渉をONに
						attackAction = true;	//アクションしている状態にする
						combo2 = true;	//次のアクションをONにする
						hit = false;	//HITを初期化
						combo1 = false;	//今まで出していたアクションをOFFに
					}
				}
			}
		}

		{//二段目処理
			if (attackAction == true)
			{


				if (combo2 == true && spotSlash == true && abi2 == true && enemyLength <= 10.5f)
				{
					attackSpeed = FOLLOW / 1.5;
					SpotSlash();
				}
				else if (combo2 == true && assult == true && abi1 == true)
				{
					attackSpeed = FOLLOW;
					Assault();
				}

				else if (combo2 == true && def1 == true && de1 == true)
				{
					Default1();
				}
				else if (combo2 == true && def2 == true && de2 == true)
				{
					Default2();
				}
			}
			else
			{
				if (hit == true && combo2 == true)
				{
					if (Input::GetKeyTrigger('L'))
					{
						int randam = rand() % 3;
						if (randam == 0)
						{
							if (rush == true)abi4 = true;
							else if (slamDunk == true)abi5 = true;
							else if (slashUpper == true)abi6 = true;
						}
						else if (randam == 1)
						{
							if (slamDunk == true)abi5 = true;
							else if (slashUpper == true)abi6 = true;
							else if (rush == true)abi4 = true;
						}
						else
						{
							if (slashUpper == true)abi6 = true;
							else if (rush == true)abi4 = true;
							else if (slamDunk == true)abi5 = true;
						}

						superArmor = true;
						comboCnt = 0;
						RotateRock();
						pass = false;
						attackAction = true;
						combo3 = true;
						hit = false;
						combo2 = false;
						playerCamera->SetEvent(true);
					}
				}
			}
		}
		{//三段目処理
			if (attackAction == true) {
				if (combo3 == true && abi4 == true)
				{
					Rush();
				}
				else if (combo3 == true && abi5 == true)
				{
					SlamDunk();
				}
				else if (combo3 == true && abi6 == true)
				{
					SlashUpper();
				}
				else if (combo3 == true)
				{
					Default3();
				}
			}
			else {
				if (hit == true && combo3 == true)
				{
					if (Input::GetKeyTrigger('L'))
					{
						D3DXVECTOR3 dirtotarget = objPosition - target->GetPosition();
						enemyLength = D3DXVec3Length(&dirtotarget);
						int randam = rand() % 2;
						if (enemyLength < 25.5)
						{
							if (lastCharge == true)abi7 = true;
							else if (longRangeSlap == true)abi8 = true;
							else if (firePursuit == true)abi9 = true;
							else defL;
						}
						else if (enemyLength >= 25.5f && randam == 0)
						{
							if (longRangeSlap == true)abi8 = true;
							if (firePursuit == true)abi9 = true;
							else if (lastCharge == true)abi7 = true;
							else defL;
						}
						else
						{
							if (firePursuit == true)abi9 = true;
							else if (longRangeSlap == true)abi8 = true;
							else if (lastCharge == true)abi7 = true;
							else defL;
						}
						superArmor = true;
						comboCnt = 0;
						RotateRock();
						pass = false;
						attackAction = true;
						comboLast = true;
						hit = false;
						combo3 = false;
						playerCamera->SetEvent(true);
					}
				}
			}
		}

		{//最終段処理
			if (attackAction == true) {
				if (comboLast == true && abi7 == true)
				{
					LastCharge();
					return;
				}
				else if (comboLast == true && abi8 == true)
				{
					LongLangeSlap();
					return;
				}
				else if (comboLast == true && abi9 == true)
				{
					FirePursuit();
					return;
				}
				else if (comboLast == true)
				{
					DefaultLast();
					return;
				}
			}
		}
	}
	{//攻撃ボタンが押されなかった時の処理
		if (attackAction != true)
		{
			superArmor = false;
			playerCamera->SetEvent(false);
			if (animeAttackRate > 0)animeAttackRate--;
			atkDelayTime++;
			locus->SetScale(D3DXVECTOR3(2.0, 2.0, 2.0));
			playerSe[9]->Stop();
			
			if (atkDelayTime > 50.0f)
			{
				//boolの初期化をしないといけない
				attackAction = false;
				combo1 = false;
				combo2 = false;
				combo3 = false;
				comboLast = false;
				hit = false;
				comboCnt = 0;
				pass = false;
				mode = DEFAULT;
			}
		}
		else
		{
			atkDelayTime = 0;
		}
	}
}

void Player::PGuard()
{
	if (interval == false)
	{
		pass = true;
		objRotation.z = 0;
		bool Add;	//ガード中に追加攻撃されたか
		if (invicibleCnt == 0) 
		{ 
			Add = false; knockBack = false; blowBack = false; revenge = false; morter = false; 
		
		}
		playerModel->Update("Stop", "Guard", animeGuardRate, animeFrame);
		jCommandAction = true;
		invicibleCnt++;

		if (invicibleCnt < 35) {	//ガード中の処理

			animeGuardRate = 1.0f;
			if (knockPow > 0)
			{
				if (audioPlay == false)
				{
					playerSe[4]->Play(false);
					playerScene->AddGameObject<Guard>();
					audioPlay = true;
				}
				knockPow -= 0.2f;

				RotateRock();
				
				Add = true;
				
				objPosition -= knockVector * knockPow;
			}
			else
			{
				Add = false;
				audioPlay = false;
			}
			if (Add == true)
			{
				target->SetAicnt(-100);
				invicibleCnt = 0;
				counterCnt = 35;
	
			}
			if (counterCnt > 0)
			{
				counterCnt--;
				if (Input::GetKeyTrigger('L'))
				{
					atkDelayTime = 0;	//コンボ間カウントを0でセット
					comboCnt = 0;			//技関数内部でのカウントを0でセット
					pass = false;
					{
						if (playerCamera->GetTarget() != NULL) { target = playerCamera->GetTarget(); }
						else if (!enemyList.empty()) { target = SetTarget(); }
						else { return; }
					}	//ターゲット指定
					D3DXVECTOR3 dir = objPosition - target->GetPosition();
					float length = D3DXVec3Length(&dir);
					if (length < 15.0f)
					{
						RotateRock();//敵を向く

						D3DXVECTOR3 dirtotarget = objPosition - target->GetPosition();
						enemyLength = D3DXVec3Length(&dirtotarget);
						revenge = true;
						morter = false;
						animeAttackRate = 0;
						attackAction = true;	//技の動作中か（攻撃ボタンを押したタイミングでtrueにし、falseは技関数の最後の当たり判定が出たタイミングで行う）
						mode = REVENGE;
						jCommandAction = false; intervalCnt = 0; interval = true;
					}
				}
			}
		}
		else { jCommandAction = false; intervalCnt = 0; interval = true; mode = DEFAULT; }
	}
	else
	{
		mode = DEFAULT;
	}
}

void Player::PInvicible()
{
	if (interval == false) {
		objRotation.z = 0;
		if (onGround == true) {
			playerModel->Update("Stop", "Dodge", animeDodgeRate, animeDodgeFrame);
		}
		else
		{
			playerModel->Update("Stop", "SkyDodge", animeDodgeRate, animeDodgeFrame);
		}
		if (invicibleCnt == 0)antiInduction = true;
		invicibleCnt++;
		if (invicibleCnt == 1) { animeDodgeFrame = 10; animeDodgeRate = 1.0f; axel = 0; }
		if(invicibleCnt==10)antiInduction = false;
		animeDodgeFrame++;
		if (invicibleCnt < 40) {
			if (onGround == true)
			{
				objPosition -= this->GetForward() * speed * 1.5f;
			}
			else
			{
				objPosition -= this->GetForward() * speed * 3.0f;
			}
		}
		else { jCommandAction = false; intervalCnt = 0; interval = true; mode = DEFAULT; }
	}
	else
	{
		mode = DEFAULT;
	}
}

void Player::PRevenge()
{
	if (jCommandAction == false)
	{
		Jcommand();	//どこからでも回避かガードでキャンセルできるようにする
	}
	if (attackAction == true)
	{
		if (revenge == true)
		{
			playerCamera->SetTargetCam(true);
			D3DXVECTOR3 dirtotarget = objPosition - target->GetPosition();
			enemyLength = D3DXVec3Length(&dirtotarget);
			Counter();
		}
	}
	else
	{
		if (hit == true && revenge == true)
		{
			if (Input::GetKeyTrigger('L'))
			{
				superArmor = true;
				D3DXVECTOR3 dirtotarget = objPosition - target->GetPosition();
				enemyLength = D3DXVec3Length(&dirtotarget);
				comboCnt = 0;		//カウントリセット
				RotateRock();
				pass = false;	//敵との判定干渉をONに
				attackAction = true;	//アクションしている状態にする
				morter = true;	//次のアクションをONにする
				hit = false;	//HITを初期化
				revenge = false;	//今まで出していたアクションをOFFに
			}
		}
	}

	if (attackAction == true)
	{
		if (morter == true)
		{
			Morter();
		}
	}
	else
	{
		if (hit == true && morter == true)
		{
			if (Input::GetKeyTrigger('L'))
			{
				Duplicate_Reset();	//技を2度同じものが出ないように管理するboolを全てtrueにセットする関数（boolの名前は後ほど変更）
				int randam = rand() % 3;
				if (randam == 0)
				{
					if (rush == true)abi4 = true;
					else if (slamDunk == true)abi5 = true;
					else if (slashUpper == true)abi6 = true;
				}
				else if (randam == 1)
				{
					if (slamDunk == true)abi5 = true;
					else if (slashUpper == true)abi6 = true;
					else if (rush == true)abi4 = true;
				}
				else
				{
					if (slashUpper == true)abi6 = true;
					else if (rush == true)abi4 = true;
					else if (slamDunk == true)abi5 = true;
				}

				superArmor = true;
				D3DXVECTOR3 T_P = objPosition - target->GetPosition();
				enemyLength = D3DXVec3Length(&T_P);
				comboCnt = 0;		//カウントリセット
				RotateRock();
				pass = false;	//敵との判定干渉をONに
				attackAction = true;	//アクションしている状態にする
				combo1 = false;
				combo2 = false;	//次のアクションをONにする
				combo3 = true;
				comboLast = false;
				hit = false;	//HITを初期化
				morter = false;	//今まで出していたアクションをOFFに
				mode = ATTACK;
			}
		}
	}
	{//攻撃ボタンが押されなかった時の処理
		if (attackAction != true)
		{
			superArmor = false;
			if (animeAttackRate > 0)animeAttackRate--;
			atkDelayTime++;
			if (atkDelayTime > 50.0f)
			{
				//boolの初期化をしないといけない
				attackAction = false;

				revenge = false;
				morter = false;
				hit = false;
				comboCnt = 0;
				pass = false;
				playerCamera->SetEvent(false);
				mode = DEFAULT;
			}
		}
		else
		{
			atkDelayTime = 0;
		}
	}
}

void Player::PFalter()
{
	playerSe[9]->Stop();
	if (animeFalterFrame == 0) { animeFalterRate = 1.0f; knockBack = false; }
	if (animeFalterFrame < 30)
	{
		animeFalterFrame++;
	}
	animeFalterRate = 1.0f;
	playerModel->Update("Stop", "Falter", animeFalterRate, animeFalterFrame);

	if (falterAction == true)
	{
		knockPow -= 0.1f;
		if (knockPow > 0.0f)
		{
			objPosition -= knockVector * knockPow;
		}
		else
		{
			falterCnt++;
			if (animeFalterRate > 0)animeFalterRate -= 0.1f;
		}
	}

	if (knockBack == true || blowBack == true)mode = DEFAULT;

	if (falterCnt > mFALTERTIME) { falterAction = false; mode = DEFAULT; }
}

void Player::PBlow()
{
	playerSe[9]->Stop();
	if (stopTime < 0)
	{
		if (animeFalterFrame == 0)
		{
			animeFalterFrame = 40; animeFalterRate = 1.0f; blowBack = false; RotateRock();
		}
		if (animeFalterFrame < 70)
		{
			animeFalterFrame++;
		}

		playerModel->Update("Stop", "Blow", animeFalterRate, animeFalterFrame);

		if (onGround == false)objPosition.y -= 0.25f;

		if (falterAction == true && blowBack == false)
		{
			knockPow -= 0.1f;
			if (knockPow > 0.0f)
			{
				objPosition -= knockVector * knockPow;
			}
			else
			{
				falterCnt++;
				if (animeFalterRate > 0)animeFalterRate -= 0.0075f;
			}
		}
		else
		{
			mode = DEFAULT;
		}
	}
	else
	{
		stopTime--;
	}

	if (knockBack == true || blowBack == true)mode = DEFAULT;

	if (falterCnt > mBLOWTIME) { animeFalterRate = 0.0f; falterAction = false; mode = DEFAULT; }
}

void Player::HitWall()
{
	{	//ステージ外処理
		D3DXVECTOR3 cen = objPosition - D3DXVECTOR3(0, 0, 0);
		centerLen = D3DXVec3Length(&cen);
		D3DXVec3Normalize(&cen, &cen);
		if (centerLen > 130)
		{
			objPosition = D3DXVECTOR3(0, 15, 0);
		}
		if (centerLen > 100)	//壁衝突
		{

			D3DXVECTOR3 SideVec;
			D3DXVec3Cross(&SideVec, &moveVec, &-moveVec);
			WallShaving(&wallDir, moveVec, SideVec);
			D3DXVec3Normalize(&wallDir, &wallDir);
			wallDir -= cen;
			D3DXVECTOR3 cendir = objPosition - D3DXVECTOR3(0, objPosition.y, 0);
			D3DXVec3Normalize(&cendir, &cendir);
			if (mode == INVICIBLE && onGround)
			{
				objPosition += wallDir * speed * 1.5;
			}
			else if (mode == INVICIBLE && !onGround)
			{
				objPosition += wallDir * speed * 3.0;
			}
			else if (mode == FALTER || mode == BLOW || mode == GUARD)
			{
				objPosition += wallDir * knockPow;
			}
			else if (diagonal == true)
			{
				objPosition += wallDir * speed * 2;
			}
			else if (slashRun == true)
			{
				objPosition += wallDir * slashRunSpeed;
			}
			else
			{
				objPosition += wallDir * speed;
			}
		}
	}
}

void Player::GetDamage()
{
	if (mode != GUARD && mode != INVICIBLE) {	//ガード、回避状態以外はダメージ計算
		if (lastDamage > 0)	//LastDamageは敵の攻撃力と技の攻撃力の計算が終わった後の数値
		{
			hp -= lastDamage * (1 - (deffencePower / 100));
			lastDamage = 0;
		}

		//怯み
		if (knockBack == true)
		{
			falterAction = true;
			falterCnt = 0;
			animeFalterFrame = 0;
			mode = FALTER;
		}
		//吹っ飛び
		if (blowBack == true)
		{
			falterAction = true;
			falterCnt = 0;
			animeFalterFrame = 0;
			mode = BLOW;
		}
	}
}

void Player::ColEnemy()
{
	//Passがfalseのときプレイヤーと敵が重ならないようにする
	if (pass == false) {
		for (Enemy* enemy : enemyList)
		{
			if (enemy->Exist == true) {
				if (GameObject::ColisionJudge(*this, *enemy, 1.25f) == true)
				{
					//m_Position += MoveVec;
					return;
				}
			}
		}
	}
}

