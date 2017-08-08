/**
 * @file	ClearBackground.cpp
 * @brief	クリア背景クラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "ClearBackground.h"

#include "DirectX11\TextureManager\TextureManager.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
ClearBackground::ClearBackground()
{
	m_Pos = D3DXVECTOR2(640, 360);
	m_Size = D3DXVECTOR2(1280, 720);
}

ClearBackground::~ClearBackground()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool ClearBackground::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\ClearScene\\ClearBackground.png",
		&m_TextureIndex))
	{
		return false;
	}

	return true;
}

void ClearBackground::Finalize()
{
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);
	ReleaseVertex2D();

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);

}

void ClearBackground::Update()
{
}

void ClearBackground::Draw()
{
	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();
}
