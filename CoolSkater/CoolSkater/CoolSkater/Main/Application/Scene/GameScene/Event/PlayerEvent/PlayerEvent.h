#ifndef PLAYEREVENT_H
#define PLAYEREVENT_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "EventManager\EventBase\EventBase.h"


class PlayerEvent : public Lib::EventBase
{
public:
	enum EVENT_TYPE
	{
		PLAYERCLEAR_EVENT,
		PLAYERDEADEND_EVENT	// プレイヤーが倒れてゲーム終了の通知.
	};

	PlayerEvent();
	virtual ~PlayerEvent();

	EVENT_TYPE GetEvent(){ return m_Type; }
	void SetEvent(EVENT_TYPE _type){ m_Type = _type; }

private:
	EVENT_TYPE m_Type;

};


#endif // !PLAYEREVENT_H
