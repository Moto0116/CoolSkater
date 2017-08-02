/**
 * @file	EventManager.cpp
 * @brief	イベント管理基底クラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "EventManager\EventManager.h"

#include "EventListenerBase\EventListenerBase.h"


namespace Lib
{
	//----------------------------------------------------------------------
	// Constructor	Destructor
	//----------------------------------------------------------------------
	EventManager::EventManager()
	{
	}

	EventManager::~EventManager()
	{
	}


	//----------------------------------------------------------------------
	// Public Functions
	//----------------------------------------------------------------------
	void EventManager::AddEventListener(EventListenerBase* _pEventListener)
	{
		m_pEventListener.push_back(_pEventListener);
	}

	void EventManager::RemoveEventListener(EventListenerBase* _pEventListener)
	{
		for (auto itr = m_pEventListener.begin(); itr != m_pEventListener.end(); itr++)
		{
			if ((*itr)->GetEventListenerID() == _pEventListener->GetEventListenerID())
			{
				m_pEventListener.erase(itr);	// IDが一致していればリストから削除.
				break;
			}
		}
	}

	void EventManager::SendEventMessage(EventBase* _pEvent)
	{
		for (auto itr = m_pEventListener.begin(); itr != m_pEventListener.end(); itr++)
		{
			(*itr)->EventMessage(_pEvent);
		}
	}
}

