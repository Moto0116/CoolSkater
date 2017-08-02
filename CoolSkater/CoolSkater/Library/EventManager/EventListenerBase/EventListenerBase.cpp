/**
 * @file	EventListenerBase.cpp
 * @brief	イベント受信クラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "EventListenerBase.h"


namespace Lib
{
	//----------------------------------------------------------------------
	// Static Private Variables
	//----------------------------------------------------------------------
	unsigned int EventListenerBase::m_EventListenerCount = 0;


	//----------------------------------------------------------------------
	// Constructor	Destructor
	//----------------------------------------------------------------------
	EventListenerBase::EventListenerBase()
	{
		m_EventListenerCount++;	// オーバーフローは考慮しない.
		m_EventListenerID = m_EventListenerCount;
	}

	EventListenerBase::~EventListenerBase()
	{
	}


	//----------------------------------------------------------------------
	// Public Functions
	//----------------------------------------------------------------------
	void EventListenerBase::EventMessage(EventBase* _pEvent)
	{
	}
}
