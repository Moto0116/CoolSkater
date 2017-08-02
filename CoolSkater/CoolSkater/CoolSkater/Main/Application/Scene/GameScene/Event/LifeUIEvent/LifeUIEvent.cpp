/**
 * @file	LifeUIEvent.cpp
 * @brief	ライフUIイベントクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "LifeUIEvent.h"

#include "Scene\GameScene\GameScene.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
LifeUIEvent::LifeUIEvent() :
	Lib::EventBase(GameScene::PLAYERDEAD_EVENT)
{
}


LifeUIEvent::~LifeUIEvent()
{
}
