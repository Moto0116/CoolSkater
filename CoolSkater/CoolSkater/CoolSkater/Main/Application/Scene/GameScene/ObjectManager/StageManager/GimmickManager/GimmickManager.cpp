/**
 * @file	GimmickManager.cpp
 * @brief	ギミック管理クラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "GimmickManager.h"

#include "Handrail\Handrail.h"
#include "JumpRamp\JumpRamp.h"
#include "Obstacle\Obstacle.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
GimmickManager::GimmickManager()
{
	m_pObjects.push_back(new Lib::ObjectBase());	// 空オブジェクト.
	m_pObjects.push_back(new Obstacle());
	m_pObjects.push_back(new JumpRamp());
	m_pObjects.push_back(new Handrail());
}

GimmickManager::~GimmickManager()
{
	for (auto itr = m_pObjects.begin(); itr != m_pObjects.end(); itr++)
	{
		delete (*itr);
	}
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool GimmickManager::Initialize()
{
	FILE* pFile = NULL;
	fopen_s(&pFile, "Resource\\GameScene\\Stage1.csv", "r");

	char Buffer[512];
	while (fgets(Buffer, _countof(Buffer), pFile))
	{
		char* pContext;
		char* Next = strtok_s(Buffer, ",\n", &pContext);
		while (Next)
		{
			m_StageData.push_back(atoi(Next));
			Next = strtok_s(NULL, ",\n", &pContext);
		}
	}

	fclose(pFile);

	for (unsigned int i = 0; i < m_StageData.size(); i++)
	{
		switch (m_StageData[i])
		{
		case NONE_GIMMICK:
			break;
		case OBSTACLE_GIMMICK:
			break;
		case JUMPRAMP_GIMMICK:
			break;
		case HANRAIL_GIMMICK:
			break;
		}
	}

	return true;
}

void GimmickManager::Finalize()
{
}

void GimmickManager::Update()
{
}

void GimmickManager::Draw()
{
}
