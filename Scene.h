#pragma once
#include <vector>
#include <list>
#include <typeinfo>
#include "GameObject.h"



class Scene
{
protected:
	// STLのリスト構造　継承元のポインタで管理できる
	std::list<GameObject*> gameObject; // ポインタ変数
	std::list<GameObject*> game2dObject;
	std::list<GameObject*> unbreakObj;
	int cnt;
public:
	Scene() {}          // コンストラクタ
	virtual ~Scene() {}	// デストラクタ

	virtual bool Init() = 0;

	template <typename T> // オブジェクト追加
	T* AddGameObject()
	{
		T* gameobject = new T();
		gameobject->Init();
		gameObject.push_back(gameobject);

		return gameobject;
	}

	template <typename T> //エネミー追加
	T* AddEnemyGameObject(int type,int id)
	{
		T* gameobject = new T();
		gameobject->SetType(type);
		gameobject->SetID(id);
		gameobject->Init();
		
		gameObject.push_back(gameobject);

		return gameobject;
	}

	template <typename T> //UI等2Dオブジェクト追加
	T* Add2DGameObject()
	{
		T* gameobject = new T();
		gameobject->Init();
		game2dObject.push_back(gameobject);

		return gameobject;
	}

	template <typename T> //UI等2Dオブジェクト追加
	T* AddUnbreakGameObject()
	{
		T* gameObject = new T();
		gameObject->Init();
		unbreakObj.push_back(gameObject);

		return gameObject;
	}

	template <typename T> // テンプレート
	T* GetGameObject()
	{
		for (GameObject* object : gameObject)
		{
			if (typeid(*object) == typeid(T))// 型を調べる
			{
				return (T*)object;
			}
		}
		return NULL;
	}

	template <typename T> // テンプレート
	T* Get2DGameObject()
	{
		for (GameObject* object : game2dObject)
		{
			if (typeid(*object) == typeid(T))// 型を調べる
			{
				return (T*)object;
			}
		}
		return NULL;
	}

	template <typename T> // テンプレート
	T* GetUnbreakGameObject()
	{
		for (GameObject* object : unbreakObj)
		{
			if (typeid(*object) == typeid(T))// 型を調べる
			{
				return (T*)object;
			}
		}
		return NULL;
	}


	template <typename T> // テンプレート
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objects; //STLの配列
		for (GameObject* object : gameObject)
		{
			if (typeid(*object) == typeid(T))// 型を調べる
			{
				objects.push_back((T*)object);
			}
		}
		return objects;
	}

	virtual void Uninit()
	{
		// リスト構造の中身の数をループする
		for (GameObject* object : gameObject)
		{
			object->Uninit();
			delete object;
		}
		gameObject.clear(); // リストのクリア
		// リスト構造の中身の数をループする
		for (GameObject* object : game2dObject)
		{
			object->Uninit();
			delete object;
		}
		game2dObject.clear(); // リストのクリア

	}

	virtual void Update()
	{
		// リスト構造の中身の数をループする
	// objectは仮想クラスのGameObjectを利用した各々のインスタンスのポインタ　camera field playerなど
	// 　　　　　　　　　　　↓継承？
		for (GameObject* object : gameObject)
		{
			object->Update();
		}
		gameObject.remove_if
		(
			[](GameObject* object) 
			{
				return object->Destroy(); 
			}
		);

		for (GameObject* object : game2dObject)
		{
			object->Update();
		}
		game2dObject.remove_if
		(
			[](GameObject* object)
			{
				return object->Destroy();
			}
		);

		for (GameObject* object : unbreakObj)
		{
			object->Update();
		}
		unbreakObj.remove_if
		(
			[](GameObject* object)
			{
				return object->Destroy();
			}
		);
	}

	virtual void Draw()
	{
	// リスト構造の中身の数をループする
	// objectは仮想クラスのGameObjectを利用した各々のインスタンスのポインタ　camera field playerなど
		for (GameObject* object : gameObject)
		{
			object->Draw();
		}

		for (GameObject* object : game2dObject)
		{
			object->Draw();
		}
	}

};