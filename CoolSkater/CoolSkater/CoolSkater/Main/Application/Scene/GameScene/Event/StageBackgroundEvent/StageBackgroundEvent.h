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
	enum EVENT_TYPE
	{
		GAMEUPDATE_EVENT,	// ゲーム開始の通知.
		STAGECHANGE_EVENT1,	// ステージの切り替え通知.
		STAGECHANGE_EVENT2,	// ステージの切り替え通知.
		STAGECHANGE_EVENT3,	// ステージの切り替え通知.
		EVENT_TYPE_MAX
	};

	StageBackgroundEvent();
	virtual ~StageBackgroundEvent();

	EVENT_TYPE GetEvent(){ return m_Type; }
	void SetEvent(EVENT_TYPE _type){ m_Type = _type; }

private:
	EVENT_TYPE m_Type;

};

#endif // !STAGEBACKGROUNDEVENT_H
