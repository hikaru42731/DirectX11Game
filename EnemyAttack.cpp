#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Scene.h"
#include "Audio.h"
#include "GameObject.h"
#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"
#include "Camera.h"
#include "AnimationModel.h"
#include "AttackOmen.h"
#include "AtkWall.h"
#include "AttackJudge.h"

void Enemy::ShortRange1()
{
	if (enemyPlayer->GetAntiInduction() == true) { unInduct = true; enemyAtkCnt = 120; }
	
	//追従距離設定
	D3DXVECTOR3 chase = objPosition - startPos;
	float chaselength = D3DXVec3Length(&chase);

	if (unInduct != true && !enemyPlayer->GetArmor()) {		//誘導が切られるまではプレイヤーの座標を更新し続ける
		targetPos = enemyPlayer->GetPosition();
		if (targetPos.y < 1.0f) targetPos.y = 1.0f;
		EnemyRotate();
	}
	else
	{
		enemyAtkAction = false;
		if (enemyAtk != NULL)enemyAtk->SetDestroy();

		unInduct = false;
	}
	//追従方向設定
	D3DXVECTOR3 direction = objPosition - targetPos;
	D3DXVec3Normalize(&direction, &direction);

	if (enemyAtkCnt == 0) {chasePow = D3DXVec3Length(&direction); }
	attackFrame+=3;
	enemyAtkCnt++;
	if ((int)enemyAtkCnt % 5 == 0)attackSe->Play(false);
	
	if (enemyAtkCnt < 150 && chaselength!=chasePow) {

		if (enemyAtkCnt == 3) {	//判定出っぱなしにする
			enemyAtk = enemyScene->AddGameObject<EnemyAttackJudge>(); 
			
			enemyAtk->SetKnock(2.0f, GetForward());				//当たった時の吹っ飛びの力とベクトルを指定
			enemyAtk->SetPosition(objPosition - GetForward() * 2);	//位置
			enemyAtk->SetATKDamage(150, attackPower);					//アタック値
			enemyAtk->big = 2.5f;									//当たり判定の大きさ
			enemyAtk->SetExistTime(150);							//判定持続時間
		}
		if (enemyAtkCnt > 3) {
			enemyAtk->SetPosition(objPosition - GetForward() * 2);
			if (ColisionJudge(*enemyAtk, *enemyPlayer, enemyAtk->big)) { unInduct = false; enemyAtkAction = false; }
		}

		if (enemyAtkCnt>3&&enemyAtkCnt < 149 && !ColisionJudge(*this, *enemyPlayer, enemyAtk->big))
		{
			objPosition -= direction * 0.3f;
			objPosition.y = direction.y * 0.01f;
		}
		else if(enemyAtkCnt>3)
		{
			enemyAtk->SetDestroy();
			
			enemyAtkAction = false;
		}
	}
	else//HITしなければここにくる
	{
		
		unInduct = false;
		attackRate = 0;
		return;
	}
}

void Enemy::ShortRange2()
{
}

void Enemy::ShortRange3()
{

}

void Enemy::MiddleRange1()
{
	if (enemyPlayer->GetAntiInduction() == true) unInduct = true;
		
	
	if (unInduct != true&&!enemyPlayer->GetArmor()) {		//誘導が切られるまではプレイヤーの座標を更新し続ける
		EnemyRotate();
		targetPos = enemyPlayer->GetPosition();
		if (targetPos.y < 1.0f) targetPos.y = 1.0f;
	}
	else
	{
		targetPos = objPosition - GetForward();
	}
	//追従方向設定
	D3DXVECTOR3 direction = objPosition - targetPos;
	D3DXVec3Normalize(&direction, &direction);

	if (enemyAtkCnt == 0) { attackRate = 0; attackFrame = 0; chasePow = D3DXVec3Length(&direction); }
	
	if (enemyAtkCnt < 200)
	{
		attackFrame += 2;
		if (attackFrame > 100) attackFrame = 0;
	}
	else if (enemyAtkCnt >= 200)
	{
		//if((int)atk_cnt%2==0)
		attackFrame++;
	}
	enemyAtkCnt++;


	if (enemyAtkCnt < 240) {

		if (enemyAtkCnt < 200)
		{
			if (!ColisionJudge(*this, *enemyPlayer, 1.25))
			{
				objPosition -= direction * 0.4f;
				objPosition.y -= direction.y * 0.05f;
			}
		}
		if ((int)enemyAtkCnt % 15 == 0 && enemyAtkCnt < 200)
		{
			attackSe->Play(false);
			enemyAtk = enemyScene->AddGameObject<EnemyAttackJudge>();

			if (enemyPlayer->GetMode() == GUARD) 
			{
				enemyAtk->SetKnock(2.0f, GetForward());				//当たった時の吹っ飛びの力とベクトルを指定
			}
			else
			{
				enemyAtk->SetKnock(1.5f, GetForward());				//当たった時の吹っ飛びの力とベクトルを指定
			}
			enemyAtk->SetPosition(objPosition - GetForward() * 2);	//位置
			enemyAtk->SetATKDamage(20, attackPower);					//アタック値
			enemyAtk->big = 2.5f;									//当たり判定の大きさ
			enemyAtk->SetExistTime(15);
		}
		

		if (enemyAtkCnt == 200)
		{
			
			attackFrame = 100;
			D3DXVECTOR3 dir = objPosition - enemyPlayer->GetPosition();
			atkLen = D3DXVec3Length(&dir);
			if (atkLen < 60)
			{
				atkDestination = enemyPlayer->GetPosition() + enemyCamera->GetForward() * 3;
				atkDestination.y = enemyPlayer->GetPosition().y + 2;
				atkStartPos = objPosition;
				warpDir = atkStartPos - atkDestination;
				atkLen = D3DXVec3Length(&warpDir);
				warpSpeed = 0;
				warp = true;
			}
			else
			{
				atkLen = -50;
			}
		}
		if (warp == true)
		{
			if (enemyAtkCnt > 200 && enemyAtkCnt < 220)
			{
				EnemyRotate();
				D3DXVECTOR3 dir = objPosition - atkStartPos;
				moveLen = D3DXVec3Length(&dir);
				D3DXVECTOR3 playerpos = enemyPlayer->GetPosition() + enemyCamera->GetForward() * 3;
				D3DXVECTOR3 dirtoplayer = objPosition - playerpos;
				D3DXVec3Normalize(&dirtoplayer, &dirtoplayer);
				warpSpeed += atkLen * 0.01f;
				if (moveLen < atkLen)
				{
					objPosition -= dirtoplayer * warpSpeed;
					enemyAtkCnt == 220;
				}
			}
		}
		if (enemyAtkCnt == 220) warp = false;
		if (enemyAtkCnt == 230&&atkLen!=-50&&!enemyPlayer->GetArmor())
		{
			EnemyRotate();
			bigAttackSe->Play(false);
			enemyAtk = enemyScene->AddGameObject<EnemyAttackJudge>();

			enemyAtk->SetBlow(2.5f, GetForward());				//当たった時の吹っ飛びの力とベクトルを指定
			enemyAtk->SetPosition(enemyPlayer->GetPosition());	//位置
			enemyAtk->SetATKDamage(100, attackPower);					//アタック値
			enemyAtk->big = 2.5f;									//当たり判定の大きさ
			enemyAtk->SetExistTime(15);
			aiCnt = -200;
		}
	}
	else//HITしなければここにくる
	{
		enemyAtkAction = false;
		unInduct = false;
		attackRate = 0;
		return;
	}
}

void Enemy::MiddleRange2()
{
}

void Enemy::MiddleRange3()
{
}

void Enemy::LongRange1()
{
	if (enemyPlayer->GetAntiInduction() == true)unInduct = true;
	attackRate = 1.0f;
	attackFrame++;

	enemyAtkCnt++;
	if (unInduct != true) {		//誘導が切られるまではプレイヤーの座標を更新し続ける
		targetPos = enemyPlayer->GetPosition();
		targetPos.y = enemyPlayer->GetPosition().y+1.0f;
	}
	D3DXVECTOR3 bulpos = D3DXVECTOR3(objPosition.x,objPosition.y + 1.0f,objPosition.z);

	if (enemyAtkCnt < 50 ) {
		if (enemyAtkCnt == 40) {			
			attackSe->Play(false);
			enemyBullet = enemyScene->AddGameObject<EBullet>();
			enemyBullet->SetPosition(bulpos);
			enemyBullet->SetBlow(2.0f, GetForward());	//弾ならベクトルは使わない
			enemyBullet->SetSpeed(0.75);
			enemyBullet->SetATKDamage(150, attackPower);
			enemyAtkAction = false;
		}
	}
}

void Enemy::LongRange2()
{
	if (enemyPlayer->GetAntiInduction() == true)unInduct = true;
	attackRate = 1.0f;
	
	enemyAtkCnt++;
	if (unInduct != true) {		//誘導が切られるまではプレイヤーの座標を更新し続ける
		targetPos = enemyPlayer->GetPosition();
		targetPos.y = enemyPlayer->GetPosition().y + 1.0f;
	}
	D3DXVECTOR3 bulpos = D3DXVECTOR3(objPosition.x, objPosition.y + 1.0f, objPosition.z);

	if (enemyAtkCnt < 20) attackFrame++;
	else if (enemyAtkCnt < 100)
	{
		if (enemyAtkCnt == 20) { enemyOmen = enemyScene->AddGameObject<Omen>(); enemyOmen->SetScale(D3DXVECTOR3(10, 10, 10)); atkWallOmenSe->Play(true); }
		if (enemyOmen != NULL)
		{
			enemyOmen->SetPosition(enemyPlayer->GetPosition());
		}
	}
	if(enemyAtkCnt>110)	attackFrame++;
	if (enemyAtkCnt == 130)
	{
		atkWallOmenSe->Stop();
		atkWallHitSe->Play(false);
		enemyWall = enemyScene->AddGameObject<AtkWall>();
		enemyWall->SetPosition(D3DXVECTOR3(enemyOmen->GetPosition().x, 0, enemyOmen->GetPosition().z));
		
		enemyAtk = enemyScene->AddGameObject<EnemyAttackJudge>();
		enemyAtk->SetArmorBreak(true);
		D3DXVECTOR3 atkposition = enemyOmen->GetPosition();
		atkposition.y = enemyPlayer->GetPosition().y;
		enemyAtk->SetPosition(atkposition);
		if (enemyPlayer->GetMode() != GUARD)
		{
			enemyAtk->SetBlow(3.0f, -GetUp());	//弾ならベクトルは使わない
		}
		else
		{
			enemyAtk->SetKnock(5.0f,GetForward());	//弾ならベクトルは使わない
		}
		enemyAtk->big=9.5f;
		enemyAtk->SetATKDamage(350, attackPower);
		enemyOmen->SetDestroy();
	}
	if (enemyAtkCnt == 150)enemyAtkAction = false;

}

void Enemy::LongRange3()
{
}

void Enemy::Counter1()
{
	//追従方向設定
	targetPos = enemyPlayer->GetPosition();
	if (enemyPlayer->GetAntiInduction() == true)unInduct = true;
	
	if (enemyAtkCnt<60||unInduct != true)
	{
		enemyPlayerDir = objPosition - targetPos;
		D3DXVec3Normalize(&enemyPlayerDir, &enemyPlayerDir);
	}
	
	if (enemyAtkCnt == 0)attackFrame = 0;

	if (enemyAtkCnt < 40) { if ((int)enemyAtkCnt % 2 == 0)attackFrame++; }
	else { attackFrame++; }
	enemyAtkCnt++;

	if (enemyAtkCnt < 110) {

		if (enemyAtkCnt == 20)
		{
			enemyAtk = enemyScene->AddGameObject<EnemyAttackJudge>();
			enemyAtk->SetKnock(0.0f, GetForward());				//当たった時の吹っ飛びの力とベクトルを指定
			enemyAtk->SetPosition(targetPos);	//位置
			enemyAtk->SetATKDamage(0, attackPower);					//アタック値
			attackChargeSe->Play(false);
		}
		if (enemyAtkCnt <= 60)
		{
			objPosition += enemyPlayerDir * 0.5f;
			if (enemyAtkCnt == 60)
			{
				chasePow = D3DXVec3Length(&enemyPlayerDir);
			}
		}
		

		if (enemyAtkCnt == 70)
		{
			enemyAtk = enemyScene->AddGameObject<EnemyAttackJudge>();
			enemyAtk->SetBlow(2.5f, GetForward());				//当たった時の吹っ飛びの力とベクトルを指定
			enemyAtk->SetATKDamage(300, attackPower);					//アタック値
			enemyAtk->big = 2.5f;									//当たり判定の大きさ
			enemyAtk->SetExistTime(50);							//判定持続時間
			enemyAtk->SetStop(20);
		}
		if (enemyAtkCnt > 60)
		{
			enemyAtk->SetPosition(objPosition - GetForward() * 2);	//位置
			if (enemyAtk != NULL) 
			{
				if (!ColisionJudge(*enemyAtk, *enemyPlayer, enemyAtk->big))
				{
					objPosition -= enemyPlayerDir * (chasePow * 2.0f);
				}
			}
		}
	}
	else//終わったらここにくる
	{
		revengeNum = 0;
		attackRate = 0;
		enemyAtkCnt = 0;
		mode = DEFAULT;
		return;
	}
}

void Enemy::Counter2()
{
	if (enemyAtkCnt == 0)
	{
		centerDir = objPosition - D3DXVECTOR3(0, 0, 0);
		centerLen = D3DXVec3Length(&centerDir);
		D3DXVec3Normalize(&centerDir, &centerDir);
		atkStartPos = objPosition;
	}
	enemyAtkCnt++;

	atkMoveDir = objPosition - atkStartPos;
	atkMoveLen = D3DXVec3Length(&atkMoveDir);
	
	if (atkMoveLen < centerLen)
	{
		warp = true;
		aiSpeed += 0.1f;
		objPosition -= centerDir * aiSpeed;
	}
	else
	{
		revengeNum = 0;
		aiSpeed = 0;
		aiCnt = -100;
		warp = false;
		enemyAtkCnt = 0;
		enemyAtkAction = true;
		unInduct = false;
		mode = ATTACK;	//mode=Revengeから抜ける
		
		return;
	}
	
}

void Enemy::Counter3()
{
}


