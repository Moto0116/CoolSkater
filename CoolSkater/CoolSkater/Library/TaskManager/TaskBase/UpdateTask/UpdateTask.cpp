/**
 * @file	UpdateTask.cpp
 * @brief	更新のタスククラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "UpdateTask.h"

#include <Windows.h>

#include "..\..\..\ObjectManagerBase\ObjectBase\ObjectBase.h"


namespace Lib
{
	//----------------------------------------------------------------------
	// Constructor	Destructor
	//----------------------------------------------------------------------
	UpdateTask::UpdateTask() :
		m_pObject(nullptr)
	{
	}

	UpdateTask::~UpdateTask()
	{
	}


	//----------------------------------------------------------------------
	// Public Functions
	//----------------------------------------------------------------------
	void UpdateTask::Run()
	{
		m_pObject->Update();
	}

	void UpdateTask::SetUpdateObject(ObjectBase* _pObject)
	{
		m_pObject = _pObject;
	}
}

