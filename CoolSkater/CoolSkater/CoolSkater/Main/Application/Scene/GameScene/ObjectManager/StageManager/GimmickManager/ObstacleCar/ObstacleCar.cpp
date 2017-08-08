/**
 * @file	ObstacleCar.cpp
 * @brief	障害物車クラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "ObstacleCar.h"

#include "DirectX11\TextureManager\TextureManager.h"
#include "Scene\GameScene\GameDataManager\GameDataManager.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
ObstacleCar::ObstacleCar()
{
	m_Pos = D3DXVECTOR2(0, 0);
	m_Size = D3DXVECTOR2(440, 140);
}

ObstacleCar::~ObstacleCar()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool ObstacleCar::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Car.png",
		&m_TextureIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Bus.png",
		&m_TextureIndex2))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Limo.png",
		&m_TextureIndex3))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Car2.png",
		&m_TextureIndex4))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Bus2.png",
		&m_TextureIndex5))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Car3.png",
		&m_TextureIndex6))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Car4.png",
		&m_TextureIndex7))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\Limo2.png",
		&m_TextureIndex8))
	{
		return false;
	}

	return true;
}

void ObstacleCar::Finalize()
{
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex8);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex7);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex6);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex5);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex4);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex3);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex2);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);
	ReleaseVertex2D();

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);
}

void ObstacleCar::Update()
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

void ObstacleCar::Draw()
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		int TextureIndex = Lib::TextureManager::m_InvalidIndex;
		switch ((*itr).GimmickType)
		{
		case CAR_TYPE:
			TextureIndex = m_TextureIndex;
			m_Size = D3DXVECTOR2(440, 140);
			break;
		case BUS_TYPE:
			TextureIndex = m_TextureIndex2;
			m_Size = D3DXVECTOR2(700, 280);
			break;
		case LIMO_TYPE:
			TextureIndex = m_TextureIndex3;
			m_Size = D3DXVECTOR2(700, 150);
			break;
		case CAR_SCRAPPED_TYPE:
			TextureIndex = m_TextureIndex4;
			m_Size = D3DXVECTOR2(440, 140);
			break;
		case BUS_SCRAPPED_TYPE:
			TextureIndex = m_TextureIndex5;
			m_Size = D3DXVECTOR2(700, 280);
			break;
		case CAR2_TYPE:
			TextureIndex = m_TextureIndex6;
			m_Size = D3DXVECTOR2(440, 140);
			break;
		case CAR3_TYPE:
			TextureIndex = m_TextureIndex7;
			m_Size = D3DXVECTOR2(440, 140);
			break;
		case LIMO_SCRAPPED_TYPE:
			TextureIndex = m_TextureIndex8;
			m_Size = D3DXVECTOR2(700, 150);
			break;
		}

		D3DXVECTOR2 GimmickPos = (*itr).Pos;
		GimmickPos.x -= SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress();

		if (GimmickPos.x < GimmickManager::m_DrawRangeMax &&GimmickPos.x > GimmickManager::m_DrawRangeMin)
		{
			m_pVertex->SetVertex(&m_Size);
			m_pVertex->WriteVertexBuffer();

			m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(TextureIndex));
			m_pVertex->ShaderSetup();
			m_pVertex->WriteConstantBuffer(&GimmickPos, &D3DXVECTOR2(1, 1), (*itr).Angle);
			m_pVertex->Draw();
		}
	}
}

void ObstacleCar::AddGimmick(D3DXVECTOR2 _pos)
{
	m_GimmickData.push_back(GIMMICK_DATA(_pos, CAR_TYPE, 0, false));
}

void ObstacleCar::AddGimmick(D3DXVECTOR2 _pos, int _type)
{
	m_GimmickData.push_back(GIMMICK_DATA(_pos, _type, 0, false));
}

void ObstacleCar::RemoveGimmick()
{
	m_GimmickData.clear();
}

void ObstacleCar::RemoveGimmick(D3DXVECTOR2 _pos)
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		if ((*itr) == GIMMICK_DATA(_pos, CAR_TYPE, 0, false))
		{
			m_GimmickData.erase(itr);
			break;
		}
	}
}

void ObstacleCar::RemoveGimmick(D3DXVECTOR2 _pos, int _type)
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		if ((*itr) == GIMMICK_DATA(_pos, _type, 0, false))
		{
			m_GimmickData.erase(itr);
			break;
		}
	}
}

GimmickManager::GIMMICK_TYPE ObstacleCar::Collision(D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize)
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		D3DXVECTOR2 Size;

		switch ((*itr).GimmickType)
		{
		case CAR_TYPE:
			Size = D3DXVECTOR2(440, 120);
			break;
		case BUS_TYPE:
			Size = D3DXVECTOR2(700, 160);
			break;
		case LIMO_TYPE:
			Size = D3DXVECTOR2(700, 100);
			break;
		case CAR_SCRAPPED_TYPE:
			Size = D3DXVECTOR2(440, 120);
			break;
		case BUS_SCRAPPED_TYPE:
			Size = D3DXVECTOR2(700, 160);
			break;
		case CAR2_TYPE:
			Size = D3DXVECTOR2(440, 120);
			break;
		case CAR3_TYPE:
			Size = D3DXVECTOR2(440, 120);
			break;
		case LIMO_SCRAPPED_TYPE:
			Size = D3DXVECTOR2(700, 100);
			break;
		}


		if ((_pPos->x - _pSize->x / 2) < ((*itr).Pos.x + Size.x / 2) &&
			(_pPos->x + _pSize->x / 2) > ((*itr).Pos.x - Size.x / 2))
		{
			if ((_pPos->y - _pSize->y / 2) < ((*itr).Pos.y + Size.y / 2) &&
				(_pPos->y + _pSize->y / 2) > ((*itr).Pos.y - Size.y / 2))
			{
				return GimmickManager::OBSTACLECAR_GIMMICK;
			}
		}
	}

	return GimmickManager::NONE_GIMMICK;
}

GimmickManager::GIMMICK_TYPE ObstacleCar::Collision(
	D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize,
	D3DXVECTOR2* _pOutPos, D3DXVECTOR2* _pOutSize)
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		D3DXVECTOR2 Size;

		switch ((*itr).GimmickType)
		{
		case CAR_TYPE:
			Size = D3DXVECTOR2(440, 120);
			break;
		case BUS_TYPE:
			Size = D3DXVECTOR2(700, 160);
			break;
		case LIMO_TYPE:
			Size = D3DXVECTOR2(700, 100);
			break;
		case CAR_SCRAPPED_TYPE:
			Size = D3DXVECTOR2(440, 120);
			break;
		case BUS_SCRAPPED_TYPE:
			Size = D3DXVECTOR2(700, 160);
			break;
		case CAR2_TYPE:
			Size = D3DXVECTOR2(440, 120);
			break;
		case CAR3_TYPE:
			Size = D3DXVECTOR2(440, 120);
			break;
		case LIMO_SCRAPPED_TYPE:
			Size = D3DXVECTOR2(700, 100);
			break;
		}


		if ((_pPos->x - _pSize->x / 2) < ((*itr).Pos.x + Size.x / 2) &&
			(_pPos->x + _pSize->x / 2) > ((*itr).Pos.x - Size.x / 2))
		{
			if ((_pPos->y - _pSize->y / 2) < ((*itr).Pos.y + Size.y / 2) &&
				(_pPos->y + _pSize->y / 2) > ((*itr).Pos.y - Size.y / 2))
			{
				*_pOutPos = D3DXVECTOR2((*itr).Pos.x - SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress(), (*itr).Pos.y);
				*_pOutSize = Size;
				return GimmickManager::OBSTACLECAR_GIMMICK;
			}
		}
	}

	return GimmickManager::NONE_GIMMICK;
}

GimmickManager::GIMMICK_TYPE ObstacleCar::DestroyGimmick(D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize)
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		D3DXVECTOR2 Size;

		switch ((*itr).GimmickType)
		{
		case CAR_TYPE:
			Size = D3DXVECTOR2(440, 120);
			break;
		case BUS_TYPE:
			Size = D3DXVECTOR2(700, 160);
			break;
		case LIMO_TYPE:
			Size = D3DXVECTOR2(700, 100);
			break;
		case CAR_SCRAPPED_TYPE:
			Size = D3DXVECTOR2(440, 120);
			break;
		case BUS_SCRAPPED_TYPE:
			Size = D3DXVECTOR2(700, 160);
			break;
		case CAR2_TYPE:
			Size = D3DXVECTOR2(440, 120);
			break;
		case CAR3_TYPE:
			Size = D3DXVECTOR2(440, 120);
			break;
		case LIMO_SCRAPPED_TYPE:
			Size = D3DXVECTOR2(700, 100);
			break;
		}


		if ((_pPos->x - _pSize->x / 2) < ((*itr).Pos.x + Size.x / 2) &&
			(_pPos->x + _pSize->x / 2) > ((*itr).Pos.x - Size.x / 2))
		{
			if ((_pPos->y - _pSize->y / 2) < ((*itr).Pos.y + Size.y / 2) &&
				(_pPos->y + _pSize->y / 2) > ((*itr).Pos.y - Size.y / 2))
			{
				if ((*itr).IsDestroy == false)
				{
					(*itr).IsDestroy = true;
					return GimmickManager::OBSTACLECAR_GIMMICK;
				}
			}
		}
	}


	return GimmickManager::NONE_GIMMICK;
}
