#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Audio.h"
#include "Camera.h"
#include "Field.h"
#include "Explosion.h"
#include "Model.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "AttackJudge.h"
#include "VSField.h"
#include "EnemyHP.h"
#include "EnemyBar.h"
#include "PlayerHP.h"
#include "PlayerBar.h"
#include "Input.h"
#include "Sky.h"
#include "GameObject.h"
#include "AnimationModel.h"
#include "Explosion.h"
#include "Title.h"
#include "TargetMark.h"
#include "Ability.h"
#include "AbilitySelect.h"
#include "Fade.h"
#include "Locus.h"
#include "EnemyWarp.h"
#include "Guard.h"
#include "AttackOmen.h"
#include "AtkWall.h"
#include "ResultWin.h"
#include "ResultLose.h"
#include "Game.h"


bool Game::enemyEmpty=false;
std::vector<Enemy*> EneList;

bool Game::Init()
{
	cnt = 0;
	enemyEmpty = false;
	Manager::SetGameId(0);		//���݂̃X�e�[�W����n��
	if (Manager::GetUnLockStage() <= 0)
	{
		Manager::SetUnLockStage(0);	//�X�e�[�W���A�����b�N����
	}

	AddGameObject<Camera>();
	Fade::Load();
	
	AttackJudge::Load();
	EnemyAttackJudge::Load();
	Explosion::Load();
	Bomb::Load();
	LASTCHARGE::Load();
	Bullet::Load();
	EBullet::Load();
	AtkWall::Load();
	Player::AudioLoad();
	Locus::Load();
	EnemyWarp::Load();
	Omen::Load();
	Guard::Load();
	
	AddGameObject<VSField>();
	
	AddGameObject<Sky>();
	

	// �|�C���^�ϐ��Ŋ֐����Ăяo��
	enemy1 = AddEnemyGameObject<Enemy>(0,0);	//�G�l�~�[�^�C�v�w��iint�j,�G�l�~�[ID�w��(int)
	
	enemy1->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 10.0f));	//�������W
	
	enemy1->SetStates(10, 30,0.1);	//�X�e�[�^�X�Z�b�g�i�U����,�h���,�X�s�[�h)

	player = AddGameObject<Player>();
	AddGameObject<Locus>();
	
	Audio* bgm = AddGameObject<Audio>();
	bgm->Load("asset\\Audio\\Game1.wav");
	bgm->SetVolume(0.5f);
	bgm->Play(true);
	AddGameObject<TargetMark>();

	Add2DGameObject<Enemy_Bar>();
	Add2DGameObject<EnemyHP>();
	
	GetUnbreakGameObject<Ability>();

	return true;
}

void Game::Uninit()
{
	Explosion::UnLoad();
	Bomb::UnLoad();
	LASTCHARGE::UnLoad();
	Bullet::UnLoad();
	EBullet::UnLoad();
	AtkWall::UnLoad();
	Locus::UnLoad();
	EnemyWarp::UnLoad();
	Omen::UnLoad();
	Guard::UnLoad();
	EneList.clear();

	Scene::Uninit();//�p�����N���X�̃��\�b�h�Ăяo��
}

void Game::Update()
{
	cnt++;
	if(cnt<40)	Fade::StartFade(1);

	if (cnt > 20) {
		Scene::Update();
		if(cnt>400)Result();
	}
}


void Game::Result()
{
	if (player->GetHP() <= 0)
	{
		Fade::StartFade(0);
	}
	if (enemyEmpty == false && cnt > 500 && Fade::GetFadeEnd() == true)
	{
		Manager::SetGameId(0);		//���݂̃X�e�[�W����n��
		Fade::SetFadeEnd();
		Manager::SetScene<Lose>();	//�s�k���U���g��
		return;
	}

		EneList = GetGameObjects<Enemy>();
		for (Enemy* enemy : EneList)
		{
			if (enemy->Exist == true) { enemyEmpty = false; break; }
			else enemyEmpty = true;
		}
		if (enemyEmpty == true)Fade::StartFade(0);
		if (enemyEmpty==true&&cnt > 500 && Fade::GetFadeEnd() == true)
		{
			Manager::SetGameId(0);		//���݂̃X�e�[�W����n��
			Fade::SetFadeEnd();
			Manager::SetScene<Win>();	//�������U���g��
		}
}