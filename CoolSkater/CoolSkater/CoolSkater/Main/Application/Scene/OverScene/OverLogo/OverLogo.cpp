/**
 * @file	OverLogo.cpp
 * @brief	ゲームオーバーロゴ実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "OverLogo.h"

#include "DirectX11\TextureManager\TextureManager.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
OverLogo::OverLogo()
{
	m_Pos = D3DXVECTOR2(740, 340);
	m_Size = D3DXVECTOR2(900, 500);
}

OverLogo::~OverLogo()
{
}



//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool OverLogo::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\OverScene\\OverLogo.png",
		&m_TextureIndex))
	{
		return false;
	}

	return true;
}

void OverLogo::Finalize()
{
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);
	ReleaseVertex2D();

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);
}

void OverLogo::Update()
{
}

void OverLogo::Draw()
{
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();
}

