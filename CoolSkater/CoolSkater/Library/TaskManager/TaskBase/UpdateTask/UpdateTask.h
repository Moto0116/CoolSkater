/**
 * @file	UpdateTask.h
 * @brief	更新のタスククラス定義
 * @author	morimoto
 */
#ifndef LIB_UPDATETASK_H
#define LIB_UPDATETASK_H

//----------------------------------------------------------------------
// Include
//----------------------------------------------------------------------
#include "..\TaskBase.h"
#include "..\..\TaskManager.h"


namespace Lib
{
	class ObjectBase;

	/**
	 * 更新のタスククラス
	 */
	class UpdateTask : public TaskBase<>
	{
	public:
		/**
		 * コンストラクタ
		 */
		UpdateTask();
	
		/**
		 * デストラクタ
		 */
		~UpdateTask();
	
		/**
		 * 更新タスク実行
		 */
		virtual void Run();
	
		/**
		 * 更新オブジェクトをセット
		 * @param[in] _pObject 更新オブジェクト
		 */
		void SetUpdateObject(ObjectBase* _pObject);
	
	private:
		ObjectBase* m_pObject;	//!< 更新を行うオブジェクト.
		
	};


	typedef TaskManager<UpdateTask> UpdateTaskManager;
}



#endif // !LIB_UPDATETASK_H
