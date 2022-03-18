#pragma once


class Manager
{
private:
	static class Scene* managerScene;
	static bool* ability1;
	static bool* ability2;
	static bool* ability3;
	static bool* ability4;
	static bool* ability5;
	static bool* ability6;
	static bool* ability7;
	static bool* ability8;
	static bool* ability9;
	static bool* ability10;
	static bool* ability11;
	static int gameId;	//ステージ数保持
	static int unLockStageNum;	//アンロックされたステージ数
public:

	static void Init();   // 初期化関数
	static void Uninit(); // 終了関数
	static void Update(); // 更新関数
	static void Draw();   // 描画関数

	static  class Scene* GetScene() { return managerScene; }

	static void SetAbility(bool* abil1, bool* abil2, bool* abil3, bool* abil4, bool* abil5, bool* abil6, bool* abil7, bool* abil8, bool* abil9, bool* abil10, bool* abil11)
	{
		ability1 = abil1; ability2 = abil2; ability3 = abil3; ability4 = abil4; ability5 = abil5; ability6 = abil6; ability7 = abil7; ability8 = abil8; ability9 = abil9; ability10 = abil10; ability11 = abil11;
	}
	static bool* GetAbility1() { return ability1; }
	static bool* GetAbility2() { return ability2; }
	static bool* GetAbility3() { return ability3; }
	static bool* GetAbility4() { return ability4; }
	static bool* GetAbility5() { return ability5; }
	static bool* GetAbility6() { return ability6; }
	static bool* GetAbility7() { return ability7; }
	static bool* GetAbility8() { return ability8; }
	static bool* GetAbility9() { return ability9; }
	static bool* GetAbility10() { return ability10; }
	static bool* GetAbility11() { return ability11; }
	static void SetGameId(int Id) { gameId = Id; }
	static int GetGameId() { return gameId; }
	static void SetUnLockStage(int UnLock) { unLockStageNum=UnLock; }
	static int GetUnLockStage() { return unLockStageNum; }
	
	template<typename T>
	static void SetScene()
	{
		if (managerScene)
		{
			managerScene->Uninit();
			delete managerScene;
		}

		T* scene = new T();
		managerScene = scene;
		scene->Init();
	}

};