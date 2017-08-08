/**
 * @file	ManualUI.cpp
 * @brief	マニュアルUIクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "ManualUI.h"

#include "Debugger\Debugger.h"
#include "InputDeviceManager\InputDeviceManager.h"
#include "Scene\GameScene\GameDataManager\GameDataManager.h"
#include "DirectX11\TextureManager\TextureManager.h"
#include "DirectX11\SoundManager\SoundManager.h"
#include "DirectX11\SoundManager\ISound\ISound.h"
#include "EventManager\EventManager.h"
#include "..\..\Event\ManualUIEvent\ManualUIEvent.h"


//----------------------------------------------------------------------
// Static Private Variables
//----------------------------------------------------------------------
const int ManualUI::m_ManualTime = 115;


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
ManualUI::ManualUI() : 
	m_Counter(0),
	m_State(PAUSE_STATE)
{
	m_Pos = D3DXVECTOR2(640, 360);
	m_Size = D3DXVECTOR2(600, 200);

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

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\PlayerWait.png",
		&m_TextureIndex2))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\StartSpace.png",
		&m_TextureIndex3))
	{
		return false;
	}


	if (!SINGLETON_INSTANCE(Lib::SoundManager)->LoadSound(
		"Resource\\GameScene\\Start.wav",
		&m_GameStartIndex))
	{
		OutputErrorLog("サウンドの読み込みに失敗しました");
		return false;
	}
	
	m_pEvent = new ManualUIEvent();
	m_IsSendMessage = false;
	m_IsManualOut = false;

	m_ManualAlphaColor = 0.9f;

	m_StartAlphaColor = 1.0f;
	m_IsStartAlphaFlash = true;

	return true;
}

void ManualUI::Finalize()
{
	delete m_pEvent;

	SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_GameStartIndex);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex3);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex2);
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
	const Lib::KeyDevice::KEYSTATE* pKey = SINGLETON_INSTANCE(Lib::InputDeviceManager)->GetKeyState();

	if (pKey[DIK_Z] == Lib::KeyDevice::KEY_PUSH && m_IsSendMessage == false)
	{
		m_IsSendMessage = true;

		SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_GameStartIndex)->SoundOperation(
			Lib::SoundManager::PLAY);
	}

	if (m_IsSendMessage)
	{
		m_Counter++;
		if (m_Counter >= m_ManualTime)
		{
			m_Counter = 0;

			m_IsManualOut = true;
			m_State = GAME_START_STATE;
			m_pUpdateFunc = &ManualUI::GameStartUpdate;
			m_pDrawFunc = &ManualUI::GameStartDraw;
			SINGLETON_INSTANCE(Lib::EventManager)->SendEventMessage(m_pEvent);
		}

		if (m_ManualAlphaColor > 0)
		{
			m_ManualAlphaColor -= 0.03f;
			if (m_ManualAlphaColor < 0)
			{
				m_ManualAlphaColor = 0.0f;
				m_IsManualOut = false;
			}
		}
	}

	if (m_IsSendMessage == false || m_StartAlphaColor != 0.0f)
	{
		if (m_IsStartAlphaFlash)
		{
			m_StartAlphaColor -= 0.015f;
			if (m_StartAlphaColor <= 0)
			{
				m_StartAlphaColor = 0.0f;
				m_IsStartAlphaFlash = false;
			}
		}
		else
		{
			m_StartAlphaColor += 0.015f;
			if (m_StartAlphaColor >= 1.0f)
			{
				m_StartAlphaColor = 1.0f;
				m_IsStartAlphaFlash = true;
			}
		}
	}
	else
	{
		m_StartAlphaColor -= 0.03f;
		if (m_StartAlphaColor <= 0)
		{
			m_StartAlphaColor = 0.0f;
		}
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
	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, m_ManualAlphaColor));
	m_pVertex->SetVertex(&m_Size);
	m_pVertex->WriteVertexBuffer();

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, m_StartAlphaColor));
	m_pVertex->SetVertex(&D3DXVECTOR2(500, 100));
	m_pVertex->WriteVertexBuffer();

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex3));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&D3DXVECTOR2(650, 650));
	m_pVertex->Draw();
}

void ManualUI::GameStartDraw()
{
}

void ManualUI::GameDraw()
{
}

