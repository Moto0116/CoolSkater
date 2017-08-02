/**
 * @file	ManualEvent.h
 * @brief	マニュアルUIイベントクラス定義
 * @author	morimoto
 */
#ifndef MANUALUIEVENT_H
#define MANUALUIEVENT_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "EventManager\EventBase\EventBase.h"


class ManualUIEvent : public Lib::EventBase
{
public:
	/**
	 * コンストラクタ
	 */
	ManualUIEvent();

	/**
	 * デストラクタ
	 */
	virtual ~ManualUIEvent();

};


#endif // !MANUALUIEVENT_H
