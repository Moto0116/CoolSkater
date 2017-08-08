/**
 * @file	TitleLogo.cpp
 * @brief	タイトルロゴクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "TitleLogo.h"

#include "DirectX11\TextureManager\TextureManager.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
TitleLogo::TitleLogo()
{
	m_Pos = D3DXVECTOR2(640, 200);
	m_Size = D3DXVECTOR2(800, 250);
}

TitleLogo::~TitleLogo()
{
}

//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool TitleLogo::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\TitleScene\\TitleLogo.png",
		&m_TextureIndex))
	{
		return false;
	}

	m_IsUp = true;
	m_Counter = 0;

	return true;
}

void TitleLogo::Finalize()
{
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);
	ReleaseVertex2D();

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);
}

void TitleLogo::Update()
{
	if (m_IsUp)
	{
		m_Counter++;
		m_Pos.y -= 0.15f;
		if (m_Counter >= 80)
		{
			m_Counter = 0;
			m_IsUp = false;
		}
	}
	else
	{
		m_Counter++;
		m_Pos.y += 0.15f;
		if (m_Counter >= 80)
		{
			m_Counter = 0;
			m_IsUp = true;
		}
	}
}

void TitleLogo::Draw()
{
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();
}

