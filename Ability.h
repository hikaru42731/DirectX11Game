#pragma once
#include "GameObject.h"

// クラス継承によりメンバ変数をまとめる fieldクラスにGameObjectクラスを継承する
class Ability : public GameObject
{
	bool ability[11];
	bool game;	//シーンがゲームか
	class Player* abilityPlayer;
	class Scene* abilityScene;
public:
	void Init();   // 初期化関数
	void Uninit(); // 終了関数
	void Update(); // 更新関数
	void Draw();   // 描画関数
};