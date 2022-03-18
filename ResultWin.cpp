#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Input.h"
#include "Audio.h"
#include "Scene.h"
#include "VSField.h"
#include "Sky.h"
#include "ResultModel.h"
#include "ResultTexture.h"
#include "ResultCursor.h"
#include "Camera.h"
#include "Fade.h"
#include "Game.h"
#include "Game2.h"
#include "Game3.h"
#include "Title.h"
#include "AbilitySelect.h"
#include "ResultWin.h"

bool Win::Init()
{
	AddGameObject<R_Camera>();
	Fade::Load();

	resultWinModel= AddGameObject<R_Model>();
	resultWinModel->SetPosition(D3DXVECTOR3(0, 0, 0));
	resultWinModel->SetWin(true);
	AddGameObject<VSField>();
	AddGameObject<Sky>();

	Audio* bgm = AddGameObject<Audio>();
	bgm->Load("asset\\Audio\\Win.wav");
	bgm->SetVolume(0.75f);
	bgm->Play(true);

	R_Texture::Load(true);	//勝ちはtrue
	resultWinModelTexture = Add2DGameObject<R_Texture>();
	cur=Add2DGameObject<ResultCursor>();
	cnt = 0;
	
	return true;
}

void Win::Uninit()
{
	Scene::Uninit();
}

void Win::Update()
{
	cnt++;
	if (end == false) {
	Scene::Update();
	gameId = Manager::GetGameId();
	if (Fade::GetFadeEnd() == false)Fade::StartFade(1);
	}
	switch (gameId)
	{
	case 0:
		if (Manager::GetUnLockStage() <= 1)
		{
			Manager::SetUnLockStage(1);	//次のステージをアンロックする
		}
		break;
	case 1:
		if (Manager::GetUnLockStage() <= 2)
		{
			Manager::SetUnLockStage(2);	//次のステージをアンロックする
		}
		break;
	case 2:
		if (Manager::GetUnLockStage() <= 3)
		{
			Manager::SetUnLockStage(3);	//次のステージをアンロックする
		}
		break;
	}
	
	if (cnt>50&&Input::GetKeyTrigger('L'))
	{
		Fade::SetFadeEnd();
		Fade::StartFade(0);
		end = true;
	}
	if (end == true && Fade::GetFadeEnd() == true)
	{
		if (cur != NULL) {
			if (cur->GetPosition().y == 515)
			{
				switch (gameId)
				{
				case 0:
					Manager::SetScene<Game2>();	//アビリティそのままに次のゲームへ
					break;
				case 1:
					Manager::SetScene<Game3>();	//アビリティそのままに次のゲームへ
					break;
				case 2:
					Manager::SetScene<Title>();
				}
				return;
			}
			if (cur->GetPosition().y == 580)
			{
				Manager::SetScene<AbilitySelect>();	//アビリティ選択へ
				return;
			}
			if (cur->GetPosition().y == 645)
			{
				Manager::SetScene<Title>();	//タイトルへ
				return;
			}
		}
	}
}
