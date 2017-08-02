/**
 * @file	LifeUI.cpp
 * @brief	ライフUIクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "LifeUI.h"

#include "Scene\GameScene\GameDataManager\GameDataManager.h"
#include "DirectX11\TextureManager\TextureManager.h"
#include "EventManager\EventManager.h"
#include "EventManager\EventBase\EventBase.h"
#include "Scene\GameScene\GameScene.h"


//----------------------------------------------------------------------
// Static Private Variables
//----------------------------------------------------------------------
const D3DXVECTOR2 LifeUI::m_LifePos = D3DXVECTOR2(1460, 73);
const D3DXVECTOR2 LifeUI::m_LifeSize = D3DXVECTOR2(32, 64);
const int LifeUI::m_DefaultLife = 2;


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
LifeUI::LifeUI() : 
	m_Life(m_DefaultLife),
	m_State(PAUSE_STATE)
{
	m_Pos = D3DXVECTOR2(1410, 70);
	m_Size = D3DXVECTOR2(32, 32);

	m_pUpdateFunc = &LifeUI::PauseUpdate;
	m_pDrawFunc = &LifeUI::PauseDraw;
}

LifeUI::~LifeUI()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool LifeUI::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Life.png",
		&m_TextureIndex))
	{
		return false;
	}

	m_pEventListener = new LifeUIEventListener(this, &LifeUI::ReciveEvent);
	SINGLETON_INSTANCE(Lib::EventManager)->AddEventListener(m_pEventListener);

	m_pFont = new Lib::Font();
	m_pFont->Initialize(SINGLETON_INSTANCE(Lib::GraphicsDevice));
	m_pFont->CreateVertexBuffer(&m_LifeSize, &D3DXCOLOR(0xffffffff));

	m_pEvent = new LifeUIEvent();

	return true;
}

void LifeUI::Finalize()
{
	delete m_pEvent;

	m_pFont->ReleaseVertexBuffer();
	m_pFont->Finalize();
	delete m_pFont;

	SINGLETON_INSTANCE(Lib::EventManager)->RemoveEventListener(m_pEventListener);
	delete m_pEventListener;

	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);
	ReleaseVertex2D();

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);
}

void LifeUI::Update()
{
	(this->*m_pUpdateFunc)();
}

void LifeUI::Draw()
{
	(this->*m_pDrawFunc)();
}


//----------------------------------------------------------------------
// Private Functions
//----------------------------------------------------------------------
void LifeUI::PauseUpdate()
{
}

void LifeUI::GameStartUpdate()
{
}

void LifeUI::GameUpdate()
{
	if (m_Life <= 0)
	{
		SINGLETON_INSTANCE(Lib::EventManager)->SendEventMessage(m_pEvent);
	}
}

void LifeUI::PauseDraw()
{
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	char DrawStr[32];
	sprintf_s(DrawStr, 32, "%d", m_Life);
	m_pFont->Draw(&m_LifePos, DrawStr);
}

void LifeUI::GameStartDraw()
{
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	char DrawStr[32];
	sprintf_s(DrawStr, 32, "%d", m_Life);
	m_pFont->Draw(&m_LifePos, DrawStr);
}

void LifeUI::GameDraw()
{
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	char DrawStr[32];
	sprintf_s(DrawStr, 32, "%d", m_Life);
	m_pFont->Draw(&m_LifePos, DrawStr);
}

void LifeUI::ReciveEvent(Lib::EventBase* _pEvent)
{
	switch (_pEvent->GetEventID())
	{
	case GameScene::GAMESTART_EVENT:
		m_State = GAME_START_STATE;
		m_pUpdateFunc = &LifeUI::GameStartUpdate;
		m_pDrawFunc = &LifeUI::GameStartDraw;
		break;
	case GameScene::GAMEUPDATE_EVENT:
		m_State = GAME_STATE;
		m_pUpdateFunc = &LifeUI::GameUpdate;
		m_pDrawFunc = &LifeUI::GameDraw;
		break;
	}
}
