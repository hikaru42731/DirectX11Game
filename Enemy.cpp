#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "Scene.h"
#include "Input.h"
#include "Model.h"
#include "Camera.h"
#include "AnimationModel.h"
#include "Player.h"
#include "Bullet.h"
#include <time.h>
#include "Audio.h"
#include "Game.h"
#include "EnemyWarp.h"
#include "AttackOmen.h"
#include "Enemy.h"

#define FALTERTIME (90)
#define ATTACKGRACE (100)
#define CHASESPEED (0.25f)
#define REVENGERESET (200)

void Enemy::ModelLoad()
{
	enemyModel = new AnimationModel();
	switch (enemyType)
	{
	case Enemy1:
		enemyModel->Load("asset\\model\\Enemy\\Type1\\Idle.fbx");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type1\\Idle.fbx", "Stop");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type1\\Walk.fbx", "Walk");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type1\\Fall.fbx", "Fall");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type1\\Falter.fbx", "Falter");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type1\\Blow.fbx", "Blow");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type1\\Dying.fbx", "Die");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type1\\SideWalk.fbx", "SW");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type1\\Run.fbx", "Run");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type1\\ShortRange1.fbx", "Short1");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type1\\ShortRange2.fbx", "Short2");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type1\\MiddleRange.fbx", "Middle1");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type1\\LongRange1.fbx", "Long1");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type1\\LongRange2.fbx", "Long2");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type1\\Counter1.fbx", "Counter1");
		attackRevenge = 10;	//�J�E���^�[���郊�x���W�l
		break;
	case Enemy2:
		enemyModel->Load("asset\\model\\Enemy\\Type2\\Idle.fbx");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type2\\Idle.fbx", "Stop");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type2\\Fall.fbx", "Fall");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type2\\Falter.fbx", "Falter");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type2\\Blow.fbx", "Blow");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type2\\Dying.fbx", "Die");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type2\\Long1.fbx", "Long1");
		enemyModel->LoadAnimation("asset\\model\\Enemy\\Type2\\Long2.fbx", "Long2");
		attackRevenge = 15;	//�J�E���^�[���郊�x���W�l
		break;
	default:
		break;
	}
}

void Enemy::ModelUnLoad()
{
	switch (enemyType)
	{
	case Enemy1:
		enemyModel->UnLoadAnimation("Stop");
		enemyModel->UnLoadAnimation("Walk");
		enemyModel->UnLoadAnimation("Fall");
		enemyModel->UnLoadAnimation("Falter");
		enemyModel->UnLoadAnimation("Blow");
		enemyModel->UnLoadAnimation("Die");
		enemyModel->UnLoadAnimation("SW");
		enemyModel->UnLoadAnimation("Run");
		enemyModel->UnLoadAnimation("Short1");
		enemyModel->UnLoadAnimation("Short2");
		enemyModel->UnLoadAnimation("Middle1");
		enemyModel->UnLoadAnimation("Long1");
		enemyModel->UnLoadAnimation("Long2");
		enemyModel->UnLoadAnimation("Counter1");
		break;
	case Enemy2:
		enemyModel->UnLoadAnimation("Stop");
		enemyModel->UnLoadAnimation("Fall");
		enemyModel->UnLoadAnimation("Falter");
		enemyModel->UnLoadAnimation("Blow");
		enemyModel->UnLoadAnimation("Die");
		enemyModel->UnLoadAnimation("Long1");
		enemyModel->UnLoadAnimation("Long2");
		break;
	}
	enemyModel->UnLoad();
	delete enemyModel;
}

void Enemy::Init()
{
	ModelLoad();
	debugMode = false;
	blendRate = 0.0f;
	animFrame = 0;
	falterRate = 0;
	falterFrame = 0;
	falterCnt = 0;

	objPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(0.02f, 0.02f, 0.02f);
	unWallPos = objPosition;

	Exist = true;
	enemyAtkAction = false;
	attackPower = 15;
	deffencePower = 15;
	hp = 1000;
	
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "vertexLightingPS.cso");

	enemyScene = Manager::GetScene();
	enemyWarp = enemyScene->AddGameObject<EnemyWarp>();

	
	cnt = 0;
	aiAction = false;
	aiCnt = 0;
	enemyDefaultCnt = 0;
	aiDestination = objPosition;
	enemyPlayer=new Player;
	warp = false;
	mode = ENCOUNTER;

	attackSe = Manager::GetScene()->AddGameObject<Audio>();
	attackSe->Load("asset\\Audio\\Attack.wav");
	attackSe->SetVolume(1.5f);

	bigAttackSe = Manager::GetScene()->AddGameObject<Audio>();
	bigAttackSe->Load("asset\\Audio\\Fire.wav");
	bigAttackSe->SetVolume(1.5f);

	attackChargeSe = Manager::GetScene()->AddGameObject<Audio>();
	attackChargeSe->Load("asset\\Audio\\LastCharge.wav");
	attackChargeSe->SetVolume(1.5f);

	atkWallHitSe = Manager::GetScene()->AddGameObject<Audio>();
	atkWallHitSe->Load("asset\\Audio\\AtkWallHit.wav");
	atkWallHitSe->SetVolume(1.5f);

	atkWallOmenSe = Manager::GetScene()->AddGameObject<Audio>();
	atkWallOmenSe->Load("asset\\Audio\\AtkWallOmen.wav");
	atkWallOmenSe->SetVolume(1.5f);


}
void Enemy::Uninit()
{
	ModelUnLoad();
	
	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void Enemy::Update()
{
	{
		enemyPlayer = enemyScene->GetGameObject<Player>();
		enemyCamera = enemyScene->GetGameObject<Camera>();
		enemyWarp->SetPosition(D3DXVECTOR3(objPosition.x,objPosition.y+1.5,objPosition.z));
		enemyWarp->SetWarp(warp);
		D3DXVECTOR3 dirtoplayer = objPosition - enemyPlayer->GetPosition();
		enemyAudioVolume = D3DXVec3Length(&dirtoplayer);
		attackSe->SetVolume(2-fabsf(enemyAudioVolume)/50);
		bigAttackSe->SetVolume(2-fabsf(enemyAudioVolume)/50);
		attackChargeSe->SetVolume(2 - fabsf(enemyAudioVolume) / 50);
		{	//���[�h���ʍX�V
			if (objPosition.y <= 0.0f)
			{
				objPosition.y = 0;
				onGround = true;
			}
			else
			{
				onGround = false;
			}	//�ڒn���Ă��邩
		}
		if (lastDamage > 0)	//�_���[�W�v�Z
		{
			hp -= lastDamage * (1 - (deffencePower / 100));
			lastDamage = 0;
		}

		if (hp <= 0 && Exist == true)	//HP��0�ɂȂ����炵�΂炭���͎̂c�邪���Ȃ��Ȃ������̂Ƃ���
		{
			dieRate = 1.0f;
			dieFrame = 0;
			if (enemyOmen != NULL)enemyOmen->SetDestroy();
			Exist = false;
		}

		if (Exist == false)	//HP��0�ɂȂ��Ă��������܂�
		{
			enemyModel->Update("Die", "Die", dieRate, dieFrame);
			if (mode == FALTER)mode = DEFAULT;
			switch (enemyType)
			{
			case Enemy1:
				if (dieFrame < 150) {
					if (onGround == false)
					{
						objPosition.y -= 0.5f;
					}
					dieFrame++;
				}
				else
				{
					DiedFrame++;
					if (DiedFrame == 180) { SetDestroy(); }
				}
				break;
			case Enemy2:
				if (dieFrame < 50) {
					if (onGround == false)
					{
						objPosition.y -= 0.5f;
					}
					dieFrame++;
				}
				else
				{
					DiedFrame++;
					if (DiedFrame == 180) { SetDestroy(); }
				}
				break;
			}
		}

		if (prevPos != objPosition)	//�ړ������擾
		{
			moveVec = prevPos - objPosition;
			D3DXVec3Normalize(&moveVec, &moveVec);
		}

		RevengeAdd();	//���x���W�l�v�Z

		HitWall();		//�X�e�[�W�O�ɏo���Ƃ��̏���

		GetDamage();	//��e����

	}	//�����܂őS�G���ʏ���
	if (Exist == true)
	{
		switch (enemyType)
		{
		case Enemy1:	//�^�C�v�ɂ���ČĂяo��Update��ς���
			E1Update();
			break;
		case Enemy2:
			E2Update();
			break;
		case Enemy3:
			break;
		}
	}
}

void Enemy::Draw()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();

	if (!camera->CheckView(objPosition))	return;
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(objVertexLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(objVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(objPixelShader, NULL, 0);
	
	//�}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, objScale.x, objScale.y, objScale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, objRotation.y, objRotation.x, objRotation.z);
	D3DXMatrixTranslation(&trans, objPosition.x, objPosition.y, objPosition.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	if (warp == false) {	//���[�v���ĂȂ��Ƃ�
		// ���f���`��
		enemyModel->Draw();
	}
}

void Enemy::E1Update()
{
	switch (mode)
	{
	case DEFAULT:
		E1Default();
		break;
	case ATTACK:
		E1Attack();
		break;
	case CHASE:
		E1Chase();
		break;
	case REVENGE:
		E1Revenge();
		break;
	case ENCOUNTER:
		enemyModel->Update("Stop", "Stop", blendRate, animFrame);
		animFrame++;
		cnt++;
		if (cnt == 500)mode = DEFAULT;
		break;
	case FALTER:
		EnemyFalter();
		break;
	case BLOW:
		EnemyBlow();
		break;
	default:
		mode = DEFAULT;
		break;
	}
}

void Enemy::E2Update()
{
	{	//���[�h���ʍX�V
		if (objPosition.y <= 0.0f)
		{
			objPosition.y = 0;
			onGround = true;
		}
		else
		{
			onGround = false;
		}	//�ڒn���Ă��邩
	}

	switch (mode)
	{
	case DEFAULT:
		E2Default();
		break;
	case ATTACK:
		E2Attack();
		break;
	case REVENGE:
		E2Revenge();
		break;
	case ENCOUNTER:
		enemyModel->Update("Stop", "Stop", blendRate, animFrame);
		animFrame++;
		cnt++;
		if (cnt == 500)mode = DEFAULT;
		break;
	case FALTER:
		EnemyFalter();
		break;
	case BLOW:
		EnemyBlow();
		break;
	default:
		mode = DEFAULT;
		break;
	}
}

void Enemy::E3Update()
{
}

void Enemy::E1Default()
{
	if (Input::GetKeyTrigger('T'))
	{
		switch (debugMode)
		{
		case true:
			debugMode = false;
			deffencePower = defenceKeep;
			break;
		case false:
			debugMode = true;
			defenceKeep=deffencePower;
			deffencePower = 100;
			break;
		}
	}
	//�f�t�H���g�֐�
	EnemyRotate();
	if (Exist == true) {
		enemyModel->Update("Stop", "SW", blendRate, animFrame);
		if (aiAction == false)
		{
			blendRate -= 0.1f;
			if (blendRate < 0)blendRate = 0;
			objPosition += D3DXVECTOR3(0, 0, 0);										//�������~�߂�
			aiDestination = D3DXVECTOR3(rand() % 60 - 30, 0, rand() % 60 - 30);	//���[�v��ݒ�

			aiStartPos = GetPosition();		//���[�v����O�̍��W���擾
			aiMovedir = objPosition - aiDestination;	//���[�v�������ׂ��������擾
			aiMoveLen = D3DXVec3Length(&aiMovedir);		//�����ׂ��������擾
			D3DXVec3Normalize(&aiMovedir, &aiMovedir);	//���K��
			if(debugMode==false)aiCnt++;

			if (aiCnt == 50) { srand(rand() % 10000); aiAction = true; }
		}
		else
		{
			D3DXVECTOR3 dir = objPosition - aiStartPos;	//������ʒu�ƃ��[�v����O�̍��W���r
			float len = D3DXVec3Length(&dir);			//���̒������擾
			if (aiMoveLen > len)						//�����ׂ������Ǝ��ۓ������������r
			{											//�����ׂ������̂ق����܂������Ȃ烏�[�v�p��
				aiSpeed += 0.05f; //���񂾂��������
				warp = true;
				objPosition -= aiMovedir * aiSpeed;
				if (blendRate < 1)blendRate += 0.1f;
			}
			else    //���ۓ����������������ׂ������𒴂����烏�[�v����߂�
			{
				aiSpeed = 0;
				warp = false;
				aiCnt = 0;
				aiAction = false;
			}
		}

		if (enemyDefaultCnt > ATTACKGRACE)		//mode��DEFAULT�ɂȂ��Ă���100f��ɂ����ɂ͂���
		{
			int act = rand() % 2;
			if (act == 1)				//�����Ă���1/2�̊m���ōU�����J�n����
			{
				aiCnt = -100;
				warp = false;
				enemyAtkCnt = 0;
				enemyAtkAction = true;
				unInduct = false;
				mode = ATTACK;	//mode=DEFAULT���甲����
			}
			enemyDefaultCnt = 0;	//�U�����Ȃ�������J�E���g����蒼��
		}
		if (onGround == false) { "Fall", "Fall", blendRate, animFrame; objPosition.y -= 0.25f; }//�󒆂ɂ���Ȃ炾�񂾂񗎉�����
		else enemyModel->Update("Stop", "Stop", blendRate, animFrame);
	}

	enemyPlayerDir = objPosition - enemyPlayer->GetPosition();
	enemyPlayerLength = D3DXVec3Length(&enemyPlayerDir);	//�v���C���[�Ƃ̋����𑪂�
	
	if(debugMode==false)enemyDefaultCnt++;
	

	animFrame++;
}

void Enemy::E1Attack()
{
	//�U���֐�
	knockBack = false;
	blowBack = false;
	enemyDefaultCnt = 0;
	if (enemyPlayerLength <= 65)	//�v���C���[�Ƃ̋�����65�ȉ��Ȃ炻�̂܂܍U������
	{
		EnemyAttack();
	}
	else  //�����艓����Έ�x�ǐՂ��J�n����
	{
		walkRate = 0;
		mode = CHASE;
	}

	if (enemyAtkAction == false) { aiAction = false; aiCnt = -50; mode = DEFAULT; }	//�U����~�̖��߂�������DEFAULT�ɖ߂�
}

void Enemy::E1Chase()
{
	//�G��ǂ��֐�
	EnemyRotate();
	enemyModel->Update("Stop", "Run", walkRate, walkFrame);

	if (walkRate == 0)chasePow = rand() % 45 + 10;	//�ǂ̂��炢�ǐՂ��邩��ݒ肷��
	walkRate = 1.0;
	walkFrame++;
	enemyPlayerDir = objPosition - enemyPlayer->GetPosition();	//���A���^�C���Ńv���C���[�Ƃ̕��p�A�������擾
	enemyPlayerLength = D3DXVec3Length(&enemyPlayerDir);
	D3DXVec3Normalize(&enemyPlayerDir, &enemyPlayerDir);

	if (enemyPlayerLength < chasePow)	//�v���C���[�Ƃ̋������ݒ肵���l���k�܂�����U���Ɉڂ�
	{
		mode = ATTACK;
	}
	else
	{
		objPosition.x -= enemyPlayerDir.x * CHASESPEED;
		objPosition.z -= enemyPlayerDir.z * CHASESPEED;	//�󒆂ɂ܂ł͒ǂ�Ȃ�
	}
	if (onGround == false)objPosition.y -= 0.15f;
}

void Enemy::E1Revenge()
{	
	//���x���W�s��
	knockBack = false;
	blowBack = false;
	enemyModel->Update("Stop", "Counter1", attackRate, attackFrame);
	attackRate = 1.0f;
	Counter1();
}

void Enemy::E2Default()
{
	//�f�t�H���g�֐�
	EnemyRotate();
	if (Exist == true) {


		if (enemyDefaultCnt > ATTACKGRACE)		//mode��DEFAULT�ɂȂ��Ă���100f��ɂ����ɂ͂���
		{
			int act = rand() % 3;
			if (act == 1)				//�����Ă���1/2�̊m���ōU�����J�n����
			{
				aiCnt = -100;
				warp = false;
				enemyAtkCnt = 0;
				enemyAtkAction = true;
				unInduct = false;
				mode = ATTACK;	//mode=DEFAULT���甲����
			}
			enemyDefaultCnt = 0;	//�U�����Ȃ�������J�E���g����蒼��
		}

		if (onGround == false) { "Fall", "Fall", blendRate, animFrame; objPosition.y -= 0.25f; }//�󒆂ɂ���Ȃ炾�񂾂񗎉�����
		else enemyModel->Update("Stop", "Stop", blendRate, animFrame);

	}

	enemyPlayerDir = objPosition - enemyPlayer->GetPosition();
	enemyPlayerLength = D3DXVec3Length(&enemyPlayerDir);	//�v���C���[�Ƃ̋����𑪂�

	enemyDefaultCnt++;

	animFrame++;
}

void Enemy::E2Attack()
{
	//�U���֐�
	knockBack = false;
	blowBack = false;
	enemyDefaultCnt = 0;

	EnemyAttack();

	if (enemyAtkAction == false) { aiAction = false; aiCnt = -50; mode = DEFAULT; }	//�U����~�̖��߂�������DEFAULT�ɖ߂�
}

void Enemy::E2Revenge()
{
	//���x���W�s��
	knockBack = false;
	blowBack = false;
	enemyModel->Update("Stop", "Stop", attackRate, attackFrame);
	attackRate = 1.0f;
	Counter2();
}

void Enemy::EnemyFalter()
{
	//�Ђ��
	warp = false;
	if (falterFrame == 0)knockBack = false;
	if (stopTime < 0) {
		if (falterFrame < 40)
		{
			falterFrame++;
		}
		falterRate = 1.0f;

		enemyModel->Update("Stop", "Falter", falterRate, falterFrame);

		if (falterAction == true)
		{
			knockPow -= 0.1f;
			if (knockPow > 0.0f)
			{
				objPosition -= knockVector * knockPow;
			}
			else
			{
				enemyDefaultCnt = 0;
				falterCnt++;
				falterRate = 0;
			}
		}
	}
	else
	{
		stopTime--;
	}

	if (knockBack == true || blowBack == true)mode = DEFAULT;

	if (falterCnt > FALTERTIME) { falterAction = false; mode = DEFAULT; }
}

void Enemy::EnemyBlow()
{
	//�������
	warp = false;
	if (falterFrame == 0) { EnemyRotate(); falterRate = 1.0f; blowBack = false; }
	if (stopTime < 0)
	{
		if (falterFrame < 40)
		{

			falterFrame++;
		}

		enemyModel->Update("Stop", "Blow", falterRate, falterFrame);

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
				enemyDefaultCnt = 0;
				falterCnt++;
				if (falterRate > 0)falterRate -= 0.02f;
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

	if (knockBack == true || blowBack == true)mode = DEFAULT;	//�Ђ�ݒ��ɍU���H�������
	if (falterCnt > FALTERTIME) { falterAction = false; mode = DEFAULT; }	//�Ђ�݂��I�������f�t�H���g�ɖ߂�
}

void Enemy::RevengeAdd()
{
	{//���x���W�l�֌W
		if (addRevenge > 0)	//���x���W�l���Z
		{
			revengeCnt = REVENGERESET;
			revengeNum += addRevenge;
			revengeRand = rand();
			addRevenge = 0;
		}

		if (revengeCnt > 0)	//���x���W�l���Z�b�g�J�E���^�[
		{
			revengeCnt--;
		}
		else                //�J�E���g��0�ɂȂ����烊�x���W�l�����Z�b�g
		{
			revengeNum = 0;
		}

		if (revengeNum > attackRevenge)
		{
			switch (enemyType)
			{
			case Enemy1:
				if (revengeRand%3 != 0)
				{
					revengeNum = 0; enemyAtkCnt = 0; mode = REVENGE;
				}
				else
				{
					revengeNum = 0; aiCnt = 30; mode = DEFAULT;
				}
				break;
			case Enemy2:
				revengeNum = 0; enemyAtkCnt = 0; mode = REVENGE;
				break;
			}
		}
	}
}

void Enemy::GetDamage()
{	
	//��e�����Ƃ��̏���
	if (mode != REVENGE && mode != ATTACK)
	{
		//�Ђ��
		if (knockBack == true)
		{
			aiCnt = -100;
			falterAction = true;
			falterCnt = 0;
			falterFrame = 0;
			mode = FALTER;
		}
		//�ӂ��Ƃ�
		if (blowBack == true)
		{
			falterAction = true;
			falterCnt = 0;
			falterFrame = 0;
			mode = BLOW;
		}
	}
}

void Enemy::HitWall()
{
	//�X�e�[�W�O�ɏo���Ƃ��̏���
	D3DXVECTOR3 cen = objPosition - D3DXVECTOR3(0, 0, 0);
	centerLen = D3DXVec3Length(&cen);
	D3DXVec3Normalize(&cen, &cen);
	if (centerLen > 130)
	{
		objPosition = D3DXVECTOR3(0, 15, 0);
	}
	if (centerLen > 100)	//�^���I�ȕ�
	{
		aiAction = true;
		D3DXVECTOR3 sidevec;
		D3DXVec3Cross(&sidevec, &moveVec, &-moveVec);
		WallShaving(&wallDir, moveVec, sidevec);
		if (mode == FALTER || mode == BLOW)
		{
			objPosition += (wallDir - (cen * 1.5f)) * knockPow;
		}
		else if (mode == CHASE)
		{
			objPosition += (wallDir - (cen * 1.5f)) * CHASESPEED;
		}
		else
		{
			objPosition += (wallDir - (cen * 1.5f)) * speed;
		}
	}
	else
	{
		unWallPos = objPosition;
	}
}

void Enemy::EnemyRotate()
{	
	//�v���C���[�̕����������֐�
	D3DXVECTOR3 direction = objPosition - enemyPlayer->GetPosition();
	D3DXVec3Normalize(&direction, &direction);

	if (objPosition.z > enemyPlayer->GetPosition().z) {  objRotation.y = direction.x ; }
	if (objPosition.z < enemyPlayer->GetPosition().z) {  objRotation.y = -direction.x +1.57*2; }
}

void Enemy::EnemyAttack()
{
	//�ǂ̍U�������邩���肷��֐�
	attackRate = 1.0f;
	if (enemyAtkCnt == 0) { EnemyRotate(); startPos = objPosition; enemyPlayer->SetPass(true); attackRate = 0; attackFrame = 0; }	//�v���C���[�Ƃ̔��花�𖳂��� 

	switch (enemyType)
	{
	case Enemy1:
		if (enemyPlayerLength < 30)	//�U���J�n�������_�ł̓G�̋������r
		{
			enemyModel->Update("Short1", "Short1", attackRate, attackFrame);
			ShortRange1();
		}
		else if (enemyPlayerLength < 60)
		{
			enemyModel->Update("Middle1", "Middle1", attackRate, attackFrame);
			MiddleRange1();
		}
		else if (enemyPlayerLength >= 60)
		{
			enemyModel->Update("Stop", "Long2", attackRate, attackFrame);
			LongRange1();
		}
		break;
	case Enemy2:
		if (enemyAtkCnt == 0) { atkRand = rand() % 2; }
		if (atkRand == 0)
		{
			enemyModel->Update("Stop", "Long2", attackRate, attackFrame);
			LongRange2();
		}
		else
		{
			enemyModel->Update("Stop", "Long1", attackRate, attackFrame);
			LongRange1();
		}
		break;
	}

	if (enemyAtkAction == false)
	{
		enemyPlayer->SetPass(false);							//�v���C���[�Ƃ̔��花�𕜊�������
		mode = DEFAULT;
	}
}

