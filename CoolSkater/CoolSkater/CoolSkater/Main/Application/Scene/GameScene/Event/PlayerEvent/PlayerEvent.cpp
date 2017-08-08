/**
 * @file	PlayerEvent.cpp
 * @brief	プレイヤーイベントクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "PlayerEvent.h"

#include "Scene\GameScene\GameScene.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
PlayerEvent::PlayerEvent() :
	Lib::EventBase(GameScene::PLAYER_EVENT),
	m_Type(PLAYERDEADEND_EVENT)
{
}

PlayerEvent::~PlayerEvent()
{
}
