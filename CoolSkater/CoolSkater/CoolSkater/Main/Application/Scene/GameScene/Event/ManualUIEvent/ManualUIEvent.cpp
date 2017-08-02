/**
 * @file	ManualUIEvent.cpp
 * @brief	マニュアルUIイベントクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "ManualUIEvent.h"

#include "Scene\GameScene\GameScene.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
ManualUIEvent::ManualUIEvent() : 
	Lib::EventBase(GameScene::GAMESTART_EVENT)
{
}

ManualUIEvent::~ManualUIEvent()
{
}
