/**
 * @file	TitleBackground.cpp
 * @brief	タイトルバックグラウンドクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "TitleBackground.h"

#include "DirectX11\TextureManager\TextureManager.h"
#include <time.h>


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
TitleBackground::TitleBackground()
{
	m_Pos = D3DXVECTOR2(800, 450);
	m_Size = D3DXVECTOR2(1600, 900);
}

TitleBackground::~TitleBackground()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool TitleBackground::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}


	char* pTextureName = NULL;
	srand(static_cast<unsigned int>(time(nullptr)));
	switch (rand() % BACKGROUND_MAX)
	{
	case BACKGROUND_1:
		pTextureName = "Resource\\TitleScene\\TitleBackground.png";
		break;
	case BACKGROUND_2:
		pTextureName = "Resource\\TitleScene\\TitleBackground2.png";
		break;
	case BACKGROUND_3:
		pTextureName = "Resource\\TitleScene\\TitleBackground3.png";
		break;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		pTextureName,
		&m_TextureIndex))
	{
		return false;
	}

	return true;
}

void TitleBackground::Finalize()
{
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);
	ReleaseVertex2D();

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);
}

void TitleBackground::Update()
{
}

void TitleBackground::Draw()
{
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();
}

