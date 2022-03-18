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

// これで実体を作成する
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
// Maneger:: でManegerクラスの関数を指定している
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

// カメラ→3Dポリゴン→2Dポリゴン　の順番で用意
void Manager::Draw()
{
		Renderer::Begin();

		managerScene->Draw();
		Fade::Draw();

		Renderer::End();
}
