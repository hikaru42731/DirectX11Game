#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "audio.h"
#include "scene.h"
#include "Game.h"
#include "Game2.h"
#include "Game3.h"
#include "AbilitySelect.h"
#include "GameObject.h"
#include "Fade.h"
#include "ResultWin.h"
#include "ResultLose.h"
#include "Title.h"

// ����Ŏ��̂��쐬����
class Scene* Manager::managerScene = NULL;
bool* Manager::ability1 = NULL;
bool* Manager::ability2 = NULL;
bool* Manager::ability3 = NULL;
bool* Manager::ability4 = NULL;
bool* Manager::ability5 = NULL;
bool* Manager::ability6 = NULL;
bool* Manager::ability7 = NULL;
bool* Manager::ability8 = NULL;
bool* Manager::ability9 = NULL;
bool* Manager::ability10 = NULL;
bool* Manager::ability11 = NULL;
int Manager::gameId = 0;
int Manager::unLockStageNum = 0;
// Maneger:: ��Maneger�N���X�̊֐����w�肵�Ă���
void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	Audio::InitMaster();
	Fade::Init();
	SetScene<Title>();
}

void Manager::Uninit()
{
	managerScene->Uninit();
	delete managerScene;
	Audio::UninitMaster();
	Input::Uninit();
	Fade::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();

	managerScene->Update();
}

// �J������3D�|���S����2D�|���S���@�̏��Ԃŗp��
void Manager::Draw()
{
		Renderer::Begin();

		managerScene->Draw();
		Fade::Draw();

		Renderer::End();
}
