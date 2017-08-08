/**
 * @file	ポイントアイテムクラス実装
 * @brief	PointItem.cpp
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "PointItem.h"

#include "DirectX11\TextureManager\TextureManager.h"
#include "Scene\GameScene\GameDataManager\GameDataManager.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
PointItem::PointItem()
{
	m_Pos = D3DXVECTOR2(0, 0);
	m_Size = D3DXVECTOR2(52, 64);
}

PointItem::~PointItem()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool PointItem::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Gummy.png",
		&m_TextureIndex))
	{
		return false;
	}

	return true;
}

void PointItem::Finalize()
{
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);
	ReleaseVertex2D();

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);
}

void PointItem::Update()
{
}

void PointItem::Draw()
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		if ((*itr).IsActive)
		{
			D3DXVECTOR2 GimmickPos = (*itr).Pos;
			GimmickPos.x -= SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress();

			if (GimmickPos.x < GimmickManager::m_DrawRangeMax &&GimmickPos.x > GimmickManager::m_DrawRangeMin)
			{
				m_pVertex->SetVertex(&m_Size);
				m_pVertex->WriteVertexBuffer();

				m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
				m_pVertex->ShaderSetup();
				m_pVertex->WriteConstantBuffer(&GimmickPos);
				m_pVertex->Draw();
			}
		}
	}
}

void PointItem::AddGimmick(D3DXVECTOR2 _pos)
{
	m_GimmickData.push_back(GIMMICK_DATA(_pos, GUMMY_TYPE));
}

void PointItem::AddGimmick(D3DXVECTOR2 _pos, int _type)
{
	m_GimmickData.push_back(GIMMICK_DATA(_pos, _type));
}

void PointItem::RemoveGimmick()
{
	m_GimmickData.clear();
}

void PointItem::RemoveGimmick(D3DXVECTOR2 _pos)
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		if ((*itr) == GIMMICK_DATA(_pos, GUMMY_TYPE))
		{
			m_GimmickData.erase(itr);
			break;
		}
	}
}

void PointItem::RemoveGimmick(D3DXVECTOR2 _pos, int _type)
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		if ((*itr) == GIMMICK_DATA(_pos, _type))
		{
			m_GimmickData.erase(itr);
			break;
		}
	}
}

GimmickManager::GIMMICK_TYPE PointItem::Collision(D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize)
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		if ((_pPos->x - _pSize->x / 2) < ((*itr).Pos.x + m_Size.x / 2) &&
			(_pPos->x + _pSize->x / 2) > ((*itr).Pos.x - m_Size.x / 2))
		{
			if ((_pPos->y - _pSize->y / 2) < ((*itr).Pos.y + m_Size.y / 2) &&
				(_pPos->y + _pSize->y / 2) > ((*itr).Pos.y - m_Size.y / 2))
			{
				(*itr).IsActive = false;

				RemoveGimmick((*itr).Pos, (*itr).GimmickType);

				return GimmickManager::POINT_GIMMICK;
			}
		}
	}

	return GimmickManager::NONE_GIMMICK;
}

GimmickManager::GIMMICK_TYPE PointItem::Collision(
	D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize,
	D3DXVECTOR2* _pOutPos, D3DXVECTOR2* _pOutSize)
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		if ((_pPos->x - _pSize->x / 2) < ((*itr).Pos.x + m_Size.x / 2) &&
			(_pPos->x + _pSize->x / 2) > ((*itr).Pos.x - m_Size.x / 2))
		{
			if ((_pPos->y - _pSize->y / 2) < ((*itr).Pos.y + m_Size.y / 2) &&
				(_pPos->y + _pSize->y / 2) > ((*itr).Pos.y - m_Size.y / 2))
			{
				*_pOutPos = D3DXVECTOR2(
					(*itr).Pos.x - SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress(),
					(*itr).Pos.y);
				*_pOutSize = m_Size;
				(*itr).IsActive = false;

				RemoveGimmick((*itr).Pos, (*itr).GimmickType);

				return GimmickManager::POINT_GIMMICK;
			}
		}
	}

	return GimmickManager::NONE_GIMMICK;
}
