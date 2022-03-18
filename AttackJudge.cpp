#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "Model.h"
#include "Player.h"
#include "Scene.h"
#include "Enemy.h"
#include "Explosion.h"
#include "GameObject.h"
#include "Audio.h"
#include "AttackJudge.h"

#define EXISTFLAME 20
Model* a_Model;
Model* e_atkModel;
class Audio* EnemyAttackJudge::hitSe;
void AttackJudge::Init()
{
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(1.0f, 2.0f, 1.0f);

	cnt = 0;
	//Damage = 20;
	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "vertexLightingPS.cso");

}
void AttackJudge::Load()
{
	a_Model = new Model;
	a_Model->Load("asset\\model\\cube\\cube.obj");
}

void AttackJudge::UnLoad()
{
	a_Model->Unload();
	delete a_Model;
	
}

void AttackJudge::Uninit()
{
	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void AttackJudge::Update()
{
	if (cnt == 0)damage = damage * (1+attackPower / 100);
	
	cnt++;
	// 当たり判定
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>();
	int enemycnt = enemyList.size();
	for (Enemy* enemy:enemyList)
	{
		D3DXVECTOR3 enemyPosition = enemy->GetPosition();

		D3DXVECTOR3 direction = objPosition - enemyPosition;
		float length = D3DXVec3Length(&direction);
		if(length<5.0f){
			if (enemy->GetWarp() == false) {
				if (ColisionJudge(*this, *enemy, big))
				{
					if (enemy->GetMode() != GUARD && enemy->GetMode() != INVICIBLE)
					{
						if (knockBack == true) { enemy->SetKnock(knockPow, knockVector); }
						if (blowBack == true) { enemy->SetBlow(knockPow, knockVector); }
						enemy->SetStop(stopTime);
						enemy->SetRevenge(addRevenge);
						enemy->SetDamage(damage);
					}
					enemyPosition = enemy->GetPosition() + player->GetForward();
					enemyPosition.y = enemy->GetPosition().y + 3;
					scene->AddGameObject<Particle>()->SetPosition(enemyPosition);
					SetDestroy();
				}
			}
		}
	}
	
	if (cnt > existTime) { SetDestroy(); }
}

void AttackJudge::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(objVertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(objVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(objPixelShader, NULL, 0);
	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
		
	D3DXMatrixScaling(&scale, objScale.x, objScale.y, objScale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, objRotation.x, objRotation.y, objRotation.y);
	D3DXMatrixTranslation(&trans, objPosition.x, objPosition.y + 0.5, objPosition.z );
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	// モデル描画
	//a_Model->Draw();
}


void EnemyAttackJudge::Init()
{
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(1.0f, 2.0f, 1.0f);
	
	cnt = 0;

	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "vertexLightingPS.cso");
}

void EnemyAttackJudge::Update()
{
	if (cnt>EXISTFLAME&&cnt > existTime) { SetDestroy(); }
	if(cnt==0)damage = damage * (1 - (attackPower / 100)); 
	if (objPosition.y < 0)objPosition.y = 0;
	
	// 当たり判定
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	if (ColisionJudge(*this, *player, big))
	{
		if (player->GetMode() != INVICIBLE)
		{
			if (player->GetArmor()==false||armorBreak==true) 
			{
				if (cnt == 0 && player->GetMode() == GUARD) { knockPow /= 1.5; }
				if (knockBack == true) { player->SetKnock(knockPow, knockVector); }
				if (blowBack == true) { player->SetBlow(knockPow, knockVector); }
			}
			player->SetStop(stopTime);
			if (player->GetMode() != GUARD) { hitSe->Play(false); player->SetDamage(damage); }
		}
		scene->AddGameObject<Particle>()->SetPosition(player->GetPosition());
		SetDestroy();
		return;
	}
	cnt++;
}

void EnemyAttackJudge::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(objVertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(objVertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(objPixelShader, NULL, 0);
	//マトリクス設定
	D3DXMATRIX world, scale, rot, trans;

	D3DXMatrixScaling(&scale, objScale.x, objScale.y, objScale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, objRotation.x, objRotation.y, objRotation.y);
	D3DXMatrixTranslation(&trans, objPosition.x, objPosition.y + 0.5, objPosition.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	// モデル描画
	//a_Model->Draw();
}

void EnemyAttackJudge::Uninit()
{
	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void EnemyAttackJudge::Load()
{
	e_atkModel = new Model;
	e_atkModel->Load("asset\\model\\cube\\cube.obj");

	hitSe = new Audio;
	hitSe->Load("asset\\Audio\\HitSword.wav");
}

void EnemyAttackJudge::UnLoad()
{
	a_Model->Unload();
	delete a_Model;
}
