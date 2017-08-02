/**
 * @file	PlayerEventListener.cpp
 * @brief	プレイヤーイベントリスナクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "PlayerEventListener.h"

#include "Scene\GameScene\GameScene.h"
#include "EventManager\EventBase\EventBase.h"
#include "Scene\GameScene\GameScene.h"
#include "..\Player.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
PlayerEventListener::PlayerEventListener(Player* _pPlayer, void(Player::*_pFunc)(Lib::EventBase*)) :
	m_pPlayer(_pPlayer),
	m_pFunc(_pFunc)
{
}

PlayerEventListener::~PlayerEventListener()
{
}


//----------------------------------------------------------------------
// Public Functions
//----------------------------------------------------------------------
void PlayerEventListener::EventMessage(Lib::EventBase* _pEvent)
{
	(m_pPlayer->*m_pFunc)(_pEvent);
}

