#pragma once
#include <vector>
#include <list>
#include <typeinfo>
#include "GameObject.h"



class Scene
{
protected:
	// STL�̃��X�g�\���@�p�����̃|�C���^�ŊǗ��ł���
	std::list<GameObject*> gameObject; // �|�C���^�ϐ�
	std::list<GameObject*> game2dObject;
	std::list<GameObject*> unbreakObj;
	int cnt;
public:
	Scene() {}          // �R���X�g���N�^
	virtual ~Scene() {}	// �f�X�g���N�^

	virtual bool Init() = 0;

	template <typename T> // �I�u�W�F�N�g�ǉ�
	T* AddGameObject()
	{
		T* gameobject = new T();
		gameobject->Init();
		gameObject.push_back(gameobject);

		return gameobject;
	}

	template <typename T> //�G�l�~�[�ǉ�
	T* AddEnemyGameObject(int type,int id)
	{
		T* gameobject = new T();
		gameobject->SetType(type);
		gameobject->SetID(id);
		gameobject->Init();
		
		gameObject.push_back(gameobject);

		return gameobject;
	}

	template <typename T> //UI��2D�I�u�W�F�N�g�ǉ�
	T* Add2DGameObject()
	{
		T* gameobject = new T();
		gameobject->Init();
		game2dObject.push_back(gameobject);

		return gameobject;
	}

	template <typename T> //UI��2D�I�u�W�F�N�g�ǉ�
	T* AddUnbreakGameObject()
	{
		T* gameObject = new T();
		gameObject->Init();
		unbreakObj.push_back(gameObject);

		return gameObject;
	}

	template <typename T> // �e���v���[�g
	T* GetGameObject()
	{
		for (GameObject* object : gameObject)
		{
			if (typeid(*object) == typeid(T))// �^�𒲂ׂ�
			{
				return (T*)object;
			}
		}
		return NULL;
	}

	template <typename T> // �e���v���[�g
	T* Get2DGameObject()
	{
		for (GameObject* object : game2dObject)
		{
			if (typeid(*object) == typeid(T))// �^�𒲂ׂ�
			{
				return (T*)object;
			}
		}
		return NULL;
	}

	template <typename T> // �e���v���[�g
	T* GetUnbreakGameObject()
	{
		for (GameObject* object : unbreakObj)
		{
			if (typeid(*object) == typeid(T))// �^�𒲂ׂ�
			{
				return (T*)object;
			}
		}
		return NULL;
	}


	template <typename T> // �e���v���[�g
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objects; //STL�̔z��
		for (GameObject* object : gameObject)
		{
			if (typeid(*object) == typeid(T))// �^�𒲂ׂ�
			{
				objects.push_back((T*)object);
			}
		}
		return objects;
	}

	virtual void Uninit()
	{
		// ���X�g�\���̒��g�̐������[�v����
		for (GameObject* object : gameObject)
		{
			object->Uninit();
			delete object;
		}
		gameObject.clear(); // ���X�g�̃N���A
		// ���X�g�\���̒��g�̐������[�v����
		for (GameObject* object : game2dObject)
		{
			object->Uninit();
			delete object;
		}
		game2dObject.clear(); // ���X�g�̃N���A

	}

	virtual void Update()
	{
		// ���X�g�\���̒��g�̐������[�v����
	// object�͉��z�N���X��GameObject�𗘗p�����e�X�̃C���X�^���X�̃|�C���^�@camera field player�Ȃ�
	// �@�@�@�@�@�@�@�@�@�@�@���p���H
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
	// ���X�g�\���̒��g�̐������[�v����
	// object�͉��z�N���X��GameObject�𗘗p�����e�X�̃C���X�^���X�̃|�C���^�@camera field player�Ȃ�
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