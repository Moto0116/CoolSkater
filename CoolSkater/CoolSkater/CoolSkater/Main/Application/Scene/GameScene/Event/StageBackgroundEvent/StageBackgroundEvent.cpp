/**
 * @file	StageBackgroundEvent.cpp
 * @brief	ステージ背景イベントクラス実装
 * @author	morimoto
 */

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "StageBackgroundEvent.h"

#include "Scene\GameScene\GameScene.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
StageBackgroundEvent::StageBackgroundEvent() :
	Lib::EventBase(GameScene::GAMEUPDATE_EVENT)
{
}

StageBackgroundEvent::~StageBackgroundEvent()
{
}
