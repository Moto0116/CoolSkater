/**
 * @file	GameSceneEventListener.cpp
 * @brief	ゲームシーンのイベントリスナ実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "GameSceneEventListener.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
GameSceneEventListener::GameSceneEventListener(GameScene* _pGameScene, void(GameScene::*_pFunc)(Lib::EventBase*)) : 
	m_pGameScene(_pGameScene),
	m_pFunc(_pFunc)
{
}

GameSceneEventListener::~GameSceneEventListener()
{
}

//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
void GameSceneEventListener::EventMessage(Lib::EventBase* _pEvent)
{
	(m_pGameScene->*m_pFunc)(_pEvent);
}
