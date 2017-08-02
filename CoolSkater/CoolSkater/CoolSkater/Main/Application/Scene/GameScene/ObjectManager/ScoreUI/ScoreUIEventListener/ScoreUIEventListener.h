/**
 * @file	ScoreUIEventListener.h
 * @brief	スコアUIイベントリスナクラス定義
 * @author	morimoto
 */
#ifndef SCOREUIEVENTLISTENER_H
#define SCOREUIEVENTLISTENER_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "EventManager\EventListenerBase\EventListenerBase.h"

class ScoreUI;

class ScoreUIEventListener : public Lib::EventListenerBase
{
public:
	/**
	 * コンストラクタ
	 */
	ScoreUIEventListener(ScoreUI* _pScoreUI, void(ScoreUI::*_pFunc)(Lib::EventBase*));

	/**
	 * デストラクタ
	 */
	virtual ~ScoreUIEventListener();

	/**
	 * イベントメッセージを受け取る関数
	 * @param[in] _pEvent 受け取るイベント
	 */
	virtual void EventMessage(Lib::EventBase* _pEvent);

private:
	ScoreUI* m_pScoreUI;
	void(ScoreUI::*m_pFunc)(Lib::EventBase*);

};


#endif // !SCOREUIEVENTLISTENER_H
