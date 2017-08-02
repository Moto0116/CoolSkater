/**
 * @file	ScoreUI.cpp
 * @brief	スコアUIクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "ScoreUI.h"

#include "DirectX11\TextureManager\TextureManager.h"
#include "EventManager\EventManager.h"
#include "EventManager\EventBase\EventBase.h"
#include "Scene\GameScene\GameScene.h"


//----------------------------------------------------------------------
// Static Private Variables
//----------------------------------------------------------------------
const D3DXVECTOR2 ScoreUI::m_ScorePos = D3DXVECTOR2(140, 73);
const D3DXVECTOR2 ScoreUI::m_ScoreSize = D3DXVECTOR2(32, 64);


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
ScoreUI::ScoreUI() : 
	m_Score(0),
	m_State(PAUSE_STATE)
{
	m_Pos = D3DXVECTOR2(90, 70);
	m_Size = D3DXVECTOR2(32, 32);

	m_pUpdateFunc = &ScoreUI::PauseUpdate;
	m_pDrawFunc = &ScoreUI::PauseDraw;
}

ScoreUI::~ScoreUI()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool ScoreUI::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Score.png",
		&m_TextureIndex))
	{
		return false;
	}

	m_pEventListener = new ScoreUIEventListener(this, &ScoreUI::ReciveEvent);
	SINGLETON_INSTANCE(Lib::EventManager)->AddEventListener(m_pEventListener);

	m_pFont = new Lib::Font();
	m_pFont->Initialize(SINGLETON_INSTANCE(Lib::GraphicsDevice));
	m_pFont->CreateVertexBuffer(&m_ScoreSize, &D3DXCOLOR(0xffffffff));

	return true;
}

void ScoreUI::Finalize()
{
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

void ScoreUI::Update()
{
	(this->*m_pUpdateFunc)();
}

void ScoreUI::Draw()
{
	(this->*m_pDrawFunc)();
}


//----------------------------------------------------------------------
// Private Functions
//----------------------------------------------------------------------
void ScoreUI::PauseUpdate()
{
}

void ScoreUI::GameStartUpdate()
{
}

void ScoreUI::GameUpdate()
{
}

void ScoreUI::PauseDraw()
{
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	char DrawStr[32];
	sprintf_s(DrawStr, 32, "%d", m_Score);
	m_pFont->Draw(&m_ScorePos, DrawStr);
}

void ScoreUI::GameStartDraw()
{
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	char DrawStr[32];
	sprintf_s(DrawStr, 32, "%d", m_Score);
	m_pFont->Draw(&m_ScorePos, DrawStr);
}

void ScoreUI::GameDraw()
{
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	char DrawStr[32];
	sprintf_s(DrawStr, 32, "%d", m_Score);
	m_pFont->Draw(&m_ScorePos, DrawStr);
}

void ScoreUI::ReciveEvent(Lib::EventBase* _pEvent)
{
	switch (_pEvent->GetEventID())
	{
	case GameScene::GAMESTART_EVENT:
		m_State = GAME_START_STATE;
		m_pUpdateFunc = &ScoreUI::GameStartUpdate;
		m_pDrawFunc = &ScoreUI::GameStartDraw;
		break;
	case GameScene::GAMEUPDATE_EVENT:
		m_State = GAME_STATE;
		m_pUpdateFunc = &ScoreUI::GameUpdate;
		m_pDrawFunc = &ScoreUI::GameDraw;
		break;
	}
}
