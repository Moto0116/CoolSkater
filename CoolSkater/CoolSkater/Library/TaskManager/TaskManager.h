/**
 * @file	TaskManager.h
 * @brief	タスク管理の基底クラス定義
 * @author	morimoto
 */
#ifndef LIB_TASKMANAGER_H
#define LIB_TASKMANAGER_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include <list>

#include "TaskBase\TaskBase.h"
#include "..\SingletonBase\SingletonBase.h"


namespace Lib
{
	/**
	 * タスク管理の基底クラス
	 * @tparam Type 管理するタスクオブジェクト
	 * @tparam BeginTask 最初に実行するタスクオブジェクト
	 */
	template <typename Type, typename BeginTask = TaskBase<>>
	class TaskManager : public SingletonBase<TaskManager<Type, BeginTask>>
	{
	public:
		friend SingletonBase<TaskManager<Type, BeginTask>>;
	
		/**
		 * タスクの実行
		 */
		inline void Run();
	
		/**
		 * タスクの追加
		 * @param[in] _pTask 追加するタスク
		 */
		inline void AddTask(Type* _pTask);

		/**
		 * タスクの削除
		 * @param[in] _pTask 削除するタスク
		 */
		inline void RemoveTask(Type* _pTask);
	
		/**
		 * 開始タスクの追加
		 * @param[in] _pBeginTask 追加するタスク
		 */
		inline void AddBeginTask(BeginTask* _pBeginTask);

		/**
		 * 開始タスクの削除
		 * @param[in] _pBeginTask 削除するタスク
		 */
		inline void RemoveBeginTask(BeginTask* _pBeginTask);

	private:
		/**
		 * コンストラクタ
		 */
		inline TaskManager();
	
		/**
		 * デストラクタ
		 */
		inline virtual ~TaskManager();


		std::list<BeginTask*>	m_pBeginTaskList;	//!< 開始タスクオブジェクトを管理するコンテナ.
		std::list<Type*>		m_pTaskList;		//!< タスクオブジェクトを管理するコンテナ.

	};
}


#include "TaskManager_private.inl"


#endif // !LIB_TASKMANAGER_H
