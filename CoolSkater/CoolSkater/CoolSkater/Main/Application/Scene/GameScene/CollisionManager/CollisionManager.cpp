/**
 * @file	CollisionManager.cpp
 * @brief	当たり判定管理クラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "CollisionManager.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
void CollisionManager::AddGimmick(GimmickBase* _pGimmick)
{
	m_pGimmicks.push_back(_pGimmick);
}

void CollisionManager::RemoveGimmicks()
{
	m_pGimmicks.clear();
}

GimmickManager::GIMMICK_TYPE CollisionManager::GimmickCollision(D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize)
{
	for (auto itr = m_pGimmicks.begin(); itr != m_pGimmicks.end(); itr++)
	{
		GimmickManager::GIMMICK_TYPE Type = (*itr)->Collision(_pPos, _pSize);
		if (Type != GimmickManager::NONE_GIMMICK)
		{
			return Type;
		}
	}

	return GimmickManager::NONE_GIMMICK;
}

GimmickManager::GIMMICK_TYPE CollisionManager::GimmickCollision(
	D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize,
	D3DXVECTOR2* _pOutPos, D3DXVECTOR2* _pOutSize)
{
	for (auto itr = m_pGimmicks.begin(); itr != m_pGimmicks.end(); itr++)
	{
		GimmickManager::GIMMICK_TYPE Type = (*itr)->Collision(_pPos, _pSize, _pOutPos, _pOutSize);
		if (Type != GimmickManager::NONE_GIMMICK)
		{
			return Type;
		}
	}

	return GimmickManager::NONE_GIMMICK;
}

GimmickManager::GIMMICK_TYPE CollisionManager::DestroyGimmick(D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize)
{
	for (auto itr = m_pGimmicks.begin(); itr != m_pGimmicks.end(); itr++)
	{
		GimmickManager::GIMMICK_TYPE Type = (*itr)->DestroyGimmick(_pPos, _pSize);
		if (Type != GimmickManager::NONE_GIMMICK)
		{
			return Type;
		}
	}

	return GimmickManager::NONE_GIMMICK;
}

