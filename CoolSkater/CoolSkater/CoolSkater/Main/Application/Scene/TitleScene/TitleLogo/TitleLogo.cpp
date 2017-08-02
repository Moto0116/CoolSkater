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
	m_Pos = D3DXVECTOR2(800, 230);
	m_Size = D3DXVECTOR2(600, 180);
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
}

void TitleLogo::Draw()
{
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();
}

