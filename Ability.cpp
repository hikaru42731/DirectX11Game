#include "Main.h"
#include "Manager.h"
#include "Ability.h"
#include "Game.h"
#include "AbilitySelect.h"
#include "AbilityON.h"
#include "Player.h"
#include "GameObject.h"
#include "Scene.h"

std::vector<AbilityON*> abilist;
void Ability::Init()
{
	abilityScene = Manager::GetScene();
	if (typeid(abilityScene) == typeid(AbilitySelect))
	{
		for (int i = 0; i < 11; i++)
		{
			ability[i] = true;
		}
		game = false;
	}
}

void Ability::Uninit()
{
}

void Ability::Update()
{
		abilist=abilityScene->GetGameObjects<AbilityON>();
		int i = 0;
		for (AbilityON* abil : abilist)
		{
			ability[i] = abil->GetON();
			i++;
		}
		Manager::SetAbility(&ability[0], &ability[1], &ability[2], &ability[3], &ability[4], &ability[5], &ability[6], &ability[7], &ability[8], &ability[9], &ability[10]);
}

void Ability::Draw()
{
}
