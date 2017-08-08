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
#include "ObstacleCar\ObstacleCar.h"
#include "PointItem\PointItem.h"
#include "Scaffold\Scaffold.h"
#include "Syringe\Syringe.h"
#include "Golden\Golden.h"
#include "Scene\GameScene\CollisionManager\CollisionManager.h"
#include "..\..\..\GameDataManager\GameDataManager.h"
#include <time.h>


//----------------------------------------------------------------------
// Static Public Variables
//----------------------------------------------------------------------
const float GimmickManager::m_DrawRangeMin = -1000;
const float GimmickManager::m_DrawRangeMax = 2400;

LPCTSTR GimmickManager::m_FileNames[MAP_TYPE_MAX] = 
{
	"Resource\\GameScene\\StageMap1.csv",
	"Resource\\GameScene\\StageMap2.csv",
	"Resource\\GameScene\\StageMap3.csv",
	"Resource\\GameScene\\StageMap4.csv",
	"Resource\\GameScene\\StageMap5.csv",
	"Resource\\GameScene\\StageMap6.csv",
	"Resource\\GameScene\\StageMap7.csv",
	"Resource\\GameScene\\StageMap8.csv",
	"Resource\\GameScene\\StageMap9.csv",
	"Resource\\GameScene\\StageMap10.csv",
	"Resource\\GameScene\\StageMap11.csv",
	"Resource\\GameScene\\StageMap12.csv",
	"Resource\\GameScene\\StageMap13.csv",
	"Resource\\GameScene\\StageMap14.csv",
	"Resource\\GameScene\\StageMap15.csv"
};


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
GimmickManager::GimmickManager()
{
	m_pGimmick[NONE_GIMMICK] = new GimmickBase();
	m_pGimmick[OBSTACLE_GIMMICK] = new Obstacle();
	m_pGimmick[OBSTACLECAR_GIMMICK] = new ObstacleCar();
	m_pGimmick[JUMPRAMP_GIMMICK] = new JumpRamp();
	m_pGimmick[HANRAIL_GIMMICK] = new Handrail();
	m_pGimmick[POINT_GIMMICK] = new PointItem();
	m_pGimmick[SCAFFOLD_GIMMICK] = new Scaffold();
	m_pGimmick[SYRINGE_GIMMICK] = new Syringe();
	m_pGimmick[GOLDEN_GIMMICK] = new Golden();
}

GimmickManager::~GimmickManager()
{
	for (int i = 0; i < GIMMICK_MAX; i++)
	{
		delete m_pGimmick[i];
	}
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool GimmickManager::Initialize()
{
	m_pUpdateTask = new Lib::UpdateTask();
	m_pUpdateTask->SetUpdateObject(this);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	m_pGimmick[NONE_GIMMICK]->Initialize();
	m_pGimmick[OBSTACLE_GIMMICK]->Initialize();
	m_pGimmick[OBSTACLECAR_GIMMICK]->Initialize();
	m_pGimmick[JUMPRAMP_GIMMICK]->Initialize();
	m_pGimmick[HANRAIL_GIMMICK]->Initialize();
	m_pGimmick[SCAFFOLD_GIMMICK]->Initialize();	///@todo 描画優先順位をつけ忘れたので初期化順で優先順位をつけてる.
	m_pGimmick[POINT_GIMMICK]->Initialize();
	m_pGimmick[SYRINGE_GIMMICK]->Initialize();
	m_pGimmick[GOLDEN_GIMMICK]->Initialize();

	// オブジェクトの初期化.
	for (int i = 0; i < GIMMICK_MAX; i++)
	{
		SINGLETON_INSTANCE(CollisionManager)->AddGimmick(m_pGimmick[i]);
	}
	

	for (int i = 0; i < MAP_TYPE_MAX; i++)
	{
		// ファイル読み込み.
		FILE* pFile = nullptr;
		long FileSize = 0;
		char* pBuffer = nullptr;
		char* pContext = nullptr;
		char* pDataStr = nullptr;

		fopen_s(&pFile, m_FileNames[i], "r");

		// ファイルサイズの取得.
		fseek(pFile, 0, SEEK_END);
		FileSize = ftell(pFile) + 1;
		fseek(pFile, 0, SEEK_SET);

		// ファイルのデータを格納するバッファ.
		pBuffer = new char[FileSize];
		ZeroMemory(pBuffer, FileSize);

		// ファイルの読み込み.
		fread(pBuffer, FileSize, 1, pFile);
		pBuffer[FileSize - 1] = '\0';

		// ファイル内のデータを見つけて取得.
		pBuffer = strstr(pBuffer, "{");
		strcpy_s(pBuffer, FileSize, strtok_s(pBuffer, "{}", &pContext));
		pDataStr = strtok_s(pBuffer, "{},\n", &pContext);

		while (1)
		{
			if (pDataStr == nullptr)
			{
				break;	// データ全てを取り出すまでループ.
			}
			else
			{
				m_StageData.push_back(static_cast<float>(atof(pDataStr)));
			}

			pDataStr = strtok_s(nullptr, "{},\n", &pContext);
		}

		fclose(pFile);
		delete[] pBuffer;


		// マップデータの追加.
		for (unsigned int j = 0; j < m_StageData.size() / 4; j++)
		{
			m_MapData[i].push_back(MAP_DATA{ 
				static_cast<int>(m_StageData[j * 4]),
				static_cast<int>(m_StageData[j * 4 + 3]),
				D3DXVECTOR2(m_StageData[j * 4 + 1],	m_StageData[j * 4 + 2]) });
		}

		m_StageData.clear();
	}


	m_StageProgress = 0;

	m_StageProgress = 0.0f;
	m_IsStageLoad = false;


	// 初回ステージ読み込み.
	for (int j = 1; j < 13; j++)
	{
		srand(unsigned(time(NULL) + j));
		int Index = rand() % MAP_TYPE_MAX;
		for (unsigned int i = 0; i < m_MapData[Index].size(); i++)
		{
			m_pGimmick[static_cast<int>(m_MapData[Index][i].GimmickType)]->AddGimmick(
				D3DXVECTOR2(
				m_MapData[Index][i].GimmickPos.x + SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress() + 5000 * j,
				m_MapData[Index][i].GimmickPos.y),
				m_MapData[Index][i].GimmickStatus);
		}
	}

	return true;
}

void GimmickManager::Finalize()
{
	// ギミックの削除.
	SINGLETON_INSTANCE(CollisionManager)->RemoveGimmicks();
	for (unsigned int i = 0; i < m_StageData.size() / 4; i++)
	{
		m_pGimmick[static_cast<int>(m_StageData[i * 4])]->RemoveGimmick(
			D3DXVECTOR2(
			m_StageData[i * 4 + 1],
			m_StageData[i * 4 + 2]),
			static_cast<int>(m_StageData[i * 4 + 3]));
	}


	// オブジェクトの解放.
	for (int i = 0; i < GIMMICK_MAX; i++)
	{
		m_pGimmick[i]->Finalize();
	}

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	delete m_pUpdateTask;
}

void GimmickManager::Update()
{
	if ((SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress() - m_StageProgress) > 66000)
	{
		m_StageProgress = SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress();

		for (int i = 0; i < GIMMICK_MAX; i++)
		{
			m_pGimmick[i]->RemoveGimmick();
		}


		// ステージ読み込み処理.
		for (int j = 1; j < 13; j++)
		{
			srand(unsigned(time(NULL) + j));
			int Index = rand() % MAP_TYPE_MAX;
			for (unsigned int i = 0; i < m_MapData[Index].size(); i++)
			{
				m_pGimmick[static_cast<int>(m_MapData[Index][i].GimmickType)]->AddGimmick(
					D3DXVECTOR2(
					m_MapData[Index][i].GimmickPos.x + SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress() + 5000 * j,
					m_MapData[Index][i].GimmickPos.y),
					m_MapData[Index][i].GimmickStatus);
			}
		}
	}
}

void GimmickManager::Draw()
{
}
