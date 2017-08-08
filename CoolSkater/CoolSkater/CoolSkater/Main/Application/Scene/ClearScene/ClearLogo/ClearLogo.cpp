/**
 * @file	ClearLogo.cpp
 * @brief	クリアロゴクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "ClearLogo.h"

#include <math.h>
#include "DirectX11\TextureManager\TextureManager.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
ClearLogo::ClearLogo()
{
	m_Pos = D3DXVECTOR2(1280, -340);
	m_Size = D3DXVECTOR2(0, 0);
}

ClearLogo::~ClearLogo()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool ClearLogo::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\ClearScene\\ClearLogo.png",
		&m_TextureIndex))
	{
		return false;
	}

	m_TmpPos = m_Pos;
	m_TargetPos = D3DXVECTOR2(640, 350);
	m_MinSize = D3DXVECTOR2(750, 400);
	m_MaxSize = D3DXVECTOR2(900, 500);
	m_Speed = 7.0f;
	m_Time = 0;

	D3DXVECTOR2 Vec;
	Vec.x = m_Pos.x - m_TargetPos.x;
	Vec.y = m_Pos.y - m_TargetPos.y;

	m_Angle = atan2(Vec.y, Vec.x);
	
	m_IsMove = false;

	// 仮コード.
	m_Pos = m_TargetPos;

	return true;
}

void ClearLogo::Finalize()
{
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);
	ReleaseVertex2D();

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);
}

void ClearLogo::Update()
{
	//if (m_Pos.x < m_TargetPos.x)
	//{
	//	m_Pos.x = m_TargetPos.x;
	//}
	//if (m_Pos.y > m_TargetPos.y)
	//{
	//	m_Pos.y = m_TargetPos.y;
	//}
	//if (m_Pos.x > m_TargetPos.x && m_Pos.y < m_TargetPos.y)
	//{
	//	m_Time += 18.0f;
	//	m_Pos.x = m_TmpPos.x - static_cast<float>(cos(m_Angle)*m_Time);
	//	m_Pos.y = m_TmpPos.y - static_cast<float>(sin(m_Angle)*m_Time);
	//}

	if (m_Size.x < m_MaxSize.x)
	{
		m_Size.x += 18;
		if (m_Size.x > m_MaxSize.x)
		{
			m_Size.x = m_MaxSize.x;
		}
	}

	if (m_Size.y < m_MaxSize.y)
	{
		m_Size.y += 10;
		if (m_Size.y > m_MaxSize.y)
		{
			m_Size.y = m_MaxSize.y;
		}
	}
}

void ClearLogo::Draw()
{
	m_pVertex->SetVertex(&m_Size);
	m_pVertex->WriteVertexBuffer();

	m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
	m_pVertex->ShaderSetup();
	m_pVertex->WriteConstantBuffer(&m_Pos);
	m_pVertex->Draw();
}
