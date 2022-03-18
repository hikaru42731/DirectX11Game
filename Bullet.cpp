#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "Scene.h"
#include "Model.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Explosion.h"
#include "AnimationModel.h"
#include "Camera.h"
#include "Player.h"


Player* player;
#define EXIST_FLAME 1500 //存在時間
class Model* Bullet::bulletModel;
class Model* EBullet::enemyBulletModel;
void Bullet::Init()
{
	objPosition = D3DXVECTOR3(0.0f, 0.5f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(3.0, 3.0, 2.0);

	player = Manager::GetScene()->GetGameObject<Player>();
	playerForward = player->GetForward();
	D3DXVec3Normalize(&playerForward, &playerForward);
	playerPosition = player->GetPosition();
	cnt = 0;

	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "vertexLightingPS.cso");

}
void Bullet::Uninit()
{
	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void Bullet::Update()
{
	D3DXVECTOR3 targetpos = player->GetTarget()->GetPosition();
	targetpos.y = player->GetTarget()->GetPosition().y + 0.5f;
	D3DXVECTOR3 dir = objPosition - targetpos;
	D3DXVec3Normalize(&dir, &dir);
	Camera* cam = Manager::GetScene()->GetGameObject<Camera>();
	if (cnt < EXIST_FLAME) {
		if (cnt == 0)damage = damage * (1 + attackPower / 100);
		objPosition -= dir * speed;

		// 当たり判定
		Scene* scene = Manager::GetScene();
		std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>();

		for (Enemy* enemy : enemyList)
		{
			if (GameObject::ColisionJudge(*this, *enemy,1.5f) == true)
			{
				if (enemy->GetMode() != GUARD && enemy->GetMode() != INVICIBLE)
				{
					if (knockBack == true) { enemy->SetKnock(knockPow, knockVector); }
					if (blowBack == true) { enemy->SetBlow(knockPow, knockVector); }
					enemy->SetDamage(damage);
					enemy->SetRevenge(addRevenge);
				}
				D3DXVECTOR3 effectpos = enemy->GetPosition()+cam->GetForward();
				effectpos.y= enemy->GetPosition().y+3.0f;
				scene->AddGameObject<Bomb>()->SetPosition(effectpos);
				SetDestroy();
				return;
			}
		}

		cnt++;
	}
	else {
		SetDestroy();
	}
}

void Bullet::Draw()
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
	D3DXMatrixTranslation(&trans, objPosition.x, objPosition.y, objPosition.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	// モデル描画
	bulletModel->Draw();
}

void Bullet::Load()
{
	// モデル読み込み
	bulletModel = new Model();
	bulletModel->Load("asset\\model\\FireBall\\FireBall.obj");
}

void Bullet::UnLoad()
{
	bulletModel->Unload();
	delete bulletModel;
}

void EBullet::Init()
{
	objPosition = D3DXVECTOR3(0.0f, 0.5f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objScale = D3DXVECTOR3(3, 3, 2);

	player = Manager::GetScene()->GetGameObject<Player>();
	target=player->GetPosition();
	Rotate();
	cnt = 0;
	existCnt = 150;
	inductRange = 5.0f;
	
	// Shaderで意味わからんエラーが出るときはcsoの名前が原因かも
	Renderer::CreateVertexShader(&objVertexShader, &objVertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&objPixelShader, "vertexLightingPS.cso");
}

void EBullet::Uninit()
{

	objVertexLayout->Release();
	objVertexShader->Release();
	objPixelShader->Release();
}

void EBullet::Update()
{
	if (player->GetAntiInduction() == true)induct = true;
	if (cnt < EXIST_FLAME) {
		if (cnt == 0) { damage = damage * (1 - (attackPower / 100));}
		
		//追従方向設定
		D3DXVECTOR3 direction = objPosition - target;
		float length = D3DXVec3Length(&direction);
		D3DXVec3Normalize(&direction, &direction);
		if (length < inductRange) induct = true;
		
		if (induct != true) 
		{		//誘導が切られるまではプレイヤーの座標を更新し続ける
			target = player->GetPosition();
			Rotate();
			if (target.y < 1.0f) target.y = 1.0f;
		}
		else
		{
			target -= GetForward();
			existCnt--;
			if (existCnt<0)SetDestroy();
		}
		objPosition -= direction * speed;
		
		// 当たり判定
		Scene* scene = Manager::GetScene();
		std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>();

		if (ColisionJudge(*this, *player,1.25) == true)
		{
			if (player->GetMode() != INVICIBLE)
			{
				if (cnt == 0 && player->GetMode() == GUARD) knockPow /= 2;
				if (knockBack == true) { player->SetKnock(knockPow, direction); }
				if (blowBack == true) { player->SetBlow(knockPow, direction); }
				if(player->GetMode()!=GUARD)player->SetDamage(damage);
			}
			scene->AddGameObject<Explosion>()->SetPosition(player->GetPosition());
			SetDestroy();
			return;
		}
		cnt++;
	}
	else {
		SetDestroy();
	}
}

void EBullet::Draw()
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
	D3DXMatrixTranslation(&trans, objPosition.x, objPosition.y, objPosition.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	// モデル描画
	enemyBulletModel->Draw();
}

void EBullet::Load()
{
	// モデル読み込み
	enemyBulletModel = new Model();
	enemyBulletModel->Load("asset\\model\\FireBall\\FireBall.obj");
}

void EBullet::UnLoad()
{
	enemyBulletModel->Unload();
	delete enemyBulletModel;
}

void EBullet::Rotate()
{
	D3DXVECTOR3 dir = objPosition - target;
	D3DXVec3Normalize(&dir, &dir);
	
	if (objPosition.z > target.z)objRotation.y = dir.x;
	else objRotation.y = -dir.x + 1.57 * 2;
}

