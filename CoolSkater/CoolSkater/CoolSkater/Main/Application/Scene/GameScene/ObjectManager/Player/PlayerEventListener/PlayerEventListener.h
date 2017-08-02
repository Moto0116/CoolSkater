/**
 * @file	PlayerEventListener.h
 * @brief	プレイヤーイベントリスナクラス実装
 * @author	morimoto
 */
#ifndef PLAYEREVENTLISTENER_H
#define PLAYEREVENTLISTENER_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "EventManager\EventListenerBase\EventListenerBase.h"

class Player;

class PlayerEventListener : public Lib::EventListenerBase
{
public:
	/**
	 * コンストラクタ
	 */
	PlayerEventListener(Player* _pPlayer, void(Player::*_pFunc)(Lib::EventBase*));
	
	/**
	 * デストラクタ
	 */
	virtual ~PlayerEventListener();

	/**
	 * イベントメッセージを受け取る関数
	 * @param[in] _pEvent 受け取るイベント
	 */
	virtual void EventMessage(Lib::EventBase* _pEvent);

private:
	Player* m_pPlayer;
	void(Player::*m_pFunc)(Lib::EventBase*);

};


#endif // !PLAYEREVENTLISTENER_H
