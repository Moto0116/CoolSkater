/**
 * @file	StageManager.cpp
 * @brief	ステージ管理クラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "StageManager.h"

#include "StageBackground\StageBackground.h"
#include "GimmickManager\GimmickManager.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
StageManager::StageManager()
{
	m_pObjects.push_back(new StageBackground());
	m_pObjects.push_back(new GimmickManager());
}

StageManager::~StageManager()
{
	for (auto itr = m_pObjects.begin(); itr != m_pObjects.end(); itr++)
	{
		delete (*itr);
	}
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool StageManager::Initialize()
{
	for (auto itr = m_pObjects.begin(); itr != m_pObjects.end(); itr++)
	{
		if (!(*itr)->Initialize())
		{
			return false;
		}
	}

	return true;
}

void StageManager::Finalize()
{
	for (auto itr = m_pObjects.begin(); itr != m_pObjects.end(); itr++)
	{
		(*itr)->Finalize();
	}
}
