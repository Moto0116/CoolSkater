#ifndef GAMECLIPEVENT_H
#define GAMECLIPEVENT_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "EventManager\EventBase\EventBase.h"


class GameClipEvent : public Lib::EventBase
{
public:
	/**
	 * コンストラクタ
	 */
	GameClipEvent();

	/**
	 * デストラクタ
	 */
	virtual ~GameClipEvent();

};


#endif // !GAMECLIPEVENT_H
