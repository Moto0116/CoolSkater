/**
 * @file	StageBackground.h
 * @brief	ステージ背景クラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "StageBackground.h"

#include "Application.h"
#include "Scene\GameScene\GameScene.h"
#include "Scene\GameScene\GameDataManager\GameDataManager.h"
#include "DirectX11\TextureManager\TextureManager.h"
#include "EventManager\EventManager.h"
#include "EventManager\EventBase\EventBase.h"


//----------------------------------------------------------------------
// Static Private Variables
//----------------------------------------------------------------------
const D3DXVECTOR2 StageBackground::m_DefaultPos = D3DXVECTOR2(800, 360);


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
StageBackground::StageBackground() : 
	m_State(PAUSE_STATE),
	m_AlphaColor(0.3f),
	m_StageType(STAGE_1),
	Stage1Length(48000),
	Stage2Length(112000),
	Stage3Length(300800)
{
	m_Pos = m_DefaultPos;
	m_Size = D3DXVECTOR2(1600, 720);

	m_IsStage12 = false;
	m_IsStage23 = false;

	m_Stage12Pos = D3DXVECTOR2(0, 0);
	m_Stage12Size = D3DXVECTOR2(200, 720);
	m_Stage23Pos = D3DXVECTOR2(0, 0);
	m_Stage23Size = D3DXVECTOR2(200, 720);

	m_pUpdateFunc = &StageBackground::PauseUpdate;
	m_pDrawFunc = &StageBackground::PauseDraw;
}

StageBackground::~StageBackground()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool StageBackground::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Background1.png",
		&m_TextureIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Background2.png",
		&m_TextureIndex2))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Background3.png",
		&m_TextureIndex3))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Background4.png",
		&m_TextureIndex4))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\stage1_stage2.png",
		&m_Stage12TextureIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\stage2_stage3.png",
		&m_Stage23TextureIndex))
	{
		return false;
	}

	m_CurrentTextureIndex = m_TextureIndex;
	m_NextTextureIndex = m_TextureIndex;

	m_pEventListener = new StageBackgroundEventListener(this, &StageBackground::ReciveEvent);
	SINGLETON_INSTANCE(Lib::EventManager)->AddEventListener(m_pEventListener);

	m_pEvent = new StageBackgroundEvent();

	return true;
}

void StageBackground::Finalize()
{
	delete m_pEvent;

	SINGLETON_INSTANCE(Lib::EventManager)->AddEventListener(m_pEventListener);
	delete m_pEventListener;

	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_Stage23TextureIndex);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_Stage12TextureIndex);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex4);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex3);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex2);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);
	ReleaseVertex2D();

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);
}

void StageBackground::Update()
{
	(this->*m_pUpdateFunc)();
}

void StageBackground::Draw()
{
	(this->*m_pDrawFunc)();
}


//----------------------------------------------------------------------
// Private Functions
//----------------------------------------------------------------------
void StageBackground::PauseUpdate()
{
}

void StageBackground::GameStartUpdate()
{
	if (m_AlphaColor + 0.008f >= 1.0f)
	{
		m_State = GAME_STATE;
		m_pUpdateFunc = &StageBackground::GameUpdate;
		m_pDrawFunc = &StageBackground::GameDraw;

		m_pEvent->SetEvent(StageBackgroundEvent::GAMEUPDATE_EVENT);
		SINGLETON_INSTANCE(Lib::EventManager)->SendEventMessage(m_pEvent);
	}
	else
	{
		m_AlphaColor += 0.008f;
	}
}

void StageBackground::GameUpdate()
{
	// プレイヤーの速度分背景をスクロール.
	float PlayerSpeed = SINGLETON_INSTANCE(GameDataManager)->GetPlayerSpeed();
	m_Pos.x -= PlayerSpeed;

	if (m_Pos.x <= -m_DefaultPos.x)
	{
		m_Pos.x = m_DefaultPos.x + m_Pos.x + m_DefaultPos.x;
	}

	if (m_IsStage12)
	{
		float PlayerSpeed = SINGLETON_INSTANCE(GameDataManager)->GetPlayerSpeed();
		m_Stage12Pos.x -= PlayerSpeed;
		if (m_Stage12Pos.x < -100)
		{
			m_IsStage12 = false;
		}
	}
	
	if (m_IsStage23)
	{
		float PlayerSpeed = SINGLETON_INSTANCE(GameDataManager)->GetPlayerSpeed();
		m_Stage23Pos.x -= PlayerSpeed;
		if (m_Stage23Pos.x < -100)
		{
			m_IsStage23 = false;
		}
	}


	// プレイヤーの移動量を設定.
	float Progress = SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress();
	SINGLETON_INSTANCE(GameDataManager)->SetPlayerProgress(Progress + PlayerSpeed);

	switch (m_StageType)
	{
	case STAGE_1:
		if (SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress() >
			Stage1Length - 1600)
		{
			m_NextTextureIndex = m_TextureIndex2;
			if (m_IsStage12 == false)
			{
				m_Stage12Pos = D3DXVECTOR2(m_Pos.x + 1600 / 2, m_Pos.y);
			}
			m_IsStage12 = true;
		}

		if (SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress() >= Stage1Length)
		{
			m_CurrentTextureIndex = m_TextureIndex2;

			m_pEvent->SetEvent(StageBackgroundEvent::STAGECHANGE_EVENT1);
			SINGLETON_INSTANCE(Lib::EventManager)->SendEventMessage(m_pEvent);

			m_StageType = STAGE_2;
		}
		break;
	case STAGE_2:
		if (SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress() >
			Stage2Length - 1600)
		{
			m_NextTextureIndex = m_TextureIndex3;
			if (m_IsStage23 == false)
			{
				m_Stage23Pos = D3DXVECTOR2(m_Pos.x + 1600 / 2, m_Pos.y);
			}
			m_IsStage23 = true;
		}

		if (SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress() >= Stage2Length)
		{
			m_CurrentTextureIndex = m_TextureIndex3;

			m_pEvent->SetEvent(StageBackgroundEvent::STAGECHANGE_EVENT2);
			SINGLETON_INSTANCE(Lib::EventManager)->SendEventMessage(m_pEvent);

			m_StageType = STAGE_3;
		}
		break;
	case STAGE_3:
		if (SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress() >
			Stage3Length - 1600)
		{
			m_NextTextureIndex = m_TextureIndex4;
		}

		if (SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress() >= Stage3Length)
		{
			m_CurrentTextureIndex = m_TextureIndex4;

			m_pEvent->SetEvent(StageBackgroundEvent::STAGECHANGE_EVENT3);
			SINGLETON_INSTANCE(Lib::EventManager)->SendEventMessage(m_pEvent);

			m_StageType = STAGE_4;
		}
		break;
	}
}

void StageBackground::PauseDraw()
{
	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, m_AlphaColor));
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_CurrentTextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();
}

void StageBackground::GameStartDraw()
{
	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, m_AlphaColor));
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_CurrentTextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_NextTextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&D3DXVECTOR2(m_Pos.x + 1600, m_Pos.y));
	m_pVertex->Draw();
}

void StageBackground::GameDraw()
{
	m_pVertex->SetVertex(&m_Size);
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_CurrentTextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	m_pVertex->SetVertex(&m_Size);
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_NextTextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&D3DXVECTOR2(m_Pos.x + 1600, m_Pos.y));
	m_pVertex->Draw();

	if (m_IsStage12)
	{
		m_pVertex->SetVertex(&m_Stage12Size);
		m_pVertex->WriteVertexBuffer();

		m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_Stage12TextureIndex));
		m_pVertex->ShaderSetup();
		m_pVertex->WriteConstantBuffer(&m_Stage12Pos);
		m_pVertex->Draw();
	}

	if (m_IsStage23)
	{
		m_pVertex->SetVertex(&m_Stage23Size);
		m_pVertex->WriteVertexBuffer();

		m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_Stage23TextureIndex));
		m_pVertex->ShaderSetup();
		m_pVertex->WriteConstantBuffer(&m_Stage23Pos);
		m_pVertex->Draw();
	}
}

void StageBackground::ReciveEvent(Lib::EventBase* _pEvent)
{
	switch (_pEvent->GetEventID())
	{
	case GameScene::GAMESTART_EVENT:
		m_State = GAME_START_STATE;
		m_pUpdateFunc = &StageBackground::GameStartUpdate;
		m_pDrawFunc = &StageBackground::GameStartDraw;
		break;
	case GameScene::PLAYER_EVENT:
		m_State = PAUSE_STATE;
		m_pUpdateFunc = &StageBackground::PauseUpdate;
		m_pDrawFunc = &StageBackground::GameDraw;
		break;
	}
}

