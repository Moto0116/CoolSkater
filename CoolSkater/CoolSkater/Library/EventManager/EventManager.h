/**
 * @file	EventManager.h
 * @brief	イベント管理基底クラス定義
 * @author	morimoto
 */
#ifndef LIB_EVENTMANAGER_H
#define LIB_EVENTMANAGER_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include <vector>

#include "..\SingletonBase\SingletonBase.h"


namespace Lib
{
	class EventBase;
	class EventListenerBase;

	/**
	 * イベント管理基底クラス
	 * @tparam Event 通知を受けるEventクラス
	 */
	class EventManager : public SingletonBase<EventManager>
	{
	public:
		friend SingletonBase<EventManager>;

		/**
		 * EventListenerを追加する
		 * @param[in] _pEventListener 追加するEventListener
		 */
		void AddEventListener(EventListenerBase* _pEventListener);

		/**
		 * EventListenerを削除する
		 * @param[in] _pEventListener 削除するEventListener
		 */
		void RemoveEventListener(EventListenerBase* _pEventListener);

		/**
		 * イベントを通知する
		 * @param[in] _pEvent 通知するイベント
		 */
		void SendEventMessage(EventBase* _pEvent);

	private:
		/**
		 * コンストラクタ
		 */
		EventManager();

		/**
		 * デストラクタ
		 */
		virtual ~EventManager();


		std::vector<EventListenerBase*> m_pEventListener;	//!< イベントリスナを格納するコンテナ.

	};
}


#endif // !LIB_EVENTMANAGER_H
