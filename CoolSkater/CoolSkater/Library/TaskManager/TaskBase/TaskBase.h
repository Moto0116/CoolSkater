/**
 * @file	TaskBase.h
 * @brief	タスクの基底クラス定義
 * @author	morimoto
 */
#ifndef LIB_TASKBASE_H
#define LIB_TASKBASE_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "..\..\Define\Define.h"


namespace Lib
{
	/**
	 * タスクの基底クラス
	 * @tparam PriorityType タスク実行の優先度を判断するオブジェクト
	 */
	template<typename PriorityType = int>
	class TaskBase
	{
	public:
		/**
		 * コンストラクタ
		 */
		TaskBase();
	
		/**
		 * デストラクタ
		 */
		virtual ~TaskBase();
	
		/**
		 * タスクの実行
		 */
		virtual void Run();
	
		/**
		 * タスク実行の優先度設定
		 * @param[in] _priority タスク優先度
		 */
		void SetPriority(PriorityType _priority)
		{
			m_Priority = _priority;
		}
	
		/**
		 * タスクのIDを取得する
		 * @return タスクのID
		 */
		unsigned int GetID()
		{
			return m_TaskID;
		}

		/**
		 * タスクの比較用ファンクタ
		 */
		class TaskCmp
		{
		public:
			bool operator()(TaskBase<PriorityType>* _pTask1, TaskBase<PriorityType>* _pTask2) const
			{
				return (_pTask1->m_Priority < _pTask2->m_Priority);
			}

			bool operator()(const TaskBase<PriorityType>& _task1, TaskBase<PriorityType>& _task2) const
			{
				return (_task1.m_Priority < _task2.m_Priority);
			}
		};
	
	protected:
		static unsigned int m_TaskNum;	//!< タスクの作成数.
		unsigned int		m_TaskID;	//!< タスクのID.
		PriorityType		m_Priority;	//!< タスク実行の優先度.

	private:
		DISALLOW_COPY_AND_ASSIGN(TaskBase);

	};
}


#include "TaskBase_private.inl"


#endif // !LIB_TASKBASE_H
