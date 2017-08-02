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
const D3DXVECTOR2 StageBackground::m_DefaultPos = D3DXVECTOR2(800, 450);


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
StageBackground::StageBackground() : 
	m_State(PAUSE_STATE),
	m_AlphaColor(0.3f)
{
	m_Pos = m_DefaultPos;
	m_Size = D3DXVECTOR2(1600, 900);

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
	if (m_AlphaColor + 0.009f >= 1.0f)
	{
		m_State = GAME_STATE;
		m_pUpdateFunc = &StageBackground::GameUpdate;
		m_pDrawFunc = &StageBackground::GameDraw;

		SINGLETON_INSTANCE(Lib::EventManager)->SendEventMessage(m_pEvent);
	}
	else
	{
		m_AlphaColor += 0.009f;
	}
}

void StageBackground::GameUpdate()
{
	m_Pos.x -= SINGLETON_INSTANCE(GameDataManager)->GetPlayerSpeed();

	if (m_Pos.x <= -m_DefaultPos.x)
	{
		m_Pos.x = m_DefaultPos.x;
	}
}

void StageBackground::PauseDraw()
{
	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, m_AlphaColor));
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();
}

void StageBackground::GameStartDraw()
{
	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, m_AlphaColor));
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&D3DXVECTOR2(m_Pos.x + Application::m_WindowWidth, m_Pos.y));
	m_pVertex->Draw();
}

void StageBackground::GameDraw()
{
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&D3DXVECTOR2(m_Pos.x + Application::m_WindowWidth, m_Pos.y));
	m_pVertex->Draw();
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
	}
}

