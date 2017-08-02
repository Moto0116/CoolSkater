/**
 * @file	ManualUI.cpp
 * @brief	マニュアルUIクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "ManualUI.h"

#include "Scene\GameScene\GameDataManager\GameDataManager.h"
#include "DirectX11\TextureManager\TextureManager.h"
#include "EventManager\EventManager.h"
#include "..\..\Event\ManualUIEvent\ManualUIEvent.h"


//----------------------------------------------------------------------
// Static Private Variables
//----------------------------------------------------------------------
const int ManualUI::m_ManualTime = 180;


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
ManualUI::ManualUI() : 
	m_Counter(0),
	m_State(PAUSE_STATE)
{
	m_Pos = D3DXVECTOR2(800, 450);
	m_Size = D3DXVECTOR2(1000, 512);

	m_pUpdateFunc = &ManualUI::PauseUpdate;
	m_pDrawFunc = &ManualUI::PauseDraw;
}

ManualUI::~ManualUI()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool ManualUI::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Manual.png",
		&m_TextureIndex))
	{
		return false;
	}

	m_pEvent = new ManualUIEvent();

	return true;
}

void ManualUI::Finalize()
{
	delete m_pEvent;

	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);
	ReleaseVertex2D();

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);
}

void ManualUI::Update()
{
	(this->*m_pUpdateFunc)();
}

void ManualUI::Draw()
{
	(this->*m_pDrawFunc)();
}


//----------------------------------------------------------------------
// Private Functions
//----------------------------------------------------------------------
void ManualUI::PauseUpdate()
{
	m_Counter++;
	if (m_Counter >= m_ManualTime)
	{
		m_Counter = 0;
		
		m_State = GAME_START_STATE;
		m_pUpdateFunc = &ManualUI::GameStartUpdate;
		m_pDrawFunc = &ManualUI::GameStartDraw;
		SINGLETON_INSTANCE(Lib::EventManager)->SendEventMessage(m_pEvent);
	}
}

void ManualUI::GameStartUpdate()
{
}

void ManualUI::GameUpdate()
{
}

void ManualUI::PauseDraw()
{
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();
}

void ManualUI::GameStartDraw()
{
}

void ManualUI::GameDraw()
{
}

