/**
 * @file	JumpRamp.cpp
 * @brief	ジャンプ台クラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "JumpRamp.h"

#include "DirectX11\TextureManager\TextureManager.h"
#include "Scene\GameScene\GameDataManager\GameDataManager.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
JumpRamp::JumpRamp()
{
	m_Pos = D3DXVECTOR2(0, 0);
	m_Size = D3DXVECTOR2(152, 152);
}

JumpRamp::~JumpRamp()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool JumpRamp::Initialize()
{
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->AddTask(m_pDrawTask);
	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->AddTask(m_pUpdateTask);

	if (!CreateVertex2D())
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\JumpRamp.png",
		&m_TextureIndex))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\JumpRamp2.png",
		&m_TextureIndex2))
	{
		return false;
	}

	if (!SINGLETON_INSTANCE(Lib::TextureManager)->LoadTexture(
		"Resource\\GameScene\\JumpRamp3.png",
		&m_TextureIndex3))
	{
		return false;
	}

	return true;
}

void JumpRamp::Finalize()
{
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex3);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex2);
	SINGLETON_INSTANCE(Lib::TextureManager)->ReleaseTexture(m_TextureIndex);
	ReleaseVertex2D();

	SINGLETON_INSTANCE(Lib::UpdateTaskManager)->RemoveTask(m_pUpdateTask);
	SINGLETON_INSTANCE(Lib::DrawTaskManager)->RemoveTask(m_pDrawTask);
}

void JumpRamp::Update()
{
}

void JumpRamp::Draw()
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		D3DXVECTOR2 GimmickPos = (*itr).Pos;
		GimmickPos.x -= SINGLETON_INSTANCE(GameDataManager)->GetPlayerProgress();

		switch ((*itr).GimmickType)
		{
		case JUMPRAMP_TYPE:
			m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex));
			break;
		case JUMPRAMP2_TYPE:
			m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex2));
			break;
		case JUMPRAMP3_TYPE:
			m_pVertex->SetTexture(SINGLETON_INSTANCE(Lib::TextureManager)->GetTexture(m_TextureIndex3));
			break;
		}

		if (GimmickPos.x < GimmickManager::m_DrawRangeMax &&GimmickPos.x > GimmickManager::m_DrawRangeMin)
		{
			m_pVertex->ShaderSetup();
			m_pVertex->WriteConstantBuffer(&GimmickPos);
			m_pVertex->Draw();
		}
	}
}

void JumpRamp::AddGimmick(D3DXVECTOR2 _pos)
{
	m_GimmickData.push_back(GIMMICK_DATA(_pos, JUMPRAMP_TYPE));
}

void JumpRamp::AddGimmick(D3DXVECTOR2 _pos, int _type)
{
	m_GimmickData.push_back(GIMMICK_DATA(_pos, _type));
}

void JumpRamp::RemoveGimmick()
{
	m_GimmickData.clear();
}

void JumpRamp::RemoveGimmick(D3DXVECTOR2 _pos)
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		if ((*itr) == GIMMICK_DATA(_pos, JUMPRAMP_TYPE))
		{
			m_GimmickData.erase(itr);
			break;
		}
	}
}

void JumpRamp::RemoveGimmick(D3DXVECTOR2 _pos, int _type)
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

GimmickManager::GIMMICK_TYPE JumpRamp::Collision(D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize)
{
	for (auto itr = m_GimmickData.begin(); itr != m_GimmickData.end(); itr++)
	{
		if ((_pPos->x - _pSize->x / 2) < ((*itr).Pos.x + m_Size.x / 2) &&
			(_pPos->x + _pSize->x / 2) > ((*itr).Pos.x - m_Size.x / 2))
		{
			if ((_pPos->y - _pSize->y / 2) < ((*itr).Pos.y + m_Size.y / 2) &&
				(_pPos->y + _pSize->y / 2) > ((*itr).Pos.y - m_Size.y / 2))
			{
				return GimmickManager::JUMPRAMP_GIMMICK;
			}
		}
	}

	return GimmickManager::NONE_GIMMICK;
}

GimmickManager::GIMMICK_TYPE JumpRamp::Collision(
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
				return GimmickManager::JUMPRAMP_GIMMICK;
			}
		}
	}

	return GimmickManager::NONE_GIMMICK;
}

