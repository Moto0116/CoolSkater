/**
 * @file	ObjectManager.cpp
 * @brief	オブジェクト管理クラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "ObjectManager.h"

#include "StageManager\StageManager.h"
#include "Player\Player.h"
#include "ManualUI\ManualUI.h"
#include "ScoreUI\ScoreUI.h"
#include "LifeUI\LifeUI.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
ObjectManager::ObjectManager()
{
	m_pObjectManagers.push_back(new StageManager());

	m_pObjects.push_back(new Player());
	m_pObjects.push_back(new ManualUI());
	m_pObjects.push_back(new ScoreUI());
	m_pObjects.push_back(new LifeUI());
}

ObjectManager::~ObjectManager()
{
	for (auto itr = m_pObjects.begin(); itr != m_pObjects.end(); itr++)
	{
		delete (*itr);
	}

	for (auto itr = m_pObjectManagers.begin(); itr != m_pObjectManagers.end(); itr++)
	{
		delete (*itr);
	}
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool ObjectManager::Initialize()
{
	for (auto itr = m_pObjectManagers.begin(); itr != m_pObjectManagers.end(); itr++)
	{
		if (!(*itr)->Initialize())
		{
			Finalize();
			return false;
		}
	}

	for (auto itr = m_pObjects.begin(); itr != m_pObjects.end(); itr++)
	{
		if (!(*itr)->Initialize())
		{
			Finalize();
			return false;
		}
	}

	return true;
}

void ObjectManager::Finalize()
{
	for (auto itr = m_pObjects.begin(); itr != m_pObjects.end(); itr++)
	{
		(*itr)->Finalize();
	}

	for (auto itr = m_pObjectManagers.begin(); itr != m_pObjectManagers.end(); itr++)
	{
		(*itr)->Finalize();
	}
}

