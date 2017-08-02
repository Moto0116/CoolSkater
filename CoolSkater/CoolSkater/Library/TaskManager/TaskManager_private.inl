/**
 * @file	TaskManager_private.inl
 * @brief	タスク管理の基底クラステンプレート実装
 * @author	morimoto
 */
#ifndef LIB_TASKMANAGER_PRIVATE_H
#define LIB_TASKMANAGER_PRIVATE_H


namespace Lib
{
	//----------------------------------------------------------------------
	// Constructor	Destructor
	//----------------------------------------------------------------------
	template <typename Type, typename BeginTask>
	inline TaskManager<Type, BeginTask>::TaskManager()
	{
	}

	template <typename Type, typename BeginTask>
	inline TaskManager<Type, BeginTask>::~TaskManager()
	{
	}


	//----------------------------------------------------------------------
	// Public Functions
	//----------------------------------------------------------------------
	template <typename Type, typename BeginTask>
	inline void TaskManager<Type, BeginTask>::Run()
	{
		for (auto itr = m_pBeginTaskList.begin(); itr != m_pBeginTaskList.end(); itr++)
		{
			(*itr)->Run();
		}

		for (auto itr = m_pTaskList.begin(); itr != m_pTaskList.end(); itr++)
		{
			(*itr)->Run();
		}
	}

	template <typename Type, typename BeginTask>
	inline void TaskManager<Type, BeginTask>::AddTask(Type* _pTask)
	{
		m_pTaskList.push_back(_pTask);
		m_pTaskList.sort(Type::TaskCmp());
	}

	template <typename Type, typename BeginTask>
	inline void TaskManager<Type, BeginTask>::RemoveTask(Type* _pTask)
	{
		for (auto itr = m_pTaskList.begin(); itr != m_pTaskList.end(); itr++)
		{
			if (_pTask->GetID() == (*itr)->GetID())
			{
				m_pTaskList.erase(itr);
				break;
			}
		}
	}

	template <typename Type, typename BeginTask>
	inline void TaskManager<Type, BeginTask>::AddBeginTask(BeginTask* _pBeginTask)
	{
		m_pBeginTaskList.push_back(_pBeginTask);
		m_pBeginTaskList.sort(BeginTask::TaskCmp());
	}

	template <typename Type, typename BeginTask>
	inline void TaskManager<Type, BeginTask>::RemoveBeginTask(BeginTask* _pBeginTask)
	{
		for (auto itr = m_pBeginTaskList.begin(); itr != m_pBeginTaskList.end(); itr++)
		{
			if (_pBeginTask->GetID() == (*itr)->GetID())
			{
				m_pBeginTaskList.erase(itr);
				break;
			}
		}
	}

}


#endif // !LIB_TASKMANAGER_PRIVATE_H
