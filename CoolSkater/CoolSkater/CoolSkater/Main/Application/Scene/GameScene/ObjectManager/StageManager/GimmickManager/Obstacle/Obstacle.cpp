/**
 * @file	Obstacle.cpp
 * @brief	障害物クラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "Obstacle.h"

#include "DirectX11\TextureManager\TextureManager.h"
#include "Scene\GameScene\GameDataManager\GameDataManager.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
Obstacle::Obstacle()
{
	m_Pos = D3DXVECTOR2(0, 0);
	m_Size = D3DXVECTOR2(128, 128);
}

Obstacle::~Obstacle()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool Obstacle::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Obstacle.png",
		&m_TextureIndex))
	{
		return false;
	}

	return true;
}

void Obstacle::Finalize()
{
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);
	ReleaseVertex2D();

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);
}

void Obstacle::Update()
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		if ((*itr).IsDestroy)
		{
			(*itr).Angle += static_cast<float>(D3DXToRadian(6.0));
			(*itr).Pos.x += 19.0f;
			(*itr).Pos.y -= 19.0f;

			if (m_Pos.x > 1700 && m_Pos.y < -100)
			{
				(*itr).IsDestroy = false;
			}
		}
	}
}

void Obstacle::Draw()
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		D3DXVECTOR2 GimmickPos = (*itr).Pos;
		GimmickPos.x -= SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress();

		if (GimmickPos.x < GimmickManager::m_DrawRangeMax &&GimmickPos.x > GimmickManager::m_DrawRangeMin)
		{
			m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
			m_pVertex->ShaderSetup();
			m_pVertex->WriteConstantBuffer(&GimmickPos, &D3DXVECTOR2(1, 1), (*itr).Angle);
			m_pVertex->Draw();
		}
	}
}

void Obstacle::AddGimmick(D3DXVECTOR2 _pos)
{
	m_GimmickData.push_back(GIMMICK_DATA(false, _pos, 0));
}

void Obstacle::AddGimmick(D3DXVECTOR2 _pos, int _type)
{
	m_GimmickData.push_back(GIMMICK_DATA(false, _pos, 0));
}

void Obstacle::RemoveGimmick()
{
	m_GimmickData.clear();
}

void Obstacle::RemoveGimmick(D3DXVECTOR2 _pos)
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		if ((*itr).Pos == _pos)
		{
			m_GimmickData.erase(itr);
			break;
		}
	}
}

void Obstacle::RemoveGimmick(D3DXVECTOR2 _pos, int _type)
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		if ((*itr).Pos == _pos)
		{
			m_GimmickData.erase(itr);
			break;
		}
	}
}

GimmickManager::GIMMICK_TYPE Obstacle::Collision(D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize)
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		if ((_pPos->x - _pSize->x / 2) < ((*itr).Pos.x + m_Size.x / 2) &&
			(_pPos->x + _pSize->x / 2) > ((*itr).Pos.x - m_Size.x / 2))
		{
			if ((_pPos->y - _pSize->y / 2) < ((*itr).Pos.y + m_Size.y / 2) &&
				(_pPos->y + _pSize->y / 2) > ((*itr).Pos.y - m_Size.y / 2))
			{
				return GimmickManager::OBSTACLE_GIMMICK;
			}
		}
	}

	return GimmickManager::NONE_GIMMICK;
}

GimmickManager::GIMMICK_TYPE Obstacle::Collision(
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
				*_pOutPos = D3DXVECTOR2((*itr).Pos.x - SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress(), (*itr).Pos.y);
				*_pOutSize = m_Size;
				return GimmickManager::OBSTACLE_GIMMICK;
			}
		}
	}

	return GimmickManager::NONE_GIMMICK;
}

GimmickManager::GIMMICK_TYPE Obstacle::DestroyGimmick(D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize)
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		if ((_pPos->x - _pSize->x / 2) < ((*itr).Pos.x + m_Size.x / 2) &&
			(_pPos->x + _pSize->x / 2) > ((*itr).Pos.x - m_Size.x / 2))
		{
			if ((_pPos->y - _pSize->y / 2) < ((*itr).Pos.y + m_Size.y / 2) &&
				(_pPos->y + _pSize->y / 2) > ((*itr).Pos.y - m_Size.y / 2))
			{
				if ((*itr).IsDestroy == false)
				{
					(*itr).IsDestroy = true;
					return GimmickManager::OBSTACLE_GIMMICK;
				}
			}
		}
	}

	return GimmickManager::NONE_GIMMICK;
}

