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
#include "DirectX11\SoundManager\SoundManager.h"
#include "DirectX11\SoundManager\ISound\ISound.h"
#include "Scene\GameScene\GameScene.h"
#include "Scene\GameScene\GameDataManager\GameDataManager.h"
#include "..\..\Event\StageBackgroundEvent\StageBackgroundEvent.h"


//----------------------------------------------------------------------
// Static Private Variables
//----------------------------------------------------------------------
const D3DXVECTOR2 ScoreUI::m_ScorePos = D3DXVECTOR2(130, 53);
const D3DXVECTOR2 ScoreUI::m_ScoreSize = D3DXVECTOR2(32, 64);


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
ScoreUI::ScoreUI() : 
	m_Score(0),
	m_State(PAUSE_STATE),
	m_ProgressType(PROGRESS_ONE),
	m_ProgressTime(0),
	m_IsProgressDraw(false),
	m_IsSound(true),
	m_ProgressAlpha(1.0f)
{
	m_Pos = D3DXVECTOR2(80, 53);
	m_Size = D3DXVECTOR2(32, 32);

	m_ProgressPos = D3DXVECTOR2(270, 140);
	m_ProgressSize = D3DXVECTOR2(400, 50);

	m_StageProgressPos = D3DXVECTOR2(630, 50);
	m_StageProgressSize = D3DXVECTOR2(400, 50);

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

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\ScoreProgress.png",
		&m_ProgressTextureIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\UIBox.png",
		&m_TextureIndex2))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::SoundManager)->LoadSound(
		"Resource\\GameScene\\Chime.wav",
		&m_ChimeSoundIndex))
	{
		return false;
	}

	m_pEventListener = new ScoreUIEventListener(this, &ScoreUI::ReciveEvent);
	SINGLETON_INSTANCE(Lib::EventManager)->AddEventListener(m_pEventListener);

	m_pFont = new Lib::Font();
	m_pFont->Initialize(SINGLETON_INSTANCE(Lib::GraphicsDevice));
	m_pFont->CreateVertexBuffer(&m_ScoreSize, &D3DXCOLOR(0xffffffff));

	m_pScoreHistoryFont = new Lib::Font();
	m_pScoreHistoryFont->Initialize(SINGLETON_INSTANCE(Lib::GraphicsDevice));
	m_pScoreHistoryFont->CreateVertexBuffer(&D3DXVECTOR2(20, 40), &D3DXCOLOR(0xffffffff));

	m_pScoreHistoryFontY = new Lib::Font();
	m_pScoreHistoryFontY->SetTextureName(TEXT("Library\\DirectX11\\Font\\Texture\\FontY.png"));
	m_pScoreHistoryFontY->Initialize(SINGLETON_INSTANCE(Lib::GraphicsDevice));
	m_pScoreHistoryFontY->CreateVertexBuffer(&D3DXVECTOR2(20, 40), &D3DXCOLOR(0xffffffff));

	SINGLETON_INSTANCE(GameDataManager)->SetScore(m_Score);

	m_PrevScore = m_Score;

	return true;
}

void ScoreUI::Finalize()
{
	m_pScoreHistoryFontY->ReleaseVertexBuffer();
	m_pScoreHistoryFontY->Finalize();
	delete m_pScoreHistoryFontY;

	m_pScoreHistoryFont->ReleaseVertexBuffer();
	m_pScoreHistoryFont->Finalize();
	delete m_pScoreHistoryFont;

	m_pFont->ReleaseVertexBuffer();
	m_pFont->Finalize();
	delete m_pFont;
	
	SINGLETON_INSTANCE(Lib::EventManager)->RemoveEventListener(m_pEventListener);
	delete m_pEventListener;

	SINGLETON_INSTANCE(Lib::SoundManager)->ReleaseSound(m_ChimeSoundIndex);

	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex2);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_ProgressTextureIndex);
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
	if (m_PrevScore != SINGLETON_INSTANCE(GameDataManager)->GetScore())
	{
		m_History.push_back(
			SCORE_HISTORY{
			SINGLETON_INSTANCE(GameDataManager)->GetScore() - m_PrevScore, 
			0,
			SINGLETON_INSTANCE(GameDataManager)->GetIsGolden()});
	}

	for (auto itr = m_History.begin(); itr != m_History.end(); itr++)
	{
		(*itr).Counter++;
	}

	for (auto itr = m_History.begin(); itr != m_History.end(); itr++)
	{
		if ((*itr).Counter > 120)
		{
			m_History.erase(itr);
			break;
		}
	}

	switch (m_ProgressType)
	{
	case ScoreUI::PROGRESS_ONE:
		if (SINGLETON_INSTANCE(GameDataManager)->GetScore() > 1000)
		{
			m_IsProgressDraw = true;
			m_ProgressTime++;

			if (m_ProgressTime >= 60)
			{
				m_ProgressPos.y -= 0.3f;
				m_ProgressAlpha -= 0.008f;
				if (m_ProgressAlpha < 0.0f)
				{
					m_ProgressAlpha = 0.0f;
				}
			}

			if (m_ProgressTime >= 280)
			{
				m_ProgressPos = D3DXVECTOR2(270, 140);
				m_ProgressAlpha = 1.0f;
				m_IsSound = true;
				m_IsProgressDraw = false;
				m_ProgressTime = 0;
				m_ProgressType = PROGRESS_TWO;
			}
			else if (m_IsSound)
			{
				SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_ChimeSoundIndex)->SoundOperation(
					Lib::SoundManager::PLAY);
				m_IsSound = false;
			}
		}
		break;
	case ScoreUI::PROGRESS_TWO:
		if (SINGLETON_INSTANCE(GameDataManager)->GetScore() > 5000)
		{
			m_IsProgressDraw = true;
			m_ProgressTime++;

			if (m_ProgressTime >= 60)
			{
				m_ProgressPos.y -= 0.3f;
				m_ProgressAlpha -= 0.008f;
				if (m_ProgressAlpha < 0.0f)
				{
					m_ProgressAlpha = 0.0f;
				}
			}

			if (m_ProgressTime >= 280)
			{
				m_ProgressPos = D3DXVECTOR2(270, 140);
				m_ProgressAlpha = 1.0f;
				m_IsSound = true;
				m_IsProgressDraw = false;
				m_ProgressTime = 0;
				m_ProgressType = PROGRESS_THREE;
			}
			else if (m_IsSound)
			{
				SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_ChimeSoundIndex)->SoundOperation(
					Lib::SoundManager::PLAY);
				m_IsSound = false;
			}
		}
		break;
	case ScoreUI::PROGRESS_THREE:
		if (SINGLETON_INSTANCE(GameDataManager)->GetScore() > 10000)
		{
			m_IsProgressDraw = true;
			m_ProgressTime++;

			if (m_ProgressTime >= 60)
			{
				m_ProgressPos.y -= 0.3f;
				m_ProgressAlpha -= 0.008f;
				if (m_ProgressAlpha < 0.0f)
				{
					m_ProgressAlpha = 0.0f;
				}
			}

			if (m_ProgressTime >= 280)
			{
				m_ProgressPos = D3DXVECTOR2(270, 140);
				m_ProgressAlpha = 1.0f;
				m_IsSound = true;
				m_IsProgressDraw = false;
				m_ProgressTime = 0;
				m_ProgressType = PROGRESS_FOUR;
			}
			else if (m_IsSound)
			{
				SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_ChimeSoundIndex)->SoundOperation(
					Lib::SoundManager::PLAY);
				m_IsSound = false;
			}
		}
		break;
	case ScoreUI::PROGRESS_FOUR:
		if (SINGLETON_INSTANCE(GameDataManager)->GetScore() > 30000)
		{
			m_IsProgressDraw = true;
			m_ProgressTime++;

			if (m_ProgressTime >= 60)
			{
				m_ProgressPos.y -= 0.3f;
				m_ProgressAlpha -= 0.008f;
				if (m_ProgressAlpha < 0.0f)
				{
					m_ProgressAlpha = 0.0f;
				}
			}

			if (m_ProgressTime >= 280)
			{
				m_ProgressPos = D3DXVECTOR2(270, 140);
				m_ProgressAlpha = 1.0f;
				m_IsSound = true;
				m_IsProgressDraw = false;
				m_ProgressTime = 0;
				m_ProgressType = PROGRESS_FIVE;
			}
			else if (m_IsSound)
			{
				SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_ChimeSoundIndex)->SoundOperation(
					Lib::SoundManager::PLAY);
				m_IsSound = false;
			}
		}
		break;
	case ScoreUI::PROGRESS_FIVE:
		if (SINGLETON_INSTANCE(GameDataManager)->GetScore() > 50000)
		{
			m_IsProgressDraw = true;
			m_ProgressTime++;

			if (m_ProgressTime >= 60)
			{
				m_ProgressPos.y -= 0.3f;
				m_ProgressAlpha -= 0.008f;
				if (m_ProgressAlpha < 0.0f)
				{
					m_ProgressAlpha = 0.0f;
				}
			}

			if (m_ProgressTime >= 280)
			{
				m_ProgressPos = D3DXVECTOR2(270, 140);
				m_ProgressAlpha = 1.0f;
				m_IsSound = true;
				m_IsProgressDraw = false;
				m_ProgressTime = 0;
				m_ProgressType = PROGRESS_SIX;
			}
			else if (m_IsSound)
			{
				SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_ChimeSoundIndex)->SoundOperation(
					Lib::SoundManager::PLAY);
				m_IsSound = false;
			}
		}
		break;
	case ScoreUI::PROGRESS_SIX:
		if (SINGLETON_INSTANCE(GameDataManager)->GetScore() > 80000)
		{
			m_IsProgressDraw = true;
			m_ProgressTime++;

			if (m_ProgressTime >= 60)
			{
				m_ProgressPos.y -= 0.3f;
				m_ProgressAlpha -= 0.008f;
				if (m_ProgressAlpha < 0.0f)
				{
					m_ProgressAlpha = 0.0f;
				}
			}

			if (m_ProgressTime >= 280)
			{
				m_ProgressPos = D3DXVECTOR2(270, 140);
				m_ProgressAlpha = 1.0f;
				m_IsSound = true;
				m_IsProgressDraw = false;
				m_ProgressTime = 0;
				m_ProgressType = PROGRESS_SEVEN;
			}
			else if (m_IsSound)
			{
				SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_ChimeSoundIndex)->SoundOperation(
					Lib::SoundManager::PLAY);
				m_IsSound = false;
			}
		}
		break;
	case ScoreUI::PROGRESS_SEVEN:
		if (SINGLETON_INSTANCE(GameDataManager)->GetScore() > 100000)
		{
			m_IsProgressDraw = true;
			m_ProgressTime++;

			if (m_ProgressTime >= 60)
			{
				m_ProgressPos.y -= 0.3f;
				m_ProgressAlpha -= 0.008f;
				if (m_ProgressAlpha < 0.0f)
				{
					m_ProgressAlpha = 0.0f;
				}
			}

			if (m_ProgressTime >= 280)
			{
				m_ProgressPos = D3DXVECTOR2(270, 140);
				m_ProgressAlpha = 1.0f;
				m_IsSound = true;
				m_IsProgressDraw = false;
				m_ProgressTime = 0;
				m_ProgressType = PROGRESS_EIGHT;
			}
			else if (m_IsSound)
			{
				SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_ChimeSoundIndex)->SoundOperation(
					Lib::SoundManager::PLAY);
				m_IsSound = false;
			}
		}
		break;
	case ScoreUI::PROGRESS_EIGHT:
		if (SINGLETON_INSTANCE(GameDataManager)->GetScore() > 300000)
		{
			m_IsProgressDraw = true;
			m_ProgressTime++;

			if (m_ProgressTime >= 60)
			{
				m_ProgressPos.y -= 0.3f;
				m_ProgressAlpha -= 0.008f;
				if (m_ProgressAlpha < 0.0f)
				{
					m_ProgressAlpha = 0.0f;
				}
			}

			if (m_ProgressTime >= 280)
			{
				m_ProgressPos = D3DXVECTOR2(270, 140);
				m_ProgressAlpha = 1.0f;
				m_IsSound = true;
				m_IsProgressDraw = false;
				m_ProgressTime = 0;
				m_ProgressType = PROGRESS_NONE;
			}
			else if (m_IsSound)
			{
				SINGLETON_INSTANCE(Lib::SoundManager)->GetSound(m_ChimeSoundIndex)->SoundOperation(
					Lib::SoundManager::PLAY);
				m_IsSound = false;
			}
		}
		break;
	default:
		break;
	}

	m_PrevScore = SINGLETON_INSTANCE(GameDataManager)->GetScore();
}

void ScoreUI::PauseDraw()
{
	m_pVertex->SetVertex(&D3DXVECTOR2(320, 70));
	m_pVertex->SetUV(&D3DXVECTOR2(0, 0), &D3DXVECTOR2(1, 1));
	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, 1));
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex2));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&D3DXVECTOR2(m_Pos.x + 130, m_Pos.y));
	m_pVertex->Draw();

	m_pVertex->SetVertex(&m_Size);
	m_pVertex->SetUV(&D3DXVECTOR2(0, 0), &D3DXVECTOR2(1, 1));
	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, 1));
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	char DrawStr[32];
	sprintf_s(DrawStr, 32, "%d", SINGLETON_INSTANCE(GameDataManager)->GetScore());
	m_pFont->Draw(&m_ScorePos, DrawStr);

	ProgressDraw();

	for (auto itr = m_History.begin(); itr != m_History.end(); itr++)
	{
		char DrawStr[32];
		sprintf_s(DrawStr, 32, "+%d", (*itr).Score);
		if ((*itr).IsGolden)
		{
			m_pScoreHistoryFontY->Draw(&D3DXVECTOR2(m_Pos.x + 305, m_Pos.y + 40 - (*itr).Counter * 2), DrawStr);
		}
		else
		{
			m_pScoreHistoryFont->Draw(&D3DXVECTOR2(m_Pos.x + 305, m_Pos.y + 40 - (*itr).Counter * 2), DrawStr);
		}
	}
}

void ScoreUI::GameStartDraw()
{
	m_pVertex->SetVertex(&D3DXVECTOR2(320, 70));
	m_pVertex->SetUV(&D3DXVECTOR2(0, 0), &D3DXVECTOR2(1, 1));
	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, 1));
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex2));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&D3DXVECTOR2(m_Pos.x + 130, m_Pos.y));
	m_pVertex->Draw();

	m_pVertex->SetVertex(&m_Size);
	m_pVertex->SetUV(&D3DXVECTOR2(0, 0), &D3DXVECTOR2(1, 1));
	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, 1));
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	char DrawStr[32];
	sprintf_s(DrawStr, 32, "%d", SINGLETON_INSTANCE(GameDataManager)->GetScore());
	m_pFont->Draw(&m_ScorePos, DrawStr);

	ProgressDraw();

	for (auto itr = m_History.begin(); itr != m_History.end(); itr++)
	{
		char DrawStr[32];
		sprintf_s(DrawStr, 32, "+%d", (*itr).Score);
		if ((*itr).IsGolden)
		{
			m_pScoreHistoryFontY->Draw(&D3DXVECTOR2(m_Pos.x + 305, m_Pos.y + 40 - (*itr).Counter * 2), DrawStr);
		}
		else
		{
			m_pScoreHistoryFont->Draw(&D3DXVECTOR2(m_Pos.x + 305, m_Pos.y + 40 - (*itr).Counter * 2), DrawStr);
		}
	}
}

void ScoreUI::GameDraw()
{
	m_pVertex->SetVertex(&D3DXVECTOR2(320, 70));
	m_pVertex->SetUV(&D3DXVECTOR2(0, 0), &D3DXVECTOR2(1, 1));
	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, 1));
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex2));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&D3DXVECTOR2(m_Pos.x + 130, m_Pos.y));
	m_pVertex->Draw();

	m_pVertex->SetVertex(&m_Size);
	m_pVertex->SetUV(&D3DXVECTOR2(0, 0), &D3DXVECTOR2(1, 1));
	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, 1));
	m_pVertex->WriteVertexBuffer();
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	char DrawStr[32];
	sprintf_s(DrawStr, 32, "%d", SINGLETON_INSTANCE(GameDataManager)->GetScore());
	m_pFont->Draw(&m_ScorePos, DrawStr);

	ProgressDraw();

	for (auto itr = m_History.begin(); itr != m_History.end(); itr++)
	{
		char DrawStr[32];
		sprintf_s(DrawStr, 32, "+%d", (*itr).Score);
		if ((*itr).IsGolden)
		{
			m_pScoreHistoryFontY->Draw(&D3DXVECTOR2(m_Pos.x + 305, m_Pos.y + 40 - (*itr).Counter * 2), DrawStr);
		}
		else
		{
			m_pScoreHistoryFont->Draw(&D3DXVECTOR2(m_Pos.x + 305, m_Pos.y + 40 - (*itr).Counter * 2), DrawStr);
		}
	}
}

void ScoreUI::ProgressDraw()
{
	if (m_IsProgressDraw)
	{
		switch (m_ProgressType)
		{
		case ScoreUI::PROGRESS_ONE:
			m_pVertex->SetUV(&D3DXVECTOR2(0, 0), &D3DXVECTOR2(1, 0.0625));
			break;
		case ScoreUI::PROGRESS_TWO:
			m_pVertex->SetUV(&D3DXVECTOR2(0, 0.0625), &D3DXVECTOR2(1, 0.125));
			break;
		case ScoreUI::PROGRESS_THREE:
			m_pVertex->SetUV(&D3DXVECTOR2(0, 0.125), &D3DXVECTOR2(1, 0.1875));
			break;
		case ScoreUI::PROGRESS_FOUR:
			m_pVertex->SetUV(&D3DXVECTOR2(0, 0.1875), &D3DXVECTOR2(1, 0.25));
			break;
		case ScoreUI::PROGRESS_FIVE:
			m_pVertex->SetUV(&D3DXVECTOR2(0, 0.25), &D3DXVECTOR2(1, 0.3125));
			break;
		case ScoreUI::PROGRESS_SIX:
			m_pVertex->SetUV(&D3DXVECTOR2(0, 0.3125), &D3DXVECTOR2(1, 0.375));
			break;
		case ScoreUI::PROGRESS_SEVEN:
			m_pVertex->SetUV(&D3DXVECTOR2(0, 0.375), &D3DXVECTOR2(1, 0.4375));
			break;
		case ScoreUI::PROGRESS_EIGHT:
			m_pVertex->SetUV(&D3DXVECTOR2(0, 0.4375), &D3DXVECTOR2(1, 0.5));
			break;
		}

		m_pVertex->SetVertex(&m_ProgressSize);
		m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, m_ProgressAlpha));
		m_pVertex->WriteVertexBuffer();
		m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_ProgressTextureIndex));
		m_pVertex->ShaderSetup();
		m_pVertex->WriteConstantBuffer(&m_ProgressPos);
		m_pVertex->Draw();
	}
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
	case GameScene::STAGEBACKGROUND_EVENT:
		switch (reinterpret_cast<StageBackgroundEvent*>(_pEvent)->GetEvent())
		{
		case StageBackgroundEvent::GAMEUPDATE_EVENT:
			m_State = GAME_STATE;
			m_pUpdateFunc = &ScoreUI::GameUpdate;
			m_pDrawFunc = &ScoreUI::GameDraw;
			break;
		case StageBackgroundEvent::STAGECHANGE_EVENT1:

			break;
		}
		break;
	}
}
