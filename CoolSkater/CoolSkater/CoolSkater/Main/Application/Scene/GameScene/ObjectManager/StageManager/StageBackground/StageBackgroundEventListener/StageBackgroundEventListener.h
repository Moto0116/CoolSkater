/**
 * @file	StageBackgroundEventListener.h
 * @brief	ステージ背景イベントリスナクラス
 * @author	morimoto
 */
#ifndef STAGEBACKGROUNDEVENTLISTENER_H
#define STAGEBACKGROUNDEVENTLISTENER_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "EventManager\EventListenerBase\EventListenerBase.h"
#include <vector>

class StageBackground;

class StageBackgroundEventListener : public Lib::EventListenerBase
{
public:
	/**
	 * コンストラクタ
	 */
	StageBackgroundEventListener(
		StageBackground* _pStageBackground,
		void(StageBackground::*_pFunc)(Lib::EventBase*));

	/**
	 * デストラクタ
	 */
	virtual ~StageBackgroundEventListener();

	/**
	 * イベントメッセージを受け取る関数
	 * @param[in] _pEvent 受け取るイベント
	 */
	virtual void EventMessage(Lib::EventBase* _pEvent);

private:
	StageBackground* m_pStageBackground;
	void(StageBackground::*m_pFunc)(Lib::EventBase*);

};


#endif // !STAGEBACKGROUNDEVENTLISTENER_H
