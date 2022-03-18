#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Input.h"
#include "TitleRogo.h"
#include "Title.h"
#include "AbilitySelect.h"
#include "SelectBack.h"
#include "AbilityON.h"
#include "SelectCursor.h"
#include "StageNumber.h"
#include "Ability.h"
#include "Fade.h"
#include "Game.h"
#include "Game2.h"
#include "Game3.h"

int AbilitySelect::gameId=0;

bool AbilitySelect::Init()
{
	cnt = 0;
	gameId = Manager::GetGameId();
	Fade::SetFadeEnd();
	Fade::Load();
	AddUnbreakGameObject<Ability>();
	AddGameObject<SelectBack>();
	AbilityON* an[11];
	int PosX = 475;
	int PosY = 190;
	int SizeY = 88;
	for (int i = 1; i < 3;) {
		for (int j = 0; j < 6; j++)
		{
			an[abilityId] = AddGameObject<AbilityON>();
			an[abilityId]->SetPosition(D3DXVECTOR3(PosX+650*(i-1), PosY+SizeY*j, 0));
			abilityId++;
			if (abilityId > 10)break;
		}
		i++;
		if (abilityId > 10)break;
	}
	stageNumber=AddGameObject<StageNumber>();
	stageNumber->SetPosition(D3DXVECTOR3(1125, 630, 0));
	Add2DGameObject<SelectCursor>();
	
	end = false;
	return true;
}

void AbilitySelect::Uninit()
{
	Scene::Uninit();
}

void AbilitySelect::Update()
{
	Scene::Update();
	if (cnt > 50)
	{
		if (Input::GetKeyTrigger(VK_SPACE))
		{
			Fade::SetFadeEnd();
			Fade::StartFade(0);
			end = true;
		}
		if (end==true&&Fade::GetFadeEnd() == true)
		{
			switch (gameId)
			{
			case 0:
				Manager::SetScene<Game>();
				break;
			case 1:
				Manager::SetScene<Game2>();
				break;
			case 2:
				Manager::SetScene<Game3>();
				break;
			}
		}
	}
	else
	{
		Fade::StartFade(1);
	}
	cnt++;
}