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

void GimmickBase::AddGimmick(int _pos)
{
}

void GimmickBase::RemoveGimmick(int _pos)
{
}
