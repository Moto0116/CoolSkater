/**
 * @file	TitleText.cpp
 * @brief	タイトルテキストクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "TitleText.h"

#include "DirectX11\TextureManager\TextureManager.h"
#include "InputDeviceManager\InputDeviceManager.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
TitleText::TitleText() : 
	m_IsGameStart(false),
	m_IsFlash(false),
	m_AlphaColor(1.0f),
	m_IsScoreStart(false),
	m_IsScoreFlash(false),
	m_ScoreAlphaColor(1.0f)
{
	m_Pos = D3DXVECTOR2(640, 650);
	m_Size = D3DXVECTOR2(750, 70);
}

TitleText::~TitleText()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool TitleText::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\TitleScene\\PushToSpace.png",
		&m_TextureIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\TitleScene\\PushToEscape.png",
		&m_TextureIndex2))
	{
		return false;
	}
	
	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\TitleScene\\PushToR.png",
		&m_TextureIndex3))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\TitleScene\\TextBox.png",
		&m_TextureIndex4))
	{
		return false;
	}

	m_IsGameStart = false;
	m_IsFlash = false;
	m_AlphaColor = 1.0f;
	m_IsScoreStart = false;
	m_IsScoreFlash = false;
	m_ScoreAlphaColor = 1.0f;

	return true;
}

void TitleText::Finalize()
{
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex4);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex3);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex2);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);
	ReleaseVertex2D();

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);
}

void TitleText::Update()
{
	if (SINGLETON_INSTANCE(Lib::InputDeviceManager)->GetKeyState()[DIK_Z] == Lib::KeyDevice::KEY_PUSH &&
		m_IsScoreStart == false)
	{
		m_IsGameStart = true;
	}
	else if (SINGLETON_INSTANCE(Lib::InputDeviceManager)->GetKeyState()[DIK_LSHIFT] == Lib::KeyDevice::KEY_PUSH &&
		m_IsGameStart == false)
	{
		m_IsScoreStart = true;
	}

	if (m_IsGameStart)
	{
		if (m_IsFlash)
		{
			m_AlphaColor -= 0.1f;
			if (m_AlphaColor <= 0.0f)
			{
				m_IsFlash = false;
			}
		}
		else
		{
			m_AlphaColor += 0.1f;
			if (m_AlphaColor >= 1.0f)
			{
				m_IsFlash = true;
			}
		}
	}

	if (m_IsScoreStart)
	{
		if (m_IsScoreFlash)
		{
			m_ScoreAlphaColor -= 0.1f;
			if (m_ScoreAlphaColor <= 0.0f)
			{
				m_IsScoreFlash = false;
			}
		}
		else
		{
			m_ScoreAlphaColor += 0.1f;
			if (m_ScoreAlphaColor >= 1.0f)
			{
				m_IsScoreFlash = true;
			}
		}
	}
}

void TitleText::Draw()
{
	m_Pos = D3DXVECTOR2(640, 490);

	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, m_AlphaColor));
	m_pVertex->WriteVertexBuffer();

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex4));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&D3DXVECTOR2(m_Pos.x, m_Pos.y + 3));
	m_pVertex->Draw();

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();


	m_Pos = D3DXVECTOR2(640, 580);

	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, 1));
	m_pVertex->WriteVertexBuffer();

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex4));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex2));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();


	m_Pos = D3DXVECTOR2(640, 670);

	m_pVertex->SetColor(&D3DXCOLOR(1, 1, 1, m_ScoreAlphaColor));
	m_pVertex->WriteVertexBuffer();

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex4));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex3));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();
}

