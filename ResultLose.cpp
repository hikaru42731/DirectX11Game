#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Scene.h"
#include "Input.h"
#include "Audio.h"
#include "VSField.h"
#include "Sky.h"
#include "ResultModel.h"
#include "ResultCursor.h"
#include "ResultTexture.h"
#include "Camera.h"
#include "ResultTexture.h"
#include "ResultCursor.h"
#include "Game.h"
#include "Game2.h"
#include "Game3.h"
#include "Title.h"
#include "Fade.h"
#include "AbilitySelect.h"
#include "ResultLose.h"

bool Lose::Init()
{
	gameId=Manager::GetGameId();
	AddGameObject<R_Camera>();
	Fade::Load();

	resultLoseModel = AddGameObject<R_Model>();
	resultLoseModel->SetPosition(D3DXVECTOR3(0, 0, 0));
	resultLoseModel->SetWin(false);
	AddGameObject<VSField>();
	AddGameObject<Sky>();

	Audio* bgm = AddGameObject<Audio>();
	bgm->Load("asset\\Audio\\Lose.wav");
	bgm->SetVolume(0.75f);
	bgm->Play(true);

	R_Texture::Load(false);	//勝ちはtrue
	resultLoseTexture=Add2DGameObject<R_Texture>();
	cur=Add2DGameObject<ResultCursor>();
	end = false;
	cnt = 0;
	return true;
}

void Lose::Uninit()
{
	Scene::Uninit();
}

void Lose::Update()
{
	cnt++;
	if (end == false) {
		Scene::Update();
		if (Fade::GetFadeEnd() == false)Fade::StartFade(1);
	}

	if (cnt > 50 && Input::GetKeyTrigger('L'))
	{
		Fade::SetFadeEnd();
		Fade::StartFade(0);
		end = true;
	}
	if (end == true && Fade::GetFadeEnd() == true)
	{
		if (cur != NULL) {
			if (cur->GetPosition().y == 580)
			{
				Manager::SetScene<AbilitySelect>();	//アビリティ選択へ
				return;
			}
			if (cur->GetPosition().y == 645)
			{
				Manager::SetScene<Title>();	//IDに対応したゲームへ
				return;
			}
		}
	}
}
