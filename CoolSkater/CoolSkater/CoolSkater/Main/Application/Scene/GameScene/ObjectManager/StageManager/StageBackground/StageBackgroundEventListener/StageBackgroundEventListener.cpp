/**
 * @file	StageBackgroundEventListener.cpp
 * @brief	ステージ背景イベントリスナクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "StageBackgroundEventListener.h"

#include "Scene\GameScene\GameScene.h"
#include "EventManager\EventBase\EventBase.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
StageBackgroundEventListener::StageBackgroundEventListener(
	StageBackground* _pStageBackground,
	void(StageBackground::*_pFunc)(Lib::EventBase*)) : 
	m_pStageBackground(_pStageBackground),
	m_pFunc(_pFunc)
{
}

StageBackgroundEventListener::~StageBackgroundEventListener()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
void StageBackgroundEventListener::EventMessage(Lib::EventBase* _pEvent)
{
	(m_pStageBackground->*m_pFunc)(_pEvent);
}
