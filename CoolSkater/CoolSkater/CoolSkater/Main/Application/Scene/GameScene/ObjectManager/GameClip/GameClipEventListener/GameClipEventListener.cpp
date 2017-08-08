/**
 * @file	GameClipEventListener.cpp
 * @brief	ゲームクリップイベントリスナクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "GameClipEventListener.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
GameClipEventListener::GameClipEventListener(GameClip* _pGameClip, void(GameClip::*_pFunc)(Lib::EventBase*)) :
	m_pGameClip(_pGameClip),
	m_pFunc(_pFunc)
{
}

GameClipEventListener::~GameClipEventListener()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
void GameClipEventListener::EventMessage(Lib::EventBase* _pEvent)
{
	(m_pGameClip->*m_pFunc)(_pEvent);
}

