/**
 * @file	Handrail.cpp
 * @brief	手すりクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Handrail.h"

#include "DirectX11\TextureManager\TextureManager.h"
#include "Scene\GameScene\GameDataManager\GameDataManager.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
Handrail::Handrail()
{
	m_Pos = D3DXVECTOR2(0, 0);
	m_Size = D3DXVECTOR2(384, 64);
}

Handrail::~Handrail()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool Handrail::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Handrail.png",
		&m_TextureIndex))
	{
		return false;
	}

	return true;
}

void Handrail::Finalize()
{
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);
	ReleaseVertex2D();

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);
}

void Handrail::Update()
{
}

void Handrail::Draw()
{
	for (auto itr = m_GimmickPos.begin(); itr != m_GimmickPos.end(); itr++)
	{
		D3DXVECTOR2 GimmickPos = *itr;
		GimmickPos.x -= SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress();

		if (GimmickPos.x < GimmickManager::m_DrawRangeMax &&GimmickPos.x > GimmickManager::m_DrawRangeMin)
		{
			m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
			m_pVertex->ShaderSetup();
			m_pVertex->WriteConstantBuffer(&GimmickPos);
			m_pVertex->Draw();
		}
	}
}

void Handrail::AddGimmick(D3DXVECTOR2 _pos)
{
	m_GimmickPos.push_back(_pos);
}

void Handrail::AddGimmick(D3DXVECTOR2 _pos, int _type)
{
	m_GimmickPos.push_back(_pos);
}

void Handrail::RemoveGimmick()
{
	m_GimmickPos.clear();
}

void Handrail::RemoveGimmick(D3DXVECTOR2 _pos)
{
	for (auto itr = m_GimmickPos.begin(); itr != m_GimmickPos.end(); itr++)
	{
		if ((*itr) == _pos)
		{
			m_GimmickPos.erase(itr);
			break;
		}
	}
}

void Handrail::RemoveGimmick(D3DXVECTOR2 _pos, int _type)
{
	for (auto itr = m_GimmickPos.begin(); itr != m_GimmickPos.end(); itr++)
	{
		if ((*itr) == _pos)
		{
			m_GimmickPos.erase(itr);
			break;
		}
	}
}

GimmickManager::GIMMICK_TYPE Handrail::Collision(D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize)
{
	for (auto itr = m_GimmickPos.begin(); itr != m_GimmickPos.end(); itr++)
	{
		if ((_pPos->x - _pSize->x / 2) < ((*itr).x + m_Size.x / 2) &&
			(_pPos->x + _pSize->x / 2) > ((*itr).x - m_Size.x / 2))
		{
			if ((_pPos->y - _pSize->y / 2) < ((*itr).y + m_Size.y / 2) &&
				(_pPos->y + _pSize->y / 2) > ((*itr).y - m_Size.y / 2))
			{
				return GimmickManager::HANRAIL_GIMMICK;
			}
		}
	}

	return GimmickManager::NONE_GIMMICK;
}

GimmickManager::GIMMICK_TYPE Handrail::Collision(
	D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize,
	D3DXVECTOR2* _pOutPos, D3DXVECTOR2* _pOutSize)
{
	for (auto itr = m_GimmickPos.begin(); itr != m_GimmickPos.end(); itr++)
	{
		if ((_pPos->x - _pSize->x / 2) < ((*itr).x + m_Size.x / 2) &&
			(_pPos->x + _pSize->x / 2) > ((*itr).x - m_Size.x / 2))
		{
			if ((_pPos->y - _pSize->y / 2) < ((*itr).y + m_Size.y / 2) &&
				(_pPos->y + _pSize->y / 2) > ((*itr).y - m_Size.y / 2))
			{
				*_pOutPos = D3DXVECTOR2((*itr).x - SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress(), (*itr).y);
				*_pOutSize = m_Size;
				return GimmickManager::HANRAIL_GIMMICK;
			}
		}
	}

	return GimmickManager::NONE_GIMMICK;
}
