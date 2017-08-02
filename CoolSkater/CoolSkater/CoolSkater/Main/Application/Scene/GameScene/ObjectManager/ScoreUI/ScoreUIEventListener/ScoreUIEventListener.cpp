/**
 * @file	ScoreUIEventListener.cpp
 * @brief	スコアUIイベントリスナクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "ScoreUIEventListener.h"


ScoreUIEventListener::ScoreUIEventListener(ScoreUI* _pScoreUI, void(ScoreUI::*_pFunc)(Lib::EventBase*)) : 
	m_pScoreUI(_pScoreUI),
	m_pFunc(_pFunc)
{
}

ScoreUIEventListener::~ScoreUIEventListener()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
void ScoreUIEventListener::EventMessage(Lib::EventBase* _pEvent)
{
	(m_pScoreUI->*m_pFunc)(_pEvent);
}
