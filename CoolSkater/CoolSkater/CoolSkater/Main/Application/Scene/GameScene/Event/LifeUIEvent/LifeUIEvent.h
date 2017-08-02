/**
 * @file	LifeUIEvent.h
 * @brief	ライフUIイベントクラス定義
 * @author	morimoto
 */
#ifndef LIFEUIEVENT_H
#define LIFEUIEVENT_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "EventManager\EventBase\EventBase.h"


class LifeUIEvent : public Lib::EventBase
{
public:
	/**
	 * コンストラクタ
	 */
	LifeUIEvent();

	/**
	 * デストラクタ
	 */
	virtual ~LifeUIEvent();

};


#endif // !LIFEUIEVENT_H
