/**
 * @file	EventListenerBase.h
 * @brief	イベント受信クラス定義
 * @author	morimoto
 */
#ifndef LIB_EVENTLISTENERBASE_H
#define LIB_EVENTLISTENERBASE_H


namespace Lib
{
	class EventBase;

	/**
	 * イベント受信クラス
	 */
	class EventListenerBase
	{
	public:
		/**
		 * コンストラクタ
		 */
		EventListenerBase();

		/**
		 * デストラクタ
		 */
		virtual ~EventListenerBase();

		/**
		 * イベントメッセージを受け取る関数
		 * @param[in] _pEvent 受け取るイベント
		 */
		virtual void EventMessage(EventBase* _pEvent);

		/**
		 * EventListenerBaseのIDを取得する関数
		 * @return EventListenerBaseのID
		 */
		inline unsigned int GetEventListenerID() const
		{
			return m_EventListenerID;
		}

	private:
		static unsigned int			m_EventListenerCount;	//!< イベントリスナオブジェクトのカウント変数.
		unsigned int				m_EventListenerID;		//!< イベントリスナのユニークID.

	};
}



#endif // !LIB_EVENTLISTENERBASE_H
