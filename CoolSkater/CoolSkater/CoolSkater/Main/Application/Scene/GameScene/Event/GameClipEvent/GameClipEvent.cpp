/**
 * @file	GameClipEvent.cpp
 * @brief	ゲームクリップイベントクラス実装
 * @author	morimoto
 */


//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "GameClipEvent.h"

#include "Scene\GameScene\GameScene.h"


//----------------------------------------------------------------------
// Constructor	Destructor
//----------------------------------------------------------------------
GameClipEvent::GameClipEvent() : 
	Lib::EventBase(GameScene::CLIP_EVENT)
{
}

GameClipEvent::~GameClipEvent()
{
}
