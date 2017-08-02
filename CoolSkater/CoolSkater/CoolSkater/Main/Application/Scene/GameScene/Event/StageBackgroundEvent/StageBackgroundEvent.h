/**
 * @file	StageBackgroundEvent.h
 * @brief	ステージ背景イベントクラス定義
 * @author	morimoto
 */
#ifndef STAGEBACKGROUNDEVENT_H
#define STAGEBACKGROUNDEVENT_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "EventManager\EventBase\EventBase.h"


class StageBackgroundEvent : public Lib::EventBase
{
public:
	StageBackgroundEvent();
	virtual ~StageBackgroundEvent();

};

#endif // !STAGEBACKGROUNDEVENT_H
