/**
 * @file	GameSceneEventListener.h
 * @brief	ゲームシーンイベントリスナ定義
 * @author	morimoto
 */
#ifndef GAMESCENEEVENTLISTENER_H
#define GAMESCENEEVENTLISTENER_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "EventManager\EventListenerBase\EventListenerBase.h"

class GameScene;

class GameSceneEventListener : public Lib::EventListenerBase
{
public:
	/**
	 * コンストラクタ
	 */
	GameSceneEventListener(GameScene* _pGameScene, void(GameScene::*_pFunc)(Lib::EventBase*));

	/**
	 * デストラクタ
	 */
	virtual ~GameSceneEventListener();

	/**
	 * イベントメッセージを受け取る関数
	 * @param[in] _pEvent 受け取るイベント
	 */
	virtual void EventMessage(Lib::EventBase* _pEvent);

private:
	GameScene* m_pGameScene;
	void(GameScene::*m_pFunc)(Lib::EventBase*);

};


#endif // !GAMESCENEEVENTLISTENER_H
