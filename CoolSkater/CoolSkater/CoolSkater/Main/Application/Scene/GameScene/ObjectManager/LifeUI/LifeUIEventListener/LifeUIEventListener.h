/**
 * @file	LifeUIEventListener.h
 * @brief	ライフUIイベントリスナ定義
 * @author	morimoto
 */
#ifndef LIFEUIEVENTLISTENER_H
#define LIFEUIEVENTLISTENER_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "EventManager\EventListenerBase\EventListenerBase.h"

class LifeUI;

class LifeUIEventListener : public Lib::EventListenerBase
{
public:
	/**
	 * コンストラクタ
	 */
	LifeUIEventListener(LifeUI* _pLifeUI, void(LifeUI::*_pFunc)(Lib::EventBase*));

	/** 
	 * デストラクタ
	 */
	virtual ~LifeUIEventListener();

	/**
	 * イベントメッセージを受け取る関数
	 * @param[in] _pEvent 受け取るイベント
	 */
	virtual void EventMessage(Lib::EventBase* _pEvent);

private:
	LifeUI* m_pLifeUI;
	void(LifeUI::*m_pFunc)(Lib::EventBase*);

};


#endif // !LIFEUIEVENTLISTENER_H
