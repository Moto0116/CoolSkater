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


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
TitleText::TitleText()
{
	m_Pos = D3DXVECTOR2(800, 650);
	m_Size = D3DXVECTOR2(750, 90);
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

	return true;
}

void TitleText::Finalize()
{
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex2);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);
	ReleaseVertex2D();

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);
}

void TitleText::Update()
{
}

void TitleText::Draw()
{
	m_Pos = D3DXVECTOR2(800, 650);

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();


	m_Pos = D3DXVECTOR2(800, 760);

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex2));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();
}

