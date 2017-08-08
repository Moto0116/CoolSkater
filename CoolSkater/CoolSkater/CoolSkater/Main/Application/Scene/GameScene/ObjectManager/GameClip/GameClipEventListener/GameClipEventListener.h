/**
 * @file	GameClipEventListener.h
 * @brief	ゲームクリップイベントリスナクラス定義
 * @author	morimoto
 */
#ifndef GAMECLIPEVENTLISTENER_H
#define GAMECLIPEVENTLISTENER_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "EventManager\EventListenerBase\EventListenerBase.h"

class GameClip;

class GameClipEventListener : public Lib::EventListenerBase
{
public:
	/**
	 * コンストラクタ
	 */
	GameClipEventListener(GameClip* _pGameClip, void(GameClip::*_pFunc)(Lib::EventBase*));

	/**
	 * デストラクタ
	 */
	virtual ~GameClipEventListener();

	/**
	 * イベントメッセージを受け取る関数
	 * @param[in] _pEvent 受け取るイベント
	 */
	virtual void EventMessage(Lib::EventBase* _pEvent);

private:
	GameClip* m_pGameClip;
	void(GameClip::*m_pFunc)(Lib::EventBase*);

};


#endif // !GAMECLIPEVENTLISTENER_H
