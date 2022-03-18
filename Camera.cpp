#include "Main.h"
#include "Manager.h"
#include "Renderer.h"
#include "Debug_font.h"
#include "Scene.h"
#include "Input.h"
#include "Audio.h"
#include "Camera.h"
#include "Model.h"
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "PlayerHP.h"
#include "PlayerBar.h"
#include "TargetMark.h"
#include "ResultModel.h"
#include "TitleModel.h"
#include "Game.h"
#include "Game2.h"
#include "Game3.h"
#include <math.h>
#include <d3dx10math.h>

// プレイヤーインスタンス生成
D3DXVECTOR3 PlayerPosition;
D3DXVECTOR3 PlayerForward;

D3DXVECTOR3 prevPos;
//敵インスタンス生成
D3DXVECTOR3 EnemyPosition;
std::vector<Enemy*> cam_enemyList;
float move_y = 0.0f;
float Axel = 0;
// Camera:: でCameraクラスの関数を指定している
void Camera::Init()
{
	objPosition = D3DXVECTOR3(0.0f, 5.0f, -25.0f);
	//target = D3DXVECTOR3(0.0f, 0.5f, 0.0f);//プレイヤー座標
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotateSpeed = 0.05f;
	rotAngle = D3DXVECTOR3(1.57f, 0.0f, 1.57f);
	move_y = 0.0f;
	prevPos = D3DXVECTOR3(0, 0, 0);
	event = false;
	posReturn = true;
	cnt = 0;
	moveLength = 0;
	returnCnt = 0;
	enemyEmpty = false;
	mode = ENCOUNTER;

	targetSe = Manager::GetScene()->AddGameObject<Audio>();
	targetSe->Load("asset\\Audio\\TargetCamera.wav");
	targetSe->SetVolume(1.5f);
}

void Camera::Uninit()
{
	
}

void Camera::Update()
{
	player = Manager::GetScene()->GetGameObject<Player>();
	PlayerPosition = player->GetPosition();
	PlayerForward = player->GetForward();
	enemy = Manager::GetScene()->GetGameObject<Enemy>();
	marker = Manager::GetScene()->GetGameObject<TargetMark>();
	exist = player->SetTarget();
	Field* field = Manager::GetScene()->GetGameObject<Field>();
	switch(Manager::GetGameId())
	{
	case 0:
		if (Game::GetEnemyEmpty() == true) enemyEmpty = true;
		break;
	case 1:
		if (Game2::GetEnemyEmpty() == true) enemyEmpty = true;
		break;
	case 2:
		if (Game3::GetEnemyEmpty() == true) enemyEmpty = true;
		break;
	default:
		break;
	}
	if (enemyEmpty==false) {
		switch (mode)
		{
		case DEFAULT:
			if (event == false) {
				if (Input::GetKeyTrigger('Q'))
				{
					targetSe->Play(false);
					if (targetCam == false && exist->Exist == true)
					{
						targetEnemy = player->SetTarget();	//一番プレイヤーから近い敵を取得

						targetCam = true;
					}
					else
					{
						targetCnt = 0;
						targetCam = false;
					}
				}
				if (targetCam == false) {
					
					if (target != prevPos)
					{
						D3DXVECTOR3 dir = target - prevPos;
						target.x -= dir.x * 0.15f;
						target.z -= dir.z * 0.15f;
					}
					target.y = PlayerPosition.y + 5.0f;

					prevPos = PlayerPosition;

					if (Input::GetKeyPress('U')) //左回転
					{
						rotAngle.x -= rotateSpeed;
						rotAngle.y -= rotateSpeed;
						rotAngle.z -= rotateSpeed;

						objPosition.x = PlayerPosition.x + 4.5f * (cos(-rotAngle.x) * 2);
						objPosition.z = PlayerPosition.z + 4.5f * (sin(-rotAngle.z) * 2);
						objPosition.y = PlayerPosition.y + move_y + 7.5f;

						objRotation.y = rotAngle.y;
						if (objRotation.y == -1.57 * 4)rotAngle = D3DXVECTOR3(0, 0, 0);
					}
					else if (Input::GetKeyPress('O')) //右回転
					{
						rotAngle.x += rotateSpeed;
						rotAngle.y += rotateSpeed;
						rotAngle.z += rotateSpeed;

						objPosition.x = PlayerPosition.x + 4.5f * (cos(-rotAngle.x) * 2);
						objPosition.z = PlayerPosition.z + 4.5f * (sin(-rotAngle.z) * 2);
						objPosition.y = PlayerPosition.y + move_y + 7.5f;


						objRotation.y = rotAngle.y;

					}

					else//追従
					{
						objPosition.x = PlayerPosition.x + 4.5f * (cos(-rotAngle.x) * 2);
						objPosition.z = PlayerPosition.z + 4.5f * (sin(-rotAngle.z) * 2);
						objPosition.y = PlayerPosition.y + move_y + 7.5f;
					}

					if (Input::GetKeyPress('I')) //上を向く
					{
						if (objPosition.y > PlayerPosition.y + 1.0f) {
							//if (m_Position.y > field->GetHeight(m_Position)+1.0f) {
							objPosition.y -= 0.5f;
							move_y -= 0.5f;
							//}
						}
					}

					if (Input::GetKeyPress('K')) //下を向く
					{
						if (objPosition.y < PlayerPosition.y + 15.0f)
						{
							objPosition.y += 0.5f;
							move_y += 0.5f;
						}
					}
					//if (m_Position.y < field->GetHeight(m_Position)+1.5f) m_Position.y = field->GetHeight(m_Position)+1.5f;
				}
				else if (targetCam == true)
				{
					if (targetEnemy != NULL) {
						if (targetEnemy->Exist == false)
						{
							targetEnemy = NULL;
							targetCam = false;
							return;
						}
					}

					if (targetEnemy != NULL) {
						EnemyPosition = targetEnemy->GetPosition();
						dir = PlayerPosition - EnemyPosition;
						length = D3DXVec3Length(&dir);
						D3DXVec3Normalize(&dir, &dir);
					}
					
					target = PlayerPosition - (dir * (length / 2));

					D3DXVECTOR3 TargetDir = target - PlayerPosition;
					float t_length = D3DXVec3Length(&TargetDir) * 0.75f;
					if (Input::GetKeyPress('U')) //左回転
					{
						rotAngle.x -= rotateSpeed;
						rotAngle.y -= rotateSpeed;
						rotAngle.z -= rotateSpeed;

						if (t_length * 0.85 < 6.5f)
						{
							beforeCameraPos.x = target.x + 6.5f * (cos(-rotAngle.x) * 2.5);
							beforeCameraPos.z = target.z + 6.5f * (sin(-rotAngle.z) * 2.5);
							beforeCameraPos.y = target.y + move_y + 7.5f;
						}
						else {
							beforeCameraPos.x = target.x + t_length * 0.85 * (cos(-rotAngle.x) * 2.5);
							beforeCameraPos.z = target.z + t_length * 0.85 * (sin(-rotAngle.z) * 2.5);
							beforeCameraPos.y = PlayerPosition.y + move_y + 7.5f;
						}
						objRotation.y = rotAngle.y;
					}
					else if (Input::GetKeyPress('O')) //右回転
					{
						rotAngle.x += rotateSpeed;
						rotAngle.y += rotateSpeed;
						rotAngle.z += rotateSpeed;
						if (t_length * 0.85 < 6.5f)
						{
							beforeCameraPos.x = target.x + 6.5f * (cos(-rotAngle.x) * 2.5);
							beforeCameraPos.z = target.z + 6.5f * (sin(-rotAngle.z) * 2.5);
							beforeCameraPos.y = target.y + move_y + 7.5f;
						}
						else {
							beforeCameraPos.x = target.x + t_length * 0.85 * (cos(-rotAngle.x) * 2.5);
							beforeCameraPos.z = target.z + t_length * 0.85 * (sin(-rotAngle.z) * 2.5);
							beforeCameraPos.y = PlayerPosition.y + move_y + 7.5f;
						}
						objRotation.y = rotAngle.y;

					}
					else//追従
					{

						if (t_length * 0.85 < 6.5f)
						{

							beforeCameraPos.x = target.x + 6.5f * (cos(-rotAngle.x) * 2.5);
							beforeCameraPos.z = target.z + 6.5f * (sin(-rotAngle.z) * 2.5);
							beforeCameraPos.y = target.y + move_y + 7.5f;
						}
						else
						{
							beforeCameraPos.x = target.x + t_length * 0.85 * (cos(-rotAngle.x) * 2.5);
							beforeCameraPos.z = target.z + t_length * 0.85 * (sin(-rotAngle.z) * 2.5);
							float distanceY = PlayerPosition.y - EnemyPosition.y;
							fabsf(distanceY);

							if (distanceY < 7.5f)
							{
								beforeCameraPos.y = PlayerPosition.y + move_y + 7.5f;
							}
							else
							{
								beforeCameraPos.y = PlayerPosition.y + move_y + distanceY;
							}

						}
					}
					if (Input::GetKeyPress('I')) //上を向く
					{
						if (objPosition.y > PlayerPosition.y + 1.0f) {
							if (objPosition.y > 0.5f) {
								objPosition.y -= 0.5f;
								move_y -= 0.5f;
							}
						}

					}

					if (Input::GetKeyPress('K')) //下を向く
					{
						if (objPosition.y < PlayerPosition.y + 15.0f)
						{
							objPosition.y += 0.5f;
							move_y += 0.5f;
						}
					}
					if (event == false)move = false;
					D3DXVECTOR3 dir = PlayerPosition - EnemyPosition;
					float dis = D3DXVec3Length(&dir);
					if (dis > 7.5f && targetCnt > 10) {
						if (objPosition != beforeCameraPos && event == false)
						{
							D3DXVECTOR3 c_Dir = objPosition - beforeCameraPos;
							float distance = D3DXVec3Length(&c_Dir);
							D3DXVec3Normalize(&c_Dir, &c_Dir);
							if (distance > 5.0f && setPos == false) { returnCnt = 0; movePosLen = distance; beginPos = objPosition; setPos = true; }
							else if (distance <= 5.0f) { objPosition = beforeCameraPos; }
							if (setPos == true)
							{
								D3DXVECTOR3 movedir = objPosition - beginPos;
								float movelen = D3DXVec3Length(&movedir);
								if (movelen < movePosLen)
								{
									returnCnt++;
									objPosition -= c_Dir * 2.0f;
								}
								else
								{
									posReturn = true;
									setPos = false;
								}

							}
						}
					}
					else
					{
						objPosition = beforeCameraPos;
					}
					//if (m_Position.y < field->GetHeight(m_Position) + 1.5f) m_Position.y = field->GetHeight(m_Position) + 1.5f;
				}
			}
			else if (event == true)
			{
				if (targetEnemy != NULL)
				{
					EnemyPosition = targetEnemy->GetUnWall();
					dir = PlayerPosition - EnemyPosition;
					length = D3DXVec3Length(&dir);
					D3DXVec3Normalize(&dir, &dir);
				}
				if (move == true)
				{
					CameraMove();
				}

				target = PlayerPosition - (dir * (length / 2));
			}
			break;
		case ENCOUNTER:
			if (cnt == 0) {  objPosition = D3DXVECTOR3(15, 100, 0); }
			cnt++;
			if (cnt <= 500) {
				if (cnt < 100)
				{
					objPosition.y -= 1;
					moveCameraLength = -4;
				}
				else if (cnt <150) { }
				else if (cnt < 250)
				{
					target = D3DXVECTOR3(enemy->GetPosition().x, enemy->GetPosition().y + 2, enemy->GetPosition().z);
					objPosition = D3DXVECTOR3(enemy->GetPosition().x + 2, enemy->GetPosition().y + 2, enemy->GetPosition().z + moveCameraLength);
					moveCameraLength += 0.02f;
				}
				else if (cnt == 250) { moveCameraLength = 4; }
				else if (cnt < 350)
				{
					target = D3DXVECTOR3(PlayerPosition.x, PlayerPosition.y + 2, PlayerPosition.z);
					objPosition = D3DXVECTOR3(PlayerPosition.x + 2, PlayerPosition.y + 2, PlayerPosition.z + moveCameraLength);
					moveCameraLength -= 0.02f;
				}
				else if (cnt < 500)
				{
					if (cnt == 350)objPosition = D3DXVECTOR3(PlayerPosition.x, PlayerPosition.y + 6, 0);
					target = D3DXVECTOR3(0, 6, 0);
					objPosition.z -= 0.15;
				}
			}
			else
			{
				for (Enemy* enemy : cam_enemyList)
				{
					enemy->SetMode(DEFAULT);
				}
				player->SetMode(DEFAULT);
				Manager::GetScene()->Add2DGameObject<PlayerHP>();
				Manager::GetScene()->Add2DGameObject<Player_Bar>();
				mode = DEFAULT;
			}
			break;
		}
	}
	else
	{
		objPosition = PlayerPosition - PlayerForward * 15;
		objPosition.y = PlayerPosition.y+3;
	}

}


void Camera::Draw()
{
	// 見続ける場所
	D3DXMatrixLookAtLH(&viewTarget, &objPosition, &target,
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXMatrixRotationYawPitchRoll(&viewRotate, objRotation.y, objRotation.x, 0);
	viewMatrix = viewTarget;
	
	Renderer::SetViewMatrix(&viewMatrix);

	// プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&ProjectionMatrix, 1.15f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&ProjectionMatrix);
	
}

void Camera::CameraMove()
{
	D3DXVECTOR3 camdir = objPosition - goalPos;
	if (getLength == false) {
		moveLength = D3DXVec3Length(&camdir);
		if (moveLength > 1) {
			getLength = true;
		}
	}
	D3DXVec3Normalize(&camdir, &camdir);
	D3DXVECTOR3 dir = objPosition - startPos;
	moveDistance=D3DXVec3Length(&dir);
	
	if (moveDistance < moveLength)
	{
		objPosition -= camdir * 2.5f;
	}
	else
	{
		move = false;
	}
}

bool Camera::CheckView(D3DXVECTOR3 Position)
{
	D3DXMATRIX vp, invvp;

	vp = viewMatrix * ProjectionMatrix;
	D3DXMatrixInverse(&invvp, NULL, &vp);	//逆行列（ワールド座標を貰う）

	D3DXVECTOR3 vpos[4];
	D3DXVECTOR3 wpos[4];

	//左上
	vpos[0] = D3DXVECTOR3(-1.25f, 1.25f, 1.0f);	
	//右上
	vpos[1] = D3DXVECTOR3(1.25f, 1.25f, 1.0f);
	//左下
	vpos[2] = D3DXVECTOR3(-1.25f, -1.25f, 1.0f);
	//右下
	vpos[3] = D3DXVECTOR3(1.25f, -1.25f, 1.0f);	//奥の４隅

	D3DXVec3TransformCoord(&wpos[0], &vpos[0], &invvp);
	D3DXVec3TransformCoord(&wpos[1], &vpos[1], &invvp);
	D3DXVec3TransformCoord(&wpos[2], &vpos[2], &invvp);
	D3DXVec3TransformCoord(&wpos[3], &vpos[3], &invvp);	//座標変換

	D3DXVECTOR3 v, v1, v2, n;
	v = Position - objPosition;	//OBJの座標-カメラの座標でOBJまでのベクトルを求められる

	v1 = wpos[0] - objPosition;
	v2 = wpos[2] - objPosition;
	D3DXVec3Cross(&n, &v1, &v2);//左辺の法線ヴェクトル（外積計算）

	if (D3DXVec3Dot(&n, &v) < 0.0f) return false;	//内積計算

	v1 = wpos[3] - objPosition;
	v2 = wpos[1] - objPosition;
	D3DXVec3Cross(&n, &v1, &v2);//右辺の法線ヴェクトル（外積計算）

	if (D3DXVec3Dot(&n, &v) < 0.0f) return false;	//内積計算

	v1 = wpos[0] - objPosition;
	v2 = wpos[1] - objPosition;
	D3DXVec3Cross(&n, &v1, &v2);//上辺の法線ヴェクトル（外積計算）

	if (D3DXVec3Dot(&n, &v) > 0.0f) return false;	//内積計算

	//v1 = wpos[2] - m_Position;
	//v2 = wpos[3] - m_Position;
	//D3DXVec3Cross(&n, &v1, &v2);//下辺の法線ヴェクトル（外積計算）

	//if (D3DXVec3Dot(&n, &v) < 0.0f) return false;	//内積計算

	return true;
}

void R_Camera::Init()
{
	objPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void R_Camera::Uninit()
{
}

void R_Camera::Update()
{
	target = Manager::GetScene()->GetGameObject<R_Model>()->GetPosition();
	target.y = Manager::GetScene()->GetGameObject<R_Model>()->GetPosition().y+3;
	
	objPosition.x = target.x + 4.5f * (cos(rotateAngle) * 2);
	objPosition.z = target.z + 4.5f * (sin(rotateAngle) * 2);
	objPosition.y = target.y + 4.5f;
	rotateAngle += 0.01f;
}

void R_Camera::Draw()
{
	// 見続ける場所
	D3DXMatrixLookAtLH(&viewTarget, &objPosition, &target,
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXMatrixRotationYawPitchRoll(&viewRotate, objRotation.y, objRotation.x, 0);
	viewMatrix = viewTarget;

	Renderer::SetViewMatrix(&viewMatrix);

	// プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}

void T_Camera::Init()
{
	objPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	target = D3DXVECTOR3(0.0f, 6.5f, 0.0f);
	objRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void T_Camera::Uninit()
{
}

void T_Camera::Update()
{
	target.x = Manager::GetScene()->GetGameObject<T_Model>()->GetPosition().x;
	target.y = Manager::GetScene()->GetGameObject<T_Model>()->GetPosition().y + 2;
	target.z = Manager::GetScene()->GetGameObject<T_Model>()->GetPosition().z;

	objPosition.x = target.x + 1.5f * (cos(rotateAngle) * 2);
	objPosition.z = target.z + 1.5f * (sin(rotateAngle) * 2);
	objPosition.y = target.y +1.5f;
	rotateAngle += 0.005f;
}

void T_Camera::Draw()
{
	// 見続ける場所
	D3DXMatrixLookAtLH(&viewTarget, &objPosition, &target,
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXMatrixRotationYawPitchRoll(&viewRotate, objRotation.y, objRotation.x, 0);
	viewMatrix = viewTarget;

	Renderer::SetViewMatrix(&viewMatrix);

	// プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}
