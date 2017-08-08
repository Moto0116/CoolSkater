/**
 * @file	GimmickBase.cpp
 * @brief	ギミック基底クラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "GimmickBase.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
GimmickBase::GimmickBase()
{
}

GimmickBase::~GimmickBase()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
bool GimmickBase::Initialize()
{
	return true;
}

void GimmickBase::Finalize()
{
}

void GimmickBase::Update()
{
}

void GimmickBase::Draw()
{
}

void GimmickBase::AddGimmick(D3DXVECTOR2 _pos)
{
}

void GimmickBase::AddGimmick(D3DXVECTOR2 _pos, int _type)
{
}

void GimmickBase::RemoveGimmick()
{
}

void GimmickBase::RemoveGimmick(D3DXVECTOR2 _pos)
{
}

void GimmickBase::RemoveGimmick(D3DXVECTOR2 _pos, int _type)
{
}

GimmickManager::GIMMICK_TYPE GimmickBase::Collision(D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize)
{
	return GimmickManager::NONE_GIMMICK;
}

GimmickManager::GIMMICK_TYPE GimmickBase::Collision(
	D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize,
	D3DXVECTOR2* _pOutPos, D3DXVECTOR2* _pOutSize)
{
	return GimmickManager::NONE_GIMMICK;
}

GimmickManager::GIMMICK_TYPE GimmickBase::DestroyGimmick(D3DXVECTOR2* _pPos, D3DXVECTOR2* _pSize)
{
	return GimmickManager::NONE_GIMMICK;
}
