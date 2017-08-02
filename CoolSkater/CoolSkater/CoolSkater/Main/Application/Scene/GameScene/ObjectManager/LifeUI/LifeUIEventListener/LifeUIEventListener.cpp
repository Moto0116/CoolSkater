/**
 * @file	LifeUIEventListener.cpp
 * @brief	ライフUIイベントリスナ実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "LifeUIEventListener.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
LifeUIEventListener::LifeUIEventListener(LifeUI* _pLifeUI, void(LifeUI::*_pFunc)(Lib::EventBase*)) : 
	m_pLifeUI(_pLifeUI),
	m_pFunc(_pFunc)
{
}

LifeUIEventListener::~LifeUIEventListener()
{
}

//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
void LifeUIEventListener::EventMessage(Lib::EventBase* _pEvent)
{
	(m_pLifeUI->*m_pFunc)(_pEvent);
}
