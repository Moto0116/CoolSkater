/**
 * @file	TaskBase_private.inl
 * @brief	タスクの基底クラス実装
 * @author	morimoto
 */


namespace Lib
{
	//----------------------------------------------------------------------
	// Static Private Variables
	//----------------------------------------------------------------------
	template<typename PriorityType>
	unsigned int TaskBase<PriorityType>::m_TaskNum = 0;

	//----------------------------------------------------------------------
	// Constructor	Destructor
	//----------------------------------------------------------------------
	template<typename PriorityType>
	TaskBase<PriorityType>::TaskBase() :
		m_Priority(0)
	{
		m_TaskID = m_TaskNum;
		m_TaskNum++;
	}

	template<typename PriorityType>
	TaskBase<PriorityType>::~TaskBase()
	{
	}


	//----------------------------------------------------------------------
	// Public Functions
	//----------------------------------------------------------------------
	template<typename PriorityType>
	void TaskBase<PriorityType>::Run()
	{
	}
}

